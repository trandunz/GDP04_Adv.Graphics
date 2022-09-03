// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Scene.h 
// Description : Scene Interface Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

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

	float GetElapsedTime() { return m_ElapsedTime; };

protected:
	float m_ElapsedTime{ 0.0f };
};

