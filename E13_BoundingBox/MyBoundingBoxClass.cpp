#include "MyBoundingBoxClass.h"

MyBoundingBoxClass::MyBoundingBoxClass(std::vector<vector3> vertexList)
{
	m_bColliding = false;
	m_fRadius = 0.0f;
	m_v3CenterGlobal = vector3(0.0f);
	m_v3VertexList = vertexList;

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

	m_v3MaxLarge = m_v3Max;
	m_v3MinLarge = m_v3Min;
	
	//m_v3Size.x = glm::distance(vector3(m_v3Min.x, 0.0, 0.0), vector3(m_v3Max.x, 0.0, 0.0));
	//m_v3Size.y = glm::distance(vector3(0.0, m_v3Min.y, 0.0), vector3(0.0, m_v3Max.y, 0.0));
	//m_v3Size.z = glm::distance(vector3(0.0, 0.0, m_v3Min.z), vector3(0.0, 0.0, m_v3Max.z));
}

void MyBoundingBoxClass::RenderSphere()
{
	vector3 v3Color = REGREEN;
	if (true == m_bColliding)
		v3Color = RERED;

	m_pMeshMngr->AddCubeToRenderList(
		m_m4ToWorld *
		glm::translate(m_v3CenterLocal) *
		glm::scale(m_v3Size),
		v3Color, WIRE);

	std::vector<vector3> colliderPoints = std::vector<vector3>();
	colliderPoints.push_back(m_v3Max);
	colliderPoints.push_back(m_v3Min);
	colliderPoints.push_back(vector3(m_v3Max.x, m_v3Max.y, m_v3Min.z));
	colliderPoints.push_back(vector3(m_v3Min.x, m_v3Max.y, m_v3Min.z));
	colliderPoints.push_back(vector3(m_v3Min.x, m_v3Max.y, m_v3Max.z));
	colliderPoints.push_back(vector3(m_v3Max.x, m_v3Min.y, m_v3Min.z));
	colliderPoints.push_back(vector3(m_v3Max.x, m_v3Min.y, m_v3Max.z));
	colliderPoints.push_back(vector3(m_v3Min.x, m_v3Min.y, m_v3Max.z));

	for (int i = 0; i < 8; i++) {
		colliderPoints[i] = vector3(m_m4ToWorld * vector4(colliderPoints[i], 1.0f));
	}

	m_v3MaxLarge = colliderPoints[0];
	m_v3MinLarge = colliderPoints[0];

	for (int i = 1; i < colliderPoints.size(); i++) {
		if (m_v3MinLarge.x > colliderPoints[i].x)
		{
			m_v3MinLarge.x = colliderPoints[i].x;
		}
		else if (m_v3MaxLarge.x < colliderPoints[i].x)
		{
			m_v3MaxLarge.x = colliderPoints[i].x;
		}

		if (m_v3MinLarge.y > colliderPoints[i].y)
		{
			m_v3MinLarge.y = colliderPoints[i].y;
		}
		else if (m_v3MaxLarge.y < colliderPoints[i].y)
		{
			m_v3MaxLarge.y = colliderPoints[i].y;
		}

		if (m_v3MinLarge.z > colliderPoints[i].z)
		{
			m_v3MinLarge.z = colliderPoints[i].z;
		}
		else if (m_v3MaxLarge.z < colliderPoints[i].z)
		{
			m_v3MaxLarge.z = colliderPoints[i].z;
		}
	}

	m_v3SizeLarge = m_v3MaxLarge - m_v3MinLarge;

	m_pMeshMngr->AddCubeToRenderList(
		glm::translate(m_v3CenterGlobal) *
		glm::scale(m_v3SizeLarge),
		REBLUE, WIRE);

	//m_pMeshMngr->AddCubeToRenderList(
	//	glm::translate(m_v3CenterGlobal) * 
	//	glm::scale(m_v3SizeLarge),
	//	REBLUE, WIRE);
}
void MyBoundingBoxClass::SetModelMatrix(matrix4 a_m4ToWorld)
{
	if (m_m4ToWorld == a_m4ToWorld)
		return;

	m_m4ToWorld = a_m4ToWorld;
	m_v3CenterGlobal = vector3(m_m4ToWorld * vector4(m_v3CenterLocal, 1.0f));
	m_v3MinG = vector3(m_m4ToWorld * vector4(m_v3Min, 1.0f));
	m_v3MaxG = vector3(m_m4ToWorld * vector4(m_v3Max, 1.0f));

	std::cout << "MaxG (" << m_v3MaxG.x << ", " << m_v3MaxG.y << ", " << m_v3MaxG.z << ")" << std::endl;
}

bool MyBoundingBoxClass::IsColliding(MyBoundingBoxClass* a_other)
{
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

	return true;
}

void MyBoundingBoxClass::SetColliding(bool input) { m_bColliding = input; }
void MyBoundingBoxClass::SetCenterLocal(vector3 input) { m_v3CenterLocal = input; }
void MyBoundingBoxClass::SetCenterGlobal(vector3 input) { m_v3CenterGlobal = input; }
void MyBoundingBoxClass::SetRadius(float input) { m_fRadius = input; }
bool MyBoundingBoxClass::GetColliding(void) { return m_bColliding; }
vector3 MyBoundingBoxClass::GetCenterLocal(void) { return m_v3CenterLocal; }
vector3 MyBoundingBoxClass::GetCenterGlobal(void) { return m_v3CenterGlobal; }
float MyBoundingBoxClass::GetRadius(void) { return m_fRadius; }
matrix4 MyBoundingBoxClass::GetModelMatrix(void) { return m_m4ToWorld; }