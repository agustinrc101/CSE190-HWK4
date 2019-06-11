#ifndef H_COMPONENT_SCOREBOARD
#define H_COMPONENT_SCOREBOARD
#pragma once

#include <iostream>
#include <vector>
#include "../Transform.h"
#include "Component.h"

class ComponentScoreboard : public Component {
public:
	bool isActive = true;

	ComponentScoreboard() {}
	~ComponentScoreboard() {}

	void Init(Transform * p) {
		transform = p;
		Start();
	}

	void Update(double deltaTime) override {

	}

	void increaseScore(bool playerRed) {
		if (playerRed) redScore++;
		else blueScore++;

		repaint();
	}

	void resetScore() {
		redScore = blueScore = 0;
		repaint();
	}

protected:
	Transform * transform;

	void Start() override {
		addRedChild(	glm::vec3(0.25f, 0.5f, 0.75f),		glm::vec3(0.1f, 0.05f, 0.2f));
		addRedChild(	glm::vec3(0.25f, 0.25f, 1),			glm::vec3(0.1f, 0.2f, 0.05f));
		addRedChild(	glm::vec3(0.25f, 0.25f, 0.5f),		glm::vec3(0.1f, 0.2f, 0.05f));
		addRedChild(	glm::vec3(0.25f, 0, 0.75f),			glm::vec3(0.1f, 0.05f, 0.2f));
		addRedChild(	glm::vec3(0.25f, -0.25f, 1),		glm::vec3(0.1f, 0.2f, 0.05f));
		addRedChild(	glm::vec3(0.25f, -0.25f, 0.5f),		glm::vec3(0.1f, 0.2f, 0.05f));
		addRedChild(	glm::vec3(0.25f, -0.5f, 0.75f),		glm::vec3(0.1f, 0.05f, 0.2f));
		
		addBlueChild(	glm::vec3(0.25f, 0.5f, -0.75f),		glm::vec3(0.1f, 0.05f, 0.2f));
		addBlueChild(	glm::vec3(0.25f, 0.25f, -0.5f),		glm::vec3(0.1f, 0.2f, 0.05f));
		addBlueChild(	glm::vec3(0.25f, 0.25f, -1),		glm::vec3(0.1f, 0.2f, 0.05f));
		addBlueChild(	glm::vec3(0.25f, 0, -0.75f),		glm::vec3(0.1f, 0.05f, 0.2f));
		addBlueChild(	glm::vec3(0.25f, -0.25f, -0.5f),	glm::vec3(0.1f, 0.2f, 0.05f));
		addBlueChild(	glm::vec3(0.25f, -0.25f, -1),		glm::vec3(0.1f, 0.2f, 0.05f));
		addBlueChild(	glm::vec3(0.25f, -0.5f, -0.75f),	glm::vec3(0.1f, 0.05f, 0.2f));

		initVectorValues();

		repaint();
	}

private:
	int redScore = 0;
	int blueScore = 0;

	std::vector<Transform *> redNum;
	std::vector<Transform *> blueNum;
	std::vector<std::vector<int>> values;

	void repaint() {
		if (redScore > 9) redScore = 0;
		if (blueScore > 9) blueScore = 0;

		for (int i = 0; i < redNum.size(); i++) {
			redNum[i]->material->color = glm::vec3(COLOR_BLACK);
			blueNum[i]->material->color = glm::vec3(COLOR_BLACK);
		}

		paint(redNum, values[redScore], glm::vec3(COLOR_RED));
		paint(blueNum, values[blueScore], glm::vec3(COLOR_BLUE));
	}

	void paint(std::vector<Transform *> t, std::vector<int> painting, glm::vec3 color) {
		for (int i = 0; i < painting.size(); i++)
			t[painting[i]]->material->color = color;
	}

	void addRedChild(glm::vec3 position, glm::vec3 scale) {
		Material * mat = new Material(Shaders::getColorShader(), glm::vec3(COLOR_RED));
		Transform * digital = new Transform(ProjectManager::project->getSphereModel(), mat);
		digital->translate(position);
		digital->scale(scale);
		redNum.push_back(digital);
		transform->addChild(digital);
	}

	void addBlueChild(glm::vec3 position, glm::vec3 scale) {
		Material * mat = new Material(Shaders::getColorShader(), glm::vec3(COLOR_BLUE));
		Transform * digital = new Transform(ProjectManager::project->getSphereModel(), mat);
		digital->translate(position);
		digital->scale(scale);
		blueNum.push_back(digital);
		transform->addChild(digital);
	}

	void initVectorValues() {
		std::vector<int> sc;
		//0
		sc.push_back(0);sc.push_back(1);sc.push_back(2);sc.push_back(4);sc.push_back(5);sc.push_back(6);
		values.push_back(sc);
		sc.clear();
		//1
		sc.push_back(2);sc.push_back(5);
		values.push_back(sc);
		sc.clear();
		//2
		sc.push_back(0);sc.push_back(2);sc.push_back(3);sc.push_back(4);sc.push_back(6);
		values.push_back(sc);
		sc.clear();
		//3
		sc.push_back(0);sc.push_back(2);sc.push_back(3);sc.push_back(5);sc.push_back(6);
		values.push_back(sc);
		sc.clear();
		//4
		sc.push_back(1);sc.push_back(2);sc.push_back(3);sc.push_back(5);
		values.push_back(sc);
		sc.clear();
		//5
		sc.push_back(0);sc.push_back(1);sc.push_back(3);sc.push_back(5);sc.push_back(6);
		values.push_back(sc);
		sc.clear();

		//6
		sc.push_back(0);sc.push_back(1);sc.push_back(3);sc.push_back(4);sc.push_back(5);sc.push_back(6);
		values.push_back(sc);
		sc.clear();

		//7
		sc.push_back(0);sc.push_back(2);sc.push_back(5);
		values.push_back(sc);
		sc.clear();

		//8
		sc.push_back(0);sc.push_back(1);sc.push_back(2);sc.push_back(3);sc.push_back(4);sc.push_back(5);sc.push_back(6);
		values.push_back(sc);
		sc.clear();

		//9
		sc.push_back(0);sc.push_back(1);sc.push_back(2);sc.push_back(3);sc.push_back(5);
		values.push_back(sc);
		sc.clear();
	}
};

#endif