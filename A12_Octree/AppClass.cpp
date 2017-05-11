#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Separation Axis Test"); // Window Name 

											   // Set the clear color based on Microsoft's CornflowerBlue (default in XNA)
											   //if this line is in Init Application it will depend on the .cfg file, if it
											   //is on the InitVariables it will always force it regardless of the .cfg
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
}

void AppClass::InitVariables(void)
{
	//Set the camera at a position other than the default
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 2.5f, 150.0f), vector3(0.0f, 2.5f, 11.0f), REAXISY);
	m_pBOMngr = (MyBOManager::GetInstance());
	m_nInstances = 220;
	int nSquare = static_cast<int>(std::sqrt(m_nInstances));
	m_nInstances = nSquare * nSquare;
	for (int i = 0; i < nSquare; i++)
	{
		for (int j = 0; j < nSquare; j++)
		{
			String sInstance = "Cube_" + std::to_string(i) + "_" + std::to_string(j);
			matrix4 m4Positions = glm::translate(static_cast<float>(i - nSquare / 2.0f), static_cast<float>(j), 0.0f);
			m4Positions = glm::translate(vector3(glm::sphericalRand(35.0f)));
			m_pMeshMngr->LoadModel("Portal\\CompanionCube.bto", sInstance, false, m4Positions);
			m_pBOMngr->AddObject(sInstance);
		}
	}

	m_MyOctree = new MyOctree(8, 40.0f, -40.0f, 40.0f, -40.0f, 40.0f, -40.0f);
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();
	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	ArcBall();

	if (!spatialOpt) {
		m_pBOMngr->Update();//Update collision detection
	}
	else {
		m_MyOctree->checkCollisions();
	}
	//m_pBOMngr->DisplayOriented("ALL");

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	Prints();
}
void AppClass::Prints(void)
{
	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
										  //Print info on the screen
	std::vector<int> list = m_pBOMngr->GetCollidingVector(0);
	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->Print("Object 0 colliding with: ", REBLUE);
	for (uint n = 0; n < list.size(); n++)
	{
		m_pMeshMngr->Print(std::to_string(list[n]) + " ", REYELLOW);
	}
	m_pMeshMngr->PrintLine(" ");
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->PrintLine(std::to_string(nFPS), RERED);

	m_pMeshMngr->Print("<O>View Octree\n");
	if (spatialOpt) {
		m_pMeshMngr->Print("<P>Brute Force\n");

	}
	else {
		m_pMeshMngr->Print("<P>Spatial Optimization\n");
	}
	


}

void AppClass::Display(void)
{
	
	//clear the screen
	ClearScreen();
	//Render the grid based on the camera's mode:
	if (viewOctree) {
		m_MyOctree->Display();
	}
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
	MyBOManager::ReleaseInstance();
}