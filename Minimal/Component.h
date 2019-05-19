#ifndef COMPONENT_H
#define COMPONENT_H

class Transform;

class Component {
public:
	Component() {}
	virtual ~Component() {}

	virtual void Start(Transform * p) = 0;
	virtual void Update(double deltaTime) = 0;

protected:
	Transform * transform;

	virtual void setTransform(Transform * p) { transform = p; }
};

#endif