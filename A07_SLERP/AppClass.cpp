#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("SLERP - YOUR USER NAME GOES HERE"); // Window Name

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);
}

void AppClass::InitVariables(void)
{
	//Setting the position in which the camera is looking and its interest point
	m_pCameraMngr->SetPositionTargetAndView(vector3(12.12f, 28.52f, 11.34f), ZERO_V3, REAXISY);

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);

	//Loading the models
	m_pMeshMngr->LoadModel("Planets\\00_Sun.obj", "Sun");
	m_pMeshMngr->LoadModel("Planets\\03_Earth.obj", "Earth");
	m_pMeshMngr->LoadModel("Planets\\03A_Moon.obj", "Moon");

	//Setting the days duration
	m_fDay = 1.0f;
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Getting the time between calls
	double fCallTime = m_pSystem->LapClock();
	//Counting the cumulative time
	static double fRunTime = 0.0f;
	fRunTime += fCallTime;

	//Earth Orbit
	double fEarthHalfOrbTime = 182.5f * m_fDay; //Earths orbit around the sun lasts 365 days / half the time for 2 stops
	float fEarthHalfRevTime = 0.5f * m_fDay; // Move for Half a day
	float fMoonHalfOrbTime = 14.0f * m_fDay; //Moon's orbit is 28 earth days, so half the time for half a route

	//Relative Distances
	matrix4 earthDist = glm::translate(vector3(11, 0, 0));
	matrix4 moonDist = glm::translate(vector3(2, 0, 0));

	//Percentages
	float earthOrbitPerc = fRunTime / fEarthHalfOrbTime;
	float earthRotPerc = fRunTime / fEarthHalfRevTime;
	float moonOrbitPerc = fRunTime / fMoonHalfOrbTime;

	//Create Quaternions
	glm::quat earthOrbitStart = glm::quat(vector3(0));
	glm::quat earthOrbitEnd = glm::quat(vector3(0, 180, 0));

	glm::quat earthRotStart = glm::quat(vector3(0));
	glm::quat earthRotEnd = glm::quat(vector3(0, 180, 0));

	glm::quat moonOrbitStart = glm::quat(vector3(0));
	glm::quat moonOrbitEnd = glm::quat(vector3(0, 180, 0));

	//SLERP between quaternions
	glm::quat earthOrbitQuat = glm::mix(earthOrbitStart, earthOrbitEnd, earthOrbitPerc);
	glm::quat earthRotQuat = glm::mix(earthRotStart, earthRotEnd, earthRotPerc);
	glm::quat moonOrbitQuat = glm::mix(moonOrbitStart, moonOrbitEnd, moonOrbitPerc);

	//Orbits
	matrix4 earthOrbit = glm::mat4_cast(earthOrbitQuat);
	matrix4 moonOrbit = glm::mat4_cast(moonOrbitQuat);

	//Rotations
	matrix4 earthRotation = glm::mat4_cast(earthRotQuat);//glm::rotate(IDENTITY_M4, m_fEarthTimer * 28.0f, vector3(0, 1, 0));

	//Put Matrices together
	matrix4 earthSpace = earthOrbit * earthDist;
	m_m4Earth = earthSpace * earthRotation;

	matrix4 moonSpace = moonOrbit * moonDist;
	m_m4Moon = earthSpace * moonSpace;

	m_m4Sun = glm::translate(IDENTITY_M4, vector3(0));

	//Scaling
	m_m4Sun = glm::scale(m_m4Sun, m_v3SunScale);
	m_m4Earth = glm::scale(m_m4Earth, m_v3EarthScale);
	m_m4Moon = glm::scale(m_m4Moon, m_v3MoonScale);

	//Setting the matrices
	m_pMeshMngr->SetModelMatrix(m_m4Sun, "Sun");
	m_pMeshMngr->SetModelMatrix(m_m4Earth, "Earth");
	m_pMeshMngr->SetModelMatrix(m_m4Moon, "Moon");

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Change Orbit and Revolution Counts
	nEarthOrbits = (int)earthOrbitPerc / 2;
	nEarthRevolutions = (int)earthRotPerc / 2;
	nMoonOrbits = (int)moonOrbitPerc / 2;

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
	m_pMeshMngr->Print("Time:");
	m_pMeshMngr->PrintLine(std::to_string(fRunTime));

	m_pMeshMngr->Print("Day:");
	m_pMeshMngr->PrintLine(std::to_string(m_fDay));

	m_pMeshMngr->Print("E_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthOrbits));

	m_pMeshMngr->Print("E_Revolutions:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthRevolutions));

	m_pMeshMngr->Print("M_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nMoonOrbits));

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default:
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList();
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}