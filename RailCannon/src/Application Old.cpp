// Remember to put the preprocessor for using static library instead of dll
#include <GL/glew.h> // Remember glew definition generally needs to be before other gl headers, the problem is double definition
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "component/Tank.h"
#include "component/Background.h"
#include "component/Wind.h"
#include "component/BlockingArea.h"

void SetTankAActive();
void SetTankBActive();
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

Tank* CurrentTank;
Tank* TankA;
Tank* TankB;
Wind* WindGen;

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1600, 900, "RailCannon", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error in glew" << std::endl;
	}
	else
	{
		std::cout << "Using OpenGL" << GetGLVersion() << std::endl;
	}

	{


		// ImGui declaration
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		const char* glsl_version = "#version 330";
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
		ImGui::StyleColorsLight();

		//test::Test* currentTest = nullptr;
		//test::TestMenu* testMenu = new test::TestMenu(currentTest);
		//currentTest = testMenu; // A command line argument can be setup so that the application boots up with a specific test

		//testMenu->RegisterTest<test::TestClearColor>("Clear color");
		//testMenu->RegisterTest<test::TestTexture2D>("2D Texture");

		glfwSetKeyCallback(window, KeyCallback);

		Renderer renderer;

		Background bg;

		BlockingArea B1("BA1");
		BlockingArea B2("BA2");
		BlockingArea B3("BA3");

		B1.Transform.Translation.y = -9.0f;
		B1.Transform.Scale.x = 32.0f;
		B1.Transform.Scale.y = 2.0f;

		B2.Transform.Translation.x = 16.0f;
		B2.Transform.Scale.x = 2.0f;
		B2.Transform.Scale.y = 36.0f;

		B3.Transform.Translation.x = -16.0f;
		B3.Transform.Scale.x = 2.0f;
		B3.Transform.Scale.y = 36.0f;

		WindGen = new Wind(0.2f);

		TankA = new Tank(1.0f, false, "Player 1", "res/textures/cannon/", WindGen);
		TankB = new Tank(1.0f, true, "Player 2", "res/textures/cannon/", WindGen);

		TankA->Transform.Scale = glm::vec3(2.0f);
		TankB->Transform.Scale = glm::vec3(2.0f);

		TankA->Transform.Translation.x = -12.0f;
		TankA->Transform.Translation.y = -3.4f;

		TankB->Transform.Translation.x = 12.0f;
		TankB->Transform.Translation.y = -3.4f;

		TankA->SetLeftBoundary(-13.76f);
		TankA->SetRightBoundary(-5.98f);

		TankB->SetLeftBoundary(5.98f);
		TankB->SetRightBoundary(13.76f);

		TankA->RegisterCannonBallCollidable(TankB);
		TankA->RegisterCannonBallCollidable(&B1);
		TankA->RegisterCannonBallCollidable(&B2);
		TankA->RegisterCannonBallCollidable(&B3);

		TankB->RegisterCannonBallCollidable(TankA);
		TankB->RegisterCannonBallCollidable(&B1);
		TankB->RegisterCannonBallCollidable(&B2);
		TankB->RegisterCannonBallCollidable(&B3);

		//TankA->OnPossessCallback = SetTankAActive;
		//TankB->OnPossessCallback = SetTankBActive;
		TankA->OnDepossessCallback = SetTankBActive;
		TankB->OnDepossessCallback = SetTankAActive;

		TankA->IsControllerOn = true;
		TankB->IsControllerOn = false;

		SetTankAActive();
		TankA->OnPossess();

		while (!glfwWindowShouldClose(window))
		{
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// Insert code here
			//if (currentTest)
			//{
			//	currentTest->OnUpdate(0.0f);
			//	currentTest->OnRender();
			//	ImGui::Begin("Test");
			//	if (currentTest != testMenu && ImGui::Button("<-")) // We can nest if it, but && is fine
			//	{
			//		delete currentTest; // Delete the current test
			//		currentTest = testMenu; // Set current test back to testMenu
			//	}
			//	currentTest->OnImGuiRender();
			//	ImGui::End();
			//}

			ImGui::Begin("FPS");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

			bg.OnUpdate(0.0f);
			bg.OnRender();

			WindGen->OnUpdate(0.0f);
			WindGen->OnRender();
			WindGen->OnImGuiRender();

			B1.OnUpdate(0.0f);
			B1.OnRender();

			B2.OnUpdate(0.0f);
			B2.OnRender();

			B3.OnUpdate(0.0f);
			B3.OnRender();

			TankA->OnUpdate(0.0f);
			TankA->OnRender();
			TankA->OnImGuiRender();

			TankB->OnUpdate(0.0f);
			TankB->OnRender();
			TankB->OnImGuiRender();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		delete TankA;
		delete TankB;
		delete WindGen;

		//delete currentTest; // Delete the current test on window close
		//if (currentTest != testMenu) // If we closed the window which inside any test, testMenu is still left, so we should clear that too
		//	delete testMenu;
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

void SetTankAActive()
{
	WindGen->ShuffleWindVelocity();
	CurrentTank = TankA;
	TankA->OnPossess();
}

void SetTankBActive()
{
	WindGen->ShuffleWindVelocity();
	CurrentTank = TankB;
	TankB->OnPossess();
}

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (CurrentTank)
		CurrentTank->KeyCallbackRedirect(key, scancode, action, mods);
}
