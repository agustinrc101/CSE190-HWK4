#ifndef H_COMPONENT
#define H_COMPONENT
#pragma once

class Transform;

class Component {
public:
	bool isActive = true;

	Component() {}
	virtual ~Component() {}

	virtual void Init(Transform * p) = 0;
	virtual void LateInit() { };
	virtual void Update(double deltaTime) { };


protected:
	Transform * transform;

	virtual void Start() {}
};

#endif