#ifndef H_COMPONENT_SEND_TEST_PACKET
#define H_COMPONENT_SEND_TEST_PACKET
#pragma once

#include <iostream>
#include "Transform.h"
#include "Component.h"
#include "Input.h"
#include "../Networking/Client.h"

class ComponentSendTestPacket : public Component {
public:
	ComponentSendTestPacket() {}
	~ComponentSendTestPacket() {}

	void Init(Transform * p) {
		transform = p;
		Start();
	}

	void Update(double deltaTime) override {
		if (Input::getButtonB()) {
			if (!button_press) {
				button_press = true;

				Packet p;
				p.type = TEST;
				std::cout << "SENDING TEST PACKET" << std::endl;
				Client::sendPacket(p);
			}
		}
		else button_press = false;
	}

protected:
	Transform * transform;

	void Start() override {

	}

private:
	bool button_press = false;
};

#endif
