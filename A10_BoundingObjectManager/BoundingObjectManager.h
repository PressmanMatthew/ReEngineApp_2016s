#pragma once
#include "RE\ReEng.h"
#include "MyBoundingObject.h"
#include <string>

class BoundingObjectManager {
private:

	std::vector<MyBoundingObject> m_boBoundingObjects;
	std::vector<String> m_vsNames;
	int m_iCount;
	static BoundingObjectManager* instance;

	BoundingObjectManager() {
		m_boBoundingObjects = std::vector<MyBoundingObject>();
		m_vsNames = std::vector<String>();
		m_iCount = 0;
	};
	//BoundingObjectManager(BoundingObjectManager const& other) {
	//	//Copy Members here
	//
	//	instance = other.GetInstance();
	//};
	//BoundingObjectManager& operator=(BoundingObjectManager const& other) {
	//	//Copy Members here
	//
	//	instance = other.GetInstance();
	//};
public:

	static BoundingObjectManager* GetInstance() {
		if (instance == nullptr) {
			instance = new BoundingObjectManager();
		}
		return instance;
	}

	static void ReleaseInstance() {
		if (instance != nullptr) {
			delete instance;
			instance = nullptr;
		}
	}

	void NewBoundingBox(std::vector<vector3> a_vv3VertexList, String a_sName) {
		m_boBoundingObjects.push_back(MyBoundingObject(a_vv3VertexList));
		m_vsNames.push_back(a_sName);
	}

	void SwitchType(String a_sName) {
		//if (m_boBoundingObjects.find(a_sName) != m_boBoundingObjects.end) {
		int index = 0;
		for (uint i = 0; i < m_vsNames.size(); i++) {
			if (a_sName == m_vsNames[i]) {
				index = i;
				break;
			}
		}
		m_boBoundingObjects[index].ToggleBoundingShape();
		//}
	}

	int GetCount() {
		return m_iCount;
	}

	void ChangeColor(String a_sName, vector3 a_v3Color) {
		//if (m_boBoundingObjects.find(a_sName) != m_boBoundingObjects.end) {
			int index = 0;
			for (uint i = 0; i < m_vsNames.size(); i++) {
				if (a_sName == m_vsNames[i]) {
					index = i;
					break;
				}
			}
			m_boBoundingObjects[index].SetColor(a_v3Color);
		//}
	}

	void ToggleVisibility(String a_sName) {
		//if (m_boBoundingObjects.find(a_sName) != m_boBoundingObjects.end) {
		int index = 0;
		for (uint i = 0; i < m_vsNames.size(); i++) {
			if (a_sName == m_vsNames[i]) {
				index = i;
				break;
			}
		}
		m_boBoundingObjects[index].ToggleVisibility();
		//}
	}

	void Render(String a_sName) {
		if (a_sName == "ALL") {
			for (uint i = 0; i < m_boBoundingObjects.size(); i++) {
				m_boBoundingObjects[i].RenderObject();
			}
			
		}
		else {
			int index = 0;
			for (uint i = 0; i < m_vsNames.size(); i++) {
				if (a_sName == m_vsNames[i]) {
					index = i;
					break;
				}
			}
			m_boBoundingObjects[index].RenderObject();
		}
	}

	void CheckCollisions() {



		//std::map<const String, MyBoundingObject>::iterator it1;
		//std::map<const String, MyBoundingObject>::iterator it2;

		//for (it1 = m_boBoundingObjects.begin(); it1 != m_boBoundingObjects.end(); it1++)
		//{
		//	for (it2 = m_boBoundingObjects.begin(); it2 != m_boBoundingObjects.end(); it2++)
		//	{
		//		if (it1->first != it2->first) {
		//			//Check for Collisions between the 2 and change the colors to red if they are colliding
		//		}
		//	}
		//}
	}
};