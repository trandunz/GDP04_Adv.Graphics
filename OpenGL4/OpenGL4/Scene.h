#pragma once
class Scene
{
public:
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void KeyEvents() {};
	virtual void CursorMoveEvent(double& xpos, double& ypos) {};
	virtual void CursorClickEvent(int button, int action, int mods) {};
	virtual void Draw() = 0;
};

