#include "Camera.h"

Camera::Camera() {
	Camera(vector3(0.0f, 0.0f, 1.0f), vector3(0.0f, 0.0f, 0.0f), REAXISY, vector2(0.0f, 0.0f));
}

Camera::Camera(vector3 target, vector3 position, vector3 up, vector2 mousePos)
{
	// Sets target, position, and up according to parameter values
	m_v3Target = target;
	m_v3Position = position;
	m_v3Up = up;
	// Sets the right value based on the up and target parameter values
	m_v3Right = glm::normalize(glm::cross(up, target));

	// Creates default front and back clipping planes relative to the
		// camera's and target's positions, respectively
	m_cpFront.v3BottomLeft = vector3(- 10.80f, - 7.68f, 5.0f);
	m_cpFront.v3TopRight = vector3(10.80f, 7.68f, 5.0f);

	m_cpBack.v3BottomLeft = vector3(- 10.80f, - 7.68f, 1000.0f);
	m_cpBack.v3TopRight = vector3(10.80f, 7.68f, 1000.0f);

	// Sets default fov to orthographic, not perspective
	m_bOrthographic = true;

	// Sets initial fov to 45.0f; currently cannot be changed
	fov = 45.0f;

	pitch = 0.0f;
	yaw = 0.0f;
	roll = 0.0f;

	prevMouse = -mousePos;
}

matrix4 Camera::GetView(vector2 vec)
{
	UpdateViewMatrix(vec);
	return m_m4View;
}

matrix4 Camera::GetProjection(bool bOrthographic)
{
	m_bOrthographic = bOrthographic;
	UpdateProjectionMatrix();
	return m_m4Projection;
}

void Camera::SetPosition(vector3 v3Position)
{
	m_v3Position = v3Position;
}

void Camera::SetTarget(vector3 v3Target)
{
	m_v3Target = v3Target;
}

void Camera::SetUp(vector3 v3Up)
{
	m_v3Up = v3Up;
}
// NEEDS TO BE LOOKED AT
void Camera::MoveForward(float fIncrement)
{
	// For Z position - target
	fIncrement *= 3;
	m_v3Target.z -= fIncrement;
	//m_v3Up.z -= fIncrement;
	//m_v3Right.z -= fIncrement;

	m_v3Position.z -= fIncrement;
}
// NEEDS TO BE LOOKED AT
void Camera::MoveSideways(float fIncrement)
{
	// For X position - right
	fIncrement *= 3;
	//m_v3Target.x += fIncrement;
	//m_v3Up.x += fIncrement;
	m_v3Right.x += fIncrement;

	m_v3Position.x += fIncrement;
}
// NEEDS TO BE LOOKED AT
void Camera::MoveVertical(float fIncrement)
{
	// For Y position - up
	fIncrement *= 3;
	//m_v3Target.y += fIncrement;
	//m_v3Up.y += fIncrement;
	//m_v3Right.y += fIncrement;

	m_v3Position.y += fIncrement;
	std::cout << "CAM POSITION: " << m_v3Position.x << ", " << m_v3Position.y << ", " << m_v3Position.z << std::endl;
}

void Camera::ChangePitch(float fIncrement)
{
	pitch += fIncrement;
}

void Camera::ChangeRoll(float fIncrement)
{
	roll += fIncrement;
}

void Camera::ChangeYaw(float fIncrement)
{
	yaw += fIncrement;
}

void Camera::SetFrontClippingPlane(vector3 bottomL, vector3 topR)
{
	m_cpFront.v3BottomLeft = bottomL;
	m_cpFront.v3TopRight = topR;
}

void Camera::SetBackClippingPlane(vector3 bottomL, vector3 topR)
{
	m_cpBack.v3BottomLeft = bottomL;
	m_cpBack.v3TopRight = topR;
}

void Camera::UpdateViewMatrix(vector2 vec)
{
	/*
	m_v3Target.x *= sin(pitch);
	m_v3Target.z *= cos(roll);

	m_v3Right.x *= cos(pitch);
	m_v3Right.z *= -sin(roll);
	*/

	vector2 diffMouse = -vec - prevMouse;

	diffMouse.x = glm::radians(diffMouse.x);
	diffMouse.y = glm::radians(diffMouse.y);

	diffMouse.x /= 5.0f;
	diffMouse.y /= 5.0f;

	pitch += diffMouse.y;
	yaw += diffMouse.x;

	glm::quat rotation = glm::quat(vector3(pitch,
										   yaw,
										   roll));

	prevMouse = -vec;

	m_m4View = ToMatrix4(rotation);
	m_m4View *= glm::lookAt(m_v3Target, m_v3Position, m_v3Up);
	/* OLD WAY OF DOING THINGS, BECAUSE I CAN READ DIRECTIONS */
	//// X
	//vector3 crossX = glm::cross(vector3(1.0f, 0.0f, 0.0f), m_v3Right);			// Axis of rotation for X
	//float dotX = glm::acos(glm::dot(vector3(1.0f, 0.0f, 0.0f), m_v3Right));		// Angle of rotation for X
	//
	//// Y
	//vector3 crossY = glm::cross(vector3(0.0f, 1.0f, 0.0f), m_v3Up);				// Axis of rotation for Y
	//float dotY = glm::acos(glm::dot(vector3(0.0f, 1.0f, 0.0f), m_v3Up));		// Angle of rotation for Y
	//
	//// Z
	//vector3 crossZ = glm::cross(vector3(0.0f, 0.0f, 1.0f), -m_v3Target);			// Axis of rotation for Z
	//float dotZ = glm::acos(glm::dot(vector3(0.0f, 0.0f, 1.0f), -m_v3Target));	// Angle of rotation for Z
	//
	//glm::quat quatX = glm::angleAxis(dotX, crossX);		// Quaternion holding X orientation
	//glm::quat quatY = glm::angleAxis(dotY, crossY);		// Quaternion holding Y orientation
	//glm::quat quatZ = glm::angleAxis(dotZ, crossZ);		// Quaternion holding Z orientation
	//
	//glm::quat orientation = quatX * quatY * quatZ;		// Sum of all orientations
	//
	//m_m4View = glm::translate(m_v3Position) * glm::mat4_cast(orientation);
}

void Camera::UpdateProjectionMatrix()
{
	if (m_bOrthographic) {
		m_m4Projection = glm::ortho(m_cpFront.v3BottomLeft.x, m_cpFront.v3TopRight.x, m_cpFront.v3BottomLeft.y, m_cpFront.v3TopRight.y, m_cpFront.v3BottomLeft.z, m_cpBack.v3BottomLeft.z);
	}
	else {
		m_m4Projection = glm::perspective(fov, 1080.0f / 768.0f, m_cpFront.v3BottomLeft.z, m_cpBack.v3BottomLeft.z);
	}
}