#ifndef H_COMPONENT
#define H_COMPONENT
#pragma once

class Transform;

class Component {
public:
	Component() {}
	virtual ~Component() {}

	virtual void Init(Transform * p) = 0;
	virtual void Update(double deltaTime) { };

protected:
	Transform * transform;

	virtual void Start() {}
};

#endif