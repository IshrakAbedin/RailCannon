// Remember to put the preprocessor for using static library instead of dll
#include <GL/glew.h> // Remember glew definition generally needs to be before other gl headers, the problem is double definition
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "stb_image/stb_image.h"

#include "scene/GameScene.h"
#include "rcutil/Saver.h"

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

GameScene* GScene;

int main(void)
{
	bool aboutWindowOpened = false;
	bool controlsWindowOpened = false;
	bool statsWindowOpened = false;
	bool settingsWindowOpened = false;
	
	int settingsIndex;
	bool settingsFullscreen;

	Saver saver("./save/settings.json");
	saver.Load();

	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	int width = saver.GetSavedWidth();
	int height = saver.GetSavedHeight();
	bool fullscreen = saver.GetFullscreen();
	settingsFullscreen = fullscreen;
	settingsIndex = saver.GetSavedIndex();

	window = glfwCreateWindow(width, height, "RailCannon", fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Assign icon
	GLFWimage icons[1];
	icons[0].pixels = stbi_load("./res/icon/RailCannonIcon.png", &icons[0].width, &icons[0].height, 0, 4); //rgba channels
	glfwSetWindowIcon(window, 1, icons);
	stbi_image_free(icons[0].pixels);

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

		glfwSetKeyCallback(window, KeyCallback);

		Renderer renderer;

		GScene = new GameScene();

		while (!glfwWindowShouldClose(window))
		{
			renderer.SetClearColorBlack();
			renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("FPS");
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

			if (GScene->GetStatus() == GameStatus::ONGOING) // Game has not ended
			{
				ImGui::Begin("Menu");
				if (ImGui::Button("Restart"))
					GScene->InitializeStats();
				ImGui::SameLine();
				if (ImGui::Button("Controls"))
				{
					controlsWindowOpened = controlsWindowOpened ? false : true;
				}
				ImGui::SameLine();
				if (ImGui::Button("About"))
				{
					aboutWindowOpened = aboutWindowOpened ? false : true;
				}
				ImGui::SameLine();
				if (ImGui::Button("Settings"))
				{
					settingsWindowOpened = settingsWindowOpened ? false : true;
				}
				ImGui::SameLine();
				if (ImGui::Button("Exit"))
					glfwSetWindowShouldClose(window, GLFW_TRUE);
				ImGui::End();

				if (aboutWindowOpened) // GUI Code
				{
					ImGui::Begin("About", &aboutWindowOpened);
					ImGui::Text("Built by Mohammad Ishrak Abedin.");
					ImGui::Text("Libraries used: ");

					ImGui::BeginChild("Libraries");
					ImGui::BulletText("GLFW 3.3.2");
					ImGui::BulletText("GLEW 2.1");
					ImGui::BulletText("GLM 0.9.9.8");
					ImGui::BulletText("Dear ImGui 1.76");
					ImGui::BulletText("stb_image");
					ImGui::BulletText("nlohmann::json");
					ImGui::EndChild();

					ImGui::End();
				}

				if (controlsWindowOpened) // GUI code
				{
					ImGui::Begin("Controls", &controlsWindowOpened);
					ImGui::Text("Controls: ");

					ImGui::BeginChild("Controls");
					ImGui::BulletText("Move Left: Left Arrow or A");
					ImGui::BulletText("Move Right: Right Arrow or D");
					ImGui::BulletText("Raise Muzzle: Up Arrow or W");
					ImGui::BulletText("Lower Muzzle: Down Arrow or S");
					ImGui::BulletText("Increase Power: Keypad 5 or E");
					ImGui::BulletText("Decrease Power: Keypad 4 or Q");
					ImGui::BulletText("Fire AP Projectile: Keypad 1 or 1 or Space (Unlimited)");
					ImGui::BulletText("Fire HE Projectile: Keypad 2 or 2 (Ammo is limited to 2)");
					ImGui::BulletText("Fire APHE Projectile: Keypad 3 or 3 (Ammo is limited to 1)");
					ImGui::BulletText("Maximum wind velocity is 0.2 at any direction");
					ImGui::EndChild();

					ImGui::End();
				}

				if (settingsWindowOpened) // GUI code
				{
					ImGui::SetNextWindowSizeConstraints(ImVec2(-1, -1), ImVec2(-1, -1));
					ImGui::Begin("Settings", &settingsWindowOpened);

					ImGui::SliderInt("Resolution", &settingsIndex, 0, 3);
					ImGui::Text("Selected Resolution: %d x %d", saver.GetWidthBasedOnIndex(settingsIndex), saver.GetHeightBasedOnIndex(settingsIndex));

					ImGui::Checkbox("Fullscreen", &settingsFullscreen);

					if (ImGui::Button("Save"))
						saver.Save(settingsIndex, settingsFullscreen);

					ImGui::Text("Saved Settings: %d x %d %s", 
						saver.GetSavedWidth(), saver.GetSavedHeight(), saver.GetFullscreen() ? "Fullscreen" : "Windowed");

					ImGui::TextColored(ImVec4(0.9f, 0.0f, 0.0f, 1.0f), "Restart is required to apply new settings.");
					ImGui::TextColored(ImVec4(0.9f, 0.0f, 0.0f, 1.0f), "Please reposition HUDs accordingly.");
					ImGui::TextColored(ImVec4(0.9f, 0.0f, 0.0f, 1.0f), "Some HUDs can be resized too.");
					ImGui::End();
				}

				GScene->OnUpdate(0.0f);
				GScene->OnRender();
				GScene->OnImGuiRender();
			}
			else // Game ended
			{
				ImGui::SetNextWindowSizeConstraints(ImVec2(-1, -1), ImVec2(-1, -1));
				ImGui::Begin("Conclusion");
				ImGui::TextColored(ImVec4(0.9f, 0.0f, 0.0f, 1.0f), "PLAYER %d IS VICTORIOUS", (int)GScene->GetStatus());
				if (ImGui::Button("Rematch"))
					GScene->InitializeStats();
				ImGui::SameLine();
				if (ImGui::Button("Stats"))
				{
					statsWindowOpened = statsWindowOpened ? false : true;
				}
				ImGui::SameLine();
				if (ImGui::Button("Exit"))
					glfwSetWindowShouldClose(window, GLFW_TRUE);
				ImGui::End();

				if (statsWindowOpened) // GUI code
				{
					ImGui::Begin("Status of the Game", &statsWindowOpened);
					ImGui::Text("Player ONE:");
					ImGui::BulletText("HP Remaining: %.0f", 100.0f * GScene->GetTankA()->GetHp());
					ImGui::BulletText("HE Remaining: %d", GScene->GetTankA()->GetMediumBallCount());
					ImGui::BulletText("APHE Remaining: %d", GScene->GetTankA()->GetHeavyBallCount());
					ImGui::Text("Player TWO:");
					ImGui::BulletText("HP Remaining: %.0f", 100.0f * GScene->GetTankB()->GetHp());
					ImGui::BulletText("HE Remaining: %d", GScene->GetTankB()->GetMediumBallCount());
					ImGui::BulletText("APHE Remaining: %d", GScene->GetTankB()->GetHeavyBallCount());
					ImGui::End();
				}

				GScene->OnUpdate(0.0f);
				GScene->OnRender();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		delete GScene;
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

void KeyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (GScene)
		GScene->KeyCallbackRedirect(key, scancode, action, mods);
}
