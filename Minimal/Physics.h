#ifndef H_PHYSICS
#define H_PHYSICS
#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "btBulletDynamicsCommon.h"

#include "Definitions.h"


// Helper class; draws the world as seen by Bullet.
// This is very handy to see it Bullet's world matches yours
// How to use this class :
// Declare an instance of the class :
// 
// dynamicsWorld->setDebugDrawer(&mydebugdrawer);
// Each frame, call it :
// mydebugdrawer.SetMatrices(ViewMatrix, ProjectionMatrix);
// dynamicsWorld->debugDrawWorld();

#include "Shaders.h"

namespace bullet {
	inline glm::vec3 ToGlm(const btVector3& v) { return glm::vec3(v.getX(), v.getY(), v.getZ()); }

	inline btVector3 fromGlm(const glm::vec3& v) { return btVector3(v.x, v.y, v.z); }

	inline glm::quat ToGlm(const btQuaternion& q) { return glm::quat(q.getW(), q.getX(), q.getY(), q.getZ()); }

	inline btQuaternion fromGlm(const glm::quat& q) { return btQuaternion(q.x, q.y, q.z, q.w); }

	inline btMatrix3x3 fromGlm(const glm::mat3& m) { return btMatrix3x3(m[0][0], m[1][0], m[2][0], m[0][1], m[1][1], m[2][1], m[0][2], m[1][2], m[2][2]); }

	// btTransform does not contain a full 4x4 matrix, so this transform is lossy.
	// Affine transformations are OK but perspective transformations are not.
	inline btTransform fromGlm(const glm::mat4& m) {
		glm::mat3 m3(m);
		return btTransform(fromGlm(m3), fromGlm(glm::vec3(m[3][0], m[3][1], m[3][2])));
	}

	inline glm::mat4 ToGlm(const btTransform& t) {
		glm::mat4 m = glm::mat4(1);
		const btMatrix3x3& basis = t.getBasis();
		// rotation
		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				m[c][r] = basis[r][c];
			}
		}
		// traslation
		btVector3 origin = t.getOrigin();
		m[3][0] = origin.getX();
		m[3][1] = origin.getY();
		m[3][2] = origin.getZ();
		// unit scale
		m[0][3] = 0.0f;
		m[1][3] = 0.0f;
		m[2][3] = 0.0f;
		m[3][3] = 1.0f;
		return m;
	}
};

class BulletDebugDrawer_OpenGL : public btIDebugDraw {
public:

	BulletDebugDrawer_OpenGL() {}
	~BulletDebugDrawer_OpenGL() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}

	void SetMatrices(glm::mat4 headPose, glm::mat4 projection) {
		GLuint shader = Shaders::getColorShader();
		glUseProgram(shader);

		glUniformMatrix4fv(glGetUniformLocation(shader, "Projection"), 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader, "View"), 1, GL_FALSE, &headPose[0][0]);

	}

	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
		// Vertex data
		GLfloat points[12];

		points[0] = from.x();
		points[1] = from.y();
		points[2] = from.z();
		points[3] = color.x();
		points[4] = color.y();
		points[5] = color.z();

		points[6] = to.x();
		points[7] = to.y();
		points[8] = to.z();
		points[9] = color.x();
		points[10] = color.y();
		points[11] = color.z();

		GLuint shader = Shaders::getColorShader();
		glm::vec3 colors = bullet::ToGlm(color);
		glUseProgram(shader);
		glUniform3f(glGetUniformLocation(shader, "Color"), colors.r, colors.g, colors.b);

		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glBindVertexArray(0);

		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, 2);
		glBindVertexArray(0);

	}
	virtual void drawContactPoint(const btVector3 &, const btVector3 &, btScalar, int, const btVector3 &) {}
	virtual void reportErrorWarning(const char *) {}
	virtual void draw3dText(const btVector3 &, const char *) {}
	virtual void setDebugMode(int p) {
		m = p;
	}
	int getDebugMode(void) const { return 3; }
	int m;

private:
	GLuint VBO, VAO;
};

class Physics{
public:
	static Physics * physics;

	Physics();
	~Physics();

	void update(double deltaTime);
	void draw(glm::mat4 headPose, glm::mat4 projection);

	static btRigidBody* addPlaneCollider(float size, glm::vec3 position, glm::vec3 axis = AXIS_Y_POSITIVE);
	static btRigidBody* addSphereCollider(float radius, glm::vec3 position, float Mass = 1.0f);
	static btRigidBody* addBoxCollider(glm::vec3 size, glm::vec3 position, float Mass = 1.0f);

	static btRigidBody* getRigidbody(int index);
	static int getCollisionObjectCount() { return physics->dynamicsWorld->getNumCollisionObjects(); }

private:
	BulletDebugDrawer_OpenGL * debugDrawer;

	btRigidBody * groundCollider = 0;

	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

};


#endif