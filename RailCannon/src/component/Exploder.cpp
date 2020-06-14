#include "Exploder.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

Exploder::Exploder()
	:m_IsExploding(false), m_BaseScaleFactor(2.0f), m_AddedScaleFactor(0.0f), 
	m_AddedScaleFactorIncrement(1.0f), m_TickRemaining(0),
	m_TextureAlpha(1.0f), m_TextureAlphaDecrement(0.0f),
	m_Proj(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f)),
	m_Model(glm::mat4(1.0f))
{
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

	m_Shader = std::make_unique<Shader>("res/shaders/Explosion.shader");
	m_Shader->Bind();
	m_Texture = std::make_unique<Texture>("res/textures/explosion/Explosion.png");
	m_Shader->SetUniform1i("u_Texture", 0); // This value must match previous texture binding slot number
}

Exploder::~Exploder()
{
}

void Exploder::OnUpdate(float deltaTime)
{
	if (m_IsExploding) 
	{
		if (m_TickRemaining == 0)
			ResetExplosion();
		else
		{
			m_TickRemaining--;
			m_AddedScaleFactor += m_AddedScaleFactorIncrement;
			m_TextureAlpha -= m_TextureAlphaDecrement;
		}
	}
}

void Exploder::OnRender()
{
	if (m_IsExploding) 
	{
		m_Renderer.EnableBlend();

		m_Texture->Bind();
		
		glm::mat4 identity(1.0f);
		glm::mat4 scale = glm::scale(identity, glm::vec3(m_BaseScaleFactor + m_AddedScaleFactor));
		
		glm::mat4 mvp = m_Proj * m_Model * scale;

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);
		m_Shader->SetUniform1f("u_Alpha", m_TextureAlpha);

		m_Renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}
}

void Exploder::OnImGuiRender()
{
}

void Exploder::ExplodeAtFor(glm::mat4 model, const int tickCount)
{
	m_IsExploding = true;
	m_Model = model;
	m_TickRemaining = tickCount;
	m_TextureAlphaDecrement = 1.0f / (float)tickCount;
}

void Exploder::ResetExplosion()
{
	m_IsExploding = false;
	m_TickRemaining = 0;
	m_AddedScaleFactor = 0.0f;
	m_TextureAlpha = 1.0f;
	m_TextureAlphaDecrement = 0.0f;
}
