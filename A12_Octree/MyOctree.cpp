#include "MyOctree.h"

MyOctree::MyOctree(void)
{
}

MyOctree::MyOctree(int a_depth, float a_maxX, float a_minX, float a_maxY, float a_minY, float a_maxZ, float a_minZ)
{
	m_pBOManagerInstance = MyBOManager::GetInstance();

	m_pMeshMngr = MeshManagerSingleton::GetInstance();
	
	centerX = (a_maxX + a_minX) / 2;
	centerY = (a_maxY + a_minY) / 2;
	centerZ = (a_maxZ + a_minZ) / 2;

	m_depth = a_depth;
	
	m_maxX = a_maxX;
	m_minX = a_minX;

	m_maxY = a_maxY;
	m_minY = a_minY;
		
	m_maxZ = a_maxZ;
	m_minZ = a_minZ;

	m_pMeshMngr->AddCubeToRenderList(glm::translate(centerX, centerY, centerZ) * glm::scale(vector3(m_maxX - m_minX)), RERED, WIRE);
	m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(0)) * glm::scale(vector3(100)), RERED, WIRE);




	//figure out what objects are in the quad
	
	AddObjects();

	if (a_depth <= 0) {
		hasChildren = false;
	}
	else {
		hasChildren = true;
		if (BOList.size() > 5) {
			Divide();
		}
	}
}



void MyOctree::AddObjects()
{
	//go through all objects in MyBOClass?? and see if they are in this cube
	
	AllObjects = m_pBOManagerInstance->Get_m_lObject();

	for (uint i = 0; i < AllObjects.size(); i++) {
		//check if CurrentObj is in the same space as the current octree box that you are in
		//if it is, add it to the current octree boxes BOList

		if (AllObjects[i]->GetCenterGlobal().x > m_minX &&
			AllObjects[i]->GetCenterGlobal().x < m_maxX &&
			AllObjects[i]->GetCenterGlobal().y > m_minY &&
			AllObjects[i]->GetCenterGlobal().y < m_maxY &&
			AllObjects[i]->GetCenterGlobal().z > m_minZ &&
			AllObjects[i]->GetCenterGlobal().z < m_maxZ) {

			BOList.push_back(AllObjects[i]);
		}
	}
}

void MyOctree::checkCollisions(void) {
	for (uint i = 0; i < BOList.size(); i++) {
		for (uint j = 0; j < BOList.size(); j++) {
			if (j != i) {
				BOList[i]->IsColliding(BOList[j]);
			}
		}
	}
}

void MyOctree::Divide(void) {

	Children.push_back(new MyOctree(m_depth - 1, centerX, m_minX, centerY, m_minY, centerZ, m_minZ));

	Children.push_back(new MyOctree(m_depth - 1, m_maxX, centerX, m_maxY, centerY, m_maxZ, centerZ));

	Children.push_back(new MyOctree(m_depth - 1, centerX, m_minX, centerY, m_minY, m_maxZ, centerZ));

	Children.push_back(new MyOctree(m_depth - 1, centerX, m_minX, m_maxY, centerY, m_maxZ, centerZ));

	Children.push_back(new MyOctree(m_depth - 1, m_maxX, centerX, centerY, m_minY, centerZ, m_minZ));

	Children.push_back(new MyOctree(m_depth - 1, m_maxX, centerX, m_maxY, centerY, centerZ, m_minZ));

	Children.push_back(new MyOctree(m_depth - 1, m_maxX, centerX, centerY, m_minY, m_maxZ, centerZ));

	Children.push_back(new MyOctree(m_depth - 1, centerX, m_minX, m_maxY, centerY, centerZ, m_minZ));


	//
}

void MyOctree::Display(void)
{
	for (int i = 0; i < Children.size(); i++) {
		Children[i]->Display();
	}
	m_pMeshMngr->AddCubeToRenderList(glm::translate(centerX, centerY, centerZ) * glm::scale(vector3(m_maxX - m_minX)), RERED, WIRE);

}

MyOctree::~MyOctree(void)
{
}
