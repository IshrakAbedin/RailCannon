#include "BlockingArea.h"

#include "VertexBufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

BlockingArea::BlockingArea(std::string identifier)
	:m_Identifier(identifier),
	m_Proj(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f)),
	m_Model(glm::mat4(1.0f)), m_TopLeftPoint(glm::vec2(-0.5f, 0.5f)), m_BottomRightPoint(glm::vec2(0.5f, -0.5f))
{
	Transform.Translation = glm::vec3(0.0f, 0.0f, 0.0f);
	Transform.Rotation = 0.0f;
	Transform.Scale = glm::vec3(1.0f, 1.0f, 0.0f);
	Transform.Flipped = false;

	float vertices[] = {
		// Position		Index
		-0.5f, -0.5f,	// 0
		0.5f, -0.5f,	// 1
		0.5f,	0.5f,	// 2
		-0.5f,	0.5f,	// 3
	};

	unsigned int indices[] = {
		0, 1, 2,	// First Triangle comprises of indices 0, 1 and 2 respectively counter clockwise
		2, 3, 0		// Second Triangle comprises of indices 2, 3 and 0 respectively counter clockwise
	};

	m_VAO = std::make_unique<VertexArray>();

	m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 4 * 4 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2); // Position

	m_VAO->AddBuffer(*m_VertexBuffer, layout);

	m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

	m_Shader = std::make_unique<Shader>("res/shaders/FlatColor.shader");
	m_Shader->Bind();
	m_Shader->SetUniform4f("u_Color", 0.0f, 0.0f, 0.0f, 1.0f); // This value must match previous texture binding slot number
}

BlockingArea::~BlockingArea()
{
}

void BlockingArea::OnUpdate(float deltaTime)
{
}

void BlockingArea::OnRender()
{
	m_Renderer.EnableBlend();

	glm::mat4 identity(1.0f);
	glm::mat4 rotation = glm::rotate(identity, glm::radians(Transform.Rotation), glm::vec3(0, 0, 1));
	glm::mat4 scale = glm::scale(identity, Transform.Scale);
	glm::mat4 translation = glm::translate(identity, Transform.Translation);

	m_Model = translation * rotation * scale;

	glm::mat4 mvp = m_Proj * m_Model;

	m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_MVP", mvp);

	m_Renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
}

void BlockingArea::OnImGuiRender()
{
}

BoundingRectangle BlockingArea::GetBoundingRectangle()
{
	glm::vec4 tl(m_TopLeftPoint.x, m_TopLeftPoint.y, 0.0f, 1.0f);
	glm::vec4 br(m_BottomRightPoint.x, m_BottomRightPoint.y, 0.0f, 1.0f);

	tl = m_Model * tl;
	br = m_Model * br;
	return BoundingRectangle(glm::vec2(tl.x, tl.y), glm::vec2(br.x, br.y));
}
