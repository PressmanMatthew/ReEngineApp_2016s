#include "MyBoundingObject.h"

MyBoundingObject::MyBoundingObject(std::vector<vector3> vertexList)
{
	m_bColliding = false;
	m_fRadius = 0.0f;
	m_v3CenterGlobal = vector3(0.0f);

	if (vertexList.size() < 1)
		return;

	m_v3Min = vertexList[0];
	m_v3Max = vertexList[0];

	for (int i = 1; i < vertexList.size(); i++)
	{
		if (m_v3Min.x > vertexList[i].x)
		{
			m_v3Min.x = vertexList[i].x;
		}
		else if (m_v3Max.x < vertexList[i].x)
		{
			m_v3Max.x = vertexList[i].x;
		}

		if (m_v3Min.y > vertexList[i].y)
		{
			m_v3Min.y = vertexList[i].y;
		}
		else if (m_v3Max.y < vertexList[i].y)
		{
			m_v3Max.y = vertexList[i].y;
		}

		if (m_v3Min.z > vertexList[i].z)
		{
			m_v3Min.z = vertexList[i].z;
		}
		else if (m_v3Max.z < vertexList[i].z)
		{
			m_v3Max.z = vertexList[i].z;
		}
	}

	m_v3CenterLocal = m_v3CenterGlobal = (m_v3Max + m_v3Min) / 2.0f;

	m_fRadius = glm::distance(m_v3CenterGlobal, m_v3Max);

	m_pMeshMngr = MeshManagerSingleton::GetInstance();

	m_v3Size = m_v3Max - m_v3Min;

	m_v3MinG = m_v3Min;
	m_v3MaxG = m_v3Max;

}

void MyBoundingObject::RenderObject()
{
	m_v3Color = REGREEN;
	if (true == m_bColliding)
		m_v3Color = RERED;

	//CUBE
	if (isCube) {
		m_pMeshMngr->AddCubeToRenderList(
			m_m4ToWorld *
			glm::translate(m_v3CenterLocal) *
			glm::scale(m_v3Size),
			m_v3Color, WIRE);
	}
	//SPHERE
	else{
		m_pMeshMngr->AddSphereToRenderList(
			glm::translate(m_v3CenterGlobal) *
			glm::scale(vector3(m_fRadius) * 2.0f), m_v3Color, WIRE);
	}

}

void MyBoundingObject::SetModelMatrix(matrix4 a_m4ToWorld)
{
	if (m_m4ToWorld == a_m4ToWorld)
		return;

	m_m4ToWorld = a_m4ToWorld;
	m_v3CenterGlobal = vector3(m_m4ToWorld * vector4(m_v3CenterLocal, 1.0f));
	m_v3MinG = vector3(m_m4ToWorld * vector4(m_v3Min, 1.0f));
	m_v3MaxG = vector3(m_m4ToWorld * vector4(m_v3Max, 1.0f));
}

bool MyBoundingObject::IsColliding(MyBoundingObject * a_other)
{
	if (!isCube) {
		//SPHERE
		float fDistance = glm::distance(this->m_v3CenterGlobal, a_other->m_v3CenterGlobal);
		float fRadiiSum = this->m_fRadius + a_other->m_fRadius;
		return fDistance < fRadiiSum;
	}
	//CUBE
	else {
		if (this->m_v3MaxG.x < a_other->m_v3MinG.x)
			return false;
		if (this->m_v3MinG.x > a_other->m_v3MaxG.x)
			return false;

		if (this->m_v3MaxG.y < a_other->m_v3MinG.y)
			return false;
		if (this->m_v3MinG.y > a_other->m_v3MaxG.y)
			return false;

		if (this->m_v3MaxG.z < a_other->m_v3MinG.z)
			return false;
		if (this->m_v3MinG.z > a_other->m_v3MaxG.z)
			return false;
	}
	return true;
}

void MyBoundingObject::ToggleVisibility()
{	visible = !visible;
}

void MyBoundingObject::ToggleBoundingShape()
{
	isCube = !isCube;
}

void MyBoundingObject::SetColliding(bool input) { m_bColliding = input; }

void MyBoundingObject::SetCenterLocal(vector3 input) { m_v3CenterLocal = input; }

void MyBoundingObject::SetCenterGlobal(vector3 input) { m_v3CenterGlobal = input; }

void MyBoundingObject::SetRadius(float input) { m_fRadius = input; }

void MyBoundingObject::SetColor(vector3 input) { m_v3Color = input; }

bool MyBoundingObject::GetColliding(void) { return m_bColliding; }

vector3 MyBoundingObject::GetCenterLocal(void) { return m_v3CenterLocal; }

vector3 MyBoundingObject::GetCenterGlobal(void) { return m_v3CenterGlobal; }

matrix4 MyBoundingObject::GetModelMatrix(void) { return m_m4ToWorld; }

float MyBoundingObject::GetRadius(void) { return m_fRadius; }

bool MyBoundingObject::GetVisible(void) { return visible; }





