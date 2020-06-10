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

#include "test/TestClearColor.h"
#include "test/TestTexture2D.h"

#include "debug/Rectangle.h"
#include "debug/Grid.h"
#include "debug/Ball.h"

void SetTankAActive();
void SetTankBActive();
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1600, 900, "Debug World", NULL, NULL);
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

		//glfwSetKeyCallback(window, KeyCallback);

		Renderer renderer;
		/*TankA = new Tank(false, "TankA");
		TankB = new Tank(true, "TankB");*/

		Grid BackGrid;

		Ball B1(true, "Ball 1", true, "res/textures/debug/Ball.png");

		Rectangle R1(false, "Rec1", true, "res/textures/debug/Num1.png");
		Rectangle R2(false, "Rec2", true, "res/textures/debug/Num2.png");
		Rectangle R3(false, "Rec3", true, "res/textures/debug/Num3.png");
		Rectangle R4(false, "Rec4", false, "res/textures/debug/Num4.png");

		B1.RegisterCollidable(&R1);

		R1.RegisterCollidable(&R2);
		R1.RegisterCollidable(&R3);
		R1.RegisterCollidable(&R4);

		R2.RegisterCollidable(&R1);
		R2.RegisterCollidable(&R3);
		R2.RegisterCollidable(&R4);

		R3.RegisterCollidable(&R1);
		R3.RegisterCollidable(&R2);
		R3.RegisterCollidable(&R4);
	
		R4.RegisterCollidable(&R1);
		R4.RegisterCollidable(&R2);
		R4.RegisterCollidable(&R3);

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

			// ToDo
			BackGrid.OnRender();

			B1.OnUpdate(0.0f);
			B1.OnRender();
			B1.OnImGuiRender();

			R1.OnUpdate(0.0f);
			R1.OnRender();
			R1.OnImGuiRender();
			R2.OnUpdate(0.0f);
			R2.OnRender();
			R2.OnImGuiRender();
			R3.OnUpdate(0.0f);
			R3.OnRender();
			R3.OnImGuiRender();
			R4.OnUpdate(0.0f);
			R4.OnRender();
			R4.OnImGuiRender();
			

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


			glfwSwapBuffers(window);
			glfwPollEvents();
		}

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

//void SetTankAActive()
//{
//	CurrentTank = TankA;
//}
//
//void SetTankBActive()
//{
//	CurrentTank = TankB;
//}
//
//void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
//{
//	if (CurrentTank)
//		CurrentTank->KeyCallbackRedirect(key, scancode, action, mods);
//}
