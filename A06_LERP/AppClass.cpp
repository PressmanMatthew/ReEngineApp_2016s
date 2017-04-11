#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	//Initialize Sphere
	sphere = new PrimitiveClass();
	sphere->GenerateSphere(.1f, 3, vector3(1, 0, 0));

	//Add all the points to the array
	points[0] = vector3(-4.0f, -2.0f, 5.0f);
	points[1] = vector3(1.0f, -2.0f, 5.0f);
	points[2] = vector3(-3.0f, -1.0f, 3.0f);
	points[3] = vector3(2.0f, -1.0f, 3.0f);
	points[4] = vector3(-2.0f, 0.0f, 0.0f);
	points[5] = vector3(3.0f, 0.0f, 0.0f);
	points[6] = vector3(-1.0f, 1.0f, -3.0f);
	points[7] = vector3(4.0f, 1.0f, -3.0f);
	points[8] = vector3(0.0f, 2.0f, -5.0f);
	points[9] = vector3(5.0f, 2.0f, -5.0f);
	points[10] = vector3(1.0f, 3.0f, -5.0f);

	//Make sure every sphere has a transformation matrix with the correct position
	for (int i = 0; i < 11; i++) {
		spherePos[i] = glm::translate( IDENTITY_M4, points[i]);
	}

	//Initialize Starting position and transform matrix
	transform = IDENTITY_M4;
	currentPos = points[0];

	fDuration = 1.0f;
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
#pragma endregion

#pragma region Your Code goes here
	lastPoint = (int)fRunTime % 10; //Get the last point visited to know which 2 points it should be between
	currentPerc = ((float)fRunTime - ((int)fRunTime)) * fDuration; //Get the current percentage it should have moved between the 2 points and make it processor independant

	//Lerp between the last point visited and the next point using the current percentage it should be at bewteen the 2 
	currentPos = glm::lerp(points[lastPoint], points[lastPoint + 1], currentPerc);

	transform = glm::translate(currentPos); //Make the transform it will use dependant on that lerped position

	m_pMeshMngr->SetModelMatrix(transform, "WallEye");

	
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	//Render Spheres
	for (int i = 0; i < 11; i++) {
		sphere->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), spherePos[i]);
	}
	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(sphere);
	super::Release(); //release the memory of the inherited fields
}