#pragma once
#include "RE\ReEngAppClass.h"
#include "ClippingPlane.h"

class Camera {
private:
	vector3 m_v3Position;
	vector3 m_v3Up;
	vector3 m_v3Right;
	vector3 m_v3Target;

	matrix4 m_m4View;
	matrix4 m_m4Projection;
	ClippingPlane m_cpFront;
	ClippingPlane m_cpBack;

	bool m_bOrthographic;

	float fov;
	float pitch, yaw, roll;

	vector2 prevMouse;

public:
	Camera();
	Camera(vector3 target, vector3 position, vector3 up, vector2 mousePos);
	matrix4 GetView(vector2 vec); //Gets the View Matrix
	matrix4 GetProjection(bool bOrthographic); //Gets the Projection Matrix (The bool lets you select between ortho and persp)
	
	void SetPosition(vector3 v3Position); //Sets the position of the camera
	void SetTarget(vector3 v3Target); //Sets the target of the camera
	void SetUp(vector3 v3Up); //Sets the up vector of the camera

	//Move the camera depending on the "view" vector
	void MoveForward(float fIncrement); //Move forward or backwards based on the "view" vector
	void MoveSideways(float fIncrement); //Based on the "right" vector
	void MoveVertical(float fIncrement); //Up or down depending on the "up" vector
	
	//Orient the camera (meaning the forward and up vectors) 
	void ChangePitch(float fIncrement); //Changing the angle in the x
	void ChangeRoll(float fIncrement); //Changing its angle in the z
	void ChangeYaw(float fIncrement); //Changing its angle in y

	// Clipping plane methods
	void SetFrontClippingPlane(vector3 bottomL, vector3 topR); // Changing the front clipping plane's position
	void SetBackClippingPlane(vector3 bottomL, vector3 topR); // Changing the back clipping plane's position

private:

	// Helper functions - updates the matrices when vec3 variables have been changed
	void UpdateViewMatrix(vector2 vec);
	void UpdateProjectionMatrix();
};