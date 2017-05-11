#ifndef __MYOCTREE_H_
#define __MYOCTREE_H_

#include "MyBOClass.h"
#include "MyBOManager.h"
//#include <list>

class MyOctree 
{
	int m_depth;
	const int MAX_OBJ_BEFORE_DIVIDE = 10;
	bool hasChildren;

	float centerX;
	float centerY;
	float centerZ;

	float m_maxX;
	float m_minX;
	
	float m_maxY;
	float m_minY;

	float m_maxZ;
	float m_minZ;

	std::vector<MyOctree*> Children;

	std::vector<MyBOClass*> BOList;

	std::vector<MyBOClass*> AllObjects;

	MeshManagerSingleton* m_pMeshMngr = nullptr;

	MyBOManager* m_pBOManagerInstance = nullptr;
public:

	MyOctree(void);
	MyOctree(int a_depth, float a_maxX, float a_minX, float a_maxY, float a_minY, float a_maxZ, float a_minZ);
	void AddObjects();
	void Divide(void);

	void Display(void);

	void checkCollisions(void);

	//MyOctree(MyOctree const& other);
	//MyOctree& operator=(MyOctree const& other);
	
	~MyOctree(void);

private:

};

#endif //__MYOCTREE_H_