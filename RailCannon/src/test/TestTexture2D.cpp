#include "TestTexture2D.h"

#include "Renderer.h"

#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace test {

	TestTexture2D::TestTexture2D()
		: m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), 
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_TranslationA(200, 200, 0), m_TranslationB(400, 200, 0)
	{
		float positions[] = {
			// Position		Tex Coord	Index
			-50.0f, -50.0f,	0.0f, 0.0f,	// 0
			50.0f, -50.0f,	1.0f, 0.0f,	// 1
			50.0f,	 50.0f,	1.0f, 1.0f,	// 2
			-50.0f,	 50.0f,	0.0f, 1.0f	// 3
		};

		unsigned int indices[] = {
			0, 1, 2,	// First Triangle comprises of indices 0, 1 and 2 respectively counter clockwise
			2, 3, 0		// Second Triangle comprises of indices 2, 3 and 0 respectively counter clockwise
		};

		GLCall(glEnable(GL_BLEND)); // Enable blend
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // Set up blend function

		m_VAO = std::make_unique<VertexArray>();

		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2); // Position
		layout.Push<float>(2); // Texture coordinates

		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		m_Texture = std::make_unique<Texture>("res/textures/SampleA.png");
		m_Shader->SetUniform1i("u_Texture", 0); // This value must match previous texture binding slot number
	}

	TestTexture2D::~TestTexture2D()
	{

	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{

	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		m_Texture->Bind();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model;
			m_Shader->Bind(); // Redundant, expensive GL call wise but safe here. In actual game engine, there is some kind of caching system that knows what is already bound and does not dispatch the call again to bind.
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", glm::value_ptr(m_TranslationA), 0.0f, 960.0f); // instead of value ptr, &translation.x or r etc can also be used
		ImGui::SliderFloat3("Translation B", glm::value_ptr(m_TranslationB), 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

}
