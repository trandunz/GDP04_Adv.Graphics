#pragma once
class Scene
{
public:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void KeyEvents() {};
	virtual void MouseEvents(double& xpos, double& ypos) {};
	virtual void Draw() = 0;
};

