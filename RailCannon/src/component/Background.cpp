#include "Background.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

Background::Background()
	:m_Proj(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f)),
	m_FlipRotation(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0))),
	m_Model(glm::mat4(1.0f)), m_CoreRotation(0.0f), m_CoreRotationDelta(0.5f)
{
	Transform.Translation = glm::vec3(0.0f, 0.0f, 0.0f);
	Transform.Rotation = 0.0f;
	Transform.Scale = glm::vec3(32.0f, 18.0f, 0.0f);
	Transform.Flipped = false;

	float vertices[] = {
		// Position		Tex Coord	Index
		-0.5f, -0.5f,	0.0f, 0.0f,	// 0
		0.5f, -0.5f,	1.0f, 0.0f,	// 1
		0.5f,	0.5f,	1.0f, 1.0f,	// 2
		-0.5f,	0.5f,	0.0f, 1.0f	// 3
	};

	unsigned int indices[] = {
		0, 1, 2,	// First Triangle comprises of indices 0, 1 and 2 respectively counter clockwise
		2, 3, 0		// Second Triangle comprises of indices 2, 3 and 0 respectively counter clockwise
	};

	m_VAO = std::make_unique<VertexArray>();

	m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 4 * 4 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2); // Position
	layout.Push<float>(2); // Texture coordinates

	m_VAO->AddBuffer(*m_VertexBuffer, layout);

	m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

	m_Shader = std::make_unique<Shader>("res/shaders/Tank.shader");
	m_Shader->Bind();
	m_TextureReactor = std::make_unique<Texture>("res/textures/background/Reactor.png");
	m_TextureReactorCore = std::make_unique<Texture>("res/textures/background/ReactorCore.png");
	m_Shader->SetUniform1i("u_Texture", 0); // This value must match previous texture binding slot number
}

Background::~Background()
{
}

void Background::OnUpdate(float deltaTime)
{
	m_CoreRotation += m_CoreRotationDelta;
	if (m_CoreRotation >= 360.0f)
		m_CoreRotation = 0.0f;
}

void Background::OnRender()
{
	m_Renderer.EnableBlend();

	m_TextureReactor->Bind();

	glm::mat4 identity(1.0f);
	glm::mat4 rotation = glm::rotate(identity, glm::radians(Transform.Rotation), glm::vec3(0, 0, 1));
	glm::mat4 rotationCore = glm::rotate(identity, glm::radians(Transform.Rotation + m_CoreRotation), glm::vec3(0, 0, 1));
	glm::mat4 scale = glm::scale(identity, Transform.Scale);
	glm::mat4 translation = glm::translate(identity, Transform.Translation);

	m_Model = translation * rotation * scale;

	glm::mat4 mvp;
	if (Transform.Flipped)
		mvp = m_Proj * m_Model * m_FlipRotation;
	else
		mvp = m_Proj * m_Model;

	m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_MVP", mvp);

	m_Renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);

	// Calculation for core

	m_Model = translation * rotationCore * scale;

	if (Transform.Flipped)
		mvp = m_Proj * m_Model * m_FlipRotation;
	else
		mvp = m_Proj * m_Model;

	m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_MVP", mvp);
	m_TextureReactorCore->Bind();
	m_Renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
}

void Background::OnImGuiRender()
{
}
