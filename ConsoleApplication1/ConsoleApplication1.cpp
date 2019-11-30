#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "AllHeader.h"
#include "Vector3.h"
#include "Color.h"
#include "ColorDeclaration.h"
#include "GameObject.h"
#include "NeonShooter.h"
#include "DataPacker.h"
#include "GUI.h"
#include "DataPacker.h"

const int RESOLUTION_X = 800;
const int RESOLUTION_Y = 600;
bool ShowConsoleWindow;
float scrollX = 0.0f, scrollY = 0.0f;
float zoom = 1.0f;
NeonShooter neonShooter;

void Stealth(int cmdShow)
{
	HWND Stealth;
	AllocConsole();
	Stealth = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(Stealth, cmdShow);
}

void OrthographicView(int width, int height)
{
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	glOrtho(scrollX, scrollX + width / zoom, scrollY + height / zoom, scrollY, -1, 1);
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();
}

void onWindowResized(GLFWwindow* window, int width, int height)
{
	glViewport(0.0f, 0.0f, width, height);				// Reset The Current Viewport
	OrthographicView(width, height);
}

void WindowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		ShowConsoleWindow = !ShowConsoleWindow;
	}
}

void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	std::cout << "YOffset: " << yoffset << std::endl;
	if (zoom + yoffset >= 1.0f && zoom + yoffset <= 2.0f)
	{
		zoom += yoffset;
		OrthographicView(RESOLUTION_X, RESOLUTION_Y);
	}
}

void OnReceivedOpponentData(float* data)
{
	neonShooter.OnReceivedOpponentData(data);
}

void OnReceivedBulletData(unsigned int id, unsigned int tagID, float* data)
{
	neonShooter.OnReceivedBulletData(id, tagID, data);
}

void InitializePlayerEnemyData()
{
	neonShooter.InitializePlayerEnemyData();
}

void RestartGame()
{
	neonShooter.Restart();
}

void ExitGameWorld()
{
	neonShooter.ExitGameWorld();
}

int main(void)
{
	//Stealth(0);
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{
		return -1;
	}
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(RESOLUTION_X, RESOLUTION_Y, "Multiplayer TicTacToe by Teena 0118856", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, onWindowResized);
	onWindowResized(window, RESOLUTION_X, RESOLUTION_Y);
	glfwSetKeyCallback(window, WindowKeyCallback);
	glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, 1);

	neonShooter = NeonShooter(window);
	neonShooter.Start();

	MyPhoton::getInstance().OnReceivedOpponentData = &OnReceivedOpponentData;
	MyPhoton::getInstance().OnReceivedBulletData = &OnReceivedBulletData;
	MyPhoton::getInstance().InitializePlayerEnemyData = &InitializePlayerEnemyData;
	MyPhoton::getInstance().RestartGame = &RestartGame;
	MyPhoton::getInstance().ExitGameWorld = &ExitGameWorld;
	
	MyPhoton::getInstance().gameData = &neonShooter.gameData;
	MyPhoton::getInstance().playerData = &neonShooter.playerObject.playerData;
	MyPhoton::getInstance().enemyData = &neonShooter.enemyObject.playerData;

	GUI gui = GUI(window, &neonShooter);

	srand(time(NULL));
	
	double prevTime = glfwGetTime();
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			break;
		}

		if (ShowConsoleWindow == true)
		{
			HWND hWnd = GetConsoleWindow();
			ShowWindow(hWnd, SW_SHOW);
		}
		else if(ShowConsoleWindow == false)
		{
			HWND hWnd = GetConsoleWindow();
			ShowWindow(hWnd, SW_HIDE);
		}
		double currentTime = glfwGetTime();
		double elapsedTime = currentTime - prevTime;

		Vector3 pos = neonShooter.playerObject.GetPos();
		int midWidth = RESOLUTION_X / 2;
		int midHeight = RESOLUTION_Y / 2;
		scrollX = pos.x - midWidth;
		scrollY = pos.y - midHeight;
		OrthographicView(RESOLUTION_X, RESOLUTION_Y);

		neonShooter.Update(elapsedTime);
		neonShooter.Draw();
		gui.DrawGameGUI();
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
		prevTime = currentTime;
	}
	glfwTerminate();
	return 0;
}