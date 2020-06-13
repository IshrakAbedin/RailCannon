#include "Smoke.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

Smoke::Smoke(float & alphaReference, Transformation& parentTransform, std::string textureDirectory)
	:m_AlphaReference(alphaReference), m_ParentTransform(parentTransform), m_TextureDirectory(textureDirectory),
	m_SmokeUp(0.0f), m_Smoke2Up(0.0f),
	m_SmokeUpIncrement(0.0065f), m_Smoke2UpIncrement(0.001f),
	m_Proj(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f)),
	m_FlipRotation(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0))),
	m_Model(glm::mat4(1.0f))
{
	Transform.Translation = glm::vec3(0.0f, 0.0f, 0.0f);
	Transform.Rotation = 0.0f;
	Transform.Scale = glm::vec3(1.0f, 1.0f, 0.0f);
	Transform.Flipped = parentTransform.Flipped;

	float vertices[] = {
		//   Position			 Tex Coord		Index
		-0.5f,		-0.75f,		0.0f, 0.0f,		//0
		 0.5f,		-0.75f,		1.0f, 0.0f,		//1
		 0.425f,	 0.1f,		1.0f, 0.5f,		//2
	    -0.425f,	 0.1f,		0.0f, 0.5f,		//3
		 0.4f,		 0.75f,		1.0f, 1.0f,		//4
		-0.4f,		 0.75f,		0.0f, 1.0f		//5

	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,
		3, 2, 4,
		4, 5, 3
	};

	m_VAO = std::make_unique<VertexArray>();

	m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, 6 * 4 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2); // Position
	layout.Push<float>(2); // Texture coordinates

	m_VAO->AddBuffer(*m_VertexBuffer, layout);

	m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 12);

	m_Shader = std::make_unique<Shader>("res/shaders/Smoke.shader");
	m_Shader->Bind();
	m_TextureSmoke1 = std::make_unique<Texture>(m_TextureDirectory + "Smoke1.png");
	m_TextureSmoke2 = std::make_unique<Texture>(m_TextureDirectory + "Smoke2.png");
	m_TextureSmokeAlphaMask = std::make_unique<Texture>(m_TextureDirectory + "SmokeAlphaMask.png");
	m_Shader->SetUniform1i("u_TextureSmoke1", 0);
	m_Shader->SetUniform1i("u_TextureSmoke2", 1);
	m_Shader->SetUniform1i("u_TextureSmokeAlphaMask", 2);

	/*u_TextureSmoke1;
	u_TextureSmokeAlphaMask;
	u_TextureSmoke2;*/

	/*Shader shader("res/shaders/Basic.shader");
	shader.Bind();
	shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
	shader.SetUniformMat4f("u_MVP", mvp);


	Texture textureFire("res/textures/Flame.png");
	Texture textureFireAlpha("res/textures/FlameAlphaMask.png");
	Texture textureFireSpark("res/textures/Spark.png");
	textureFire.Bind(0);
	textureFireAlpha.Bind(1);
	textureFireSpark.Bind(2);
	shader.SetUniform1i("u_TextureFire", 0);
	shader.SetUniform1i("u_TextureFireAlpha", 1);
	shader.SetUniform1i("u_TextureFireSpark", 2);*/
}

Smoke::~Smoke()
{
}

void Smoke::OnUpdate(float deltaTime)
{
	// Smoke animation logic
	if (m_SmokeUp < -1.0f)
		m_SmokeUp = 0;
	else
		m_SmokeUp -= m_SmokeUpIncrement;

	if (m_Smoke2Up < -1.0f)
		m_Smoke2Up = 0;
	else
		m_Smoke2Up -= m_Smoke2UpIncrement;
}

void Smoke::OnRender()
{
	m_Renderer.EnableBlend();

	m_TextureSmoke1->Bind(0);
	m_TextureSmoke2->Bind(1);
	m_TextureSmokeAlphaMask->Bind(2);

	glm::mat4 identity(1.0f);
	glm::mat4 rotation = glm::rotate(identity, glm::radians(Transform.Rotation + m_ParentTransform.Rotation), glm::vec3(0, 0, 1));
	glm::mat4 scale = glm::scale(identity, Transform.Scale * m_ParentTransform.Scale);
	glm::mat4 translation = glm::translate(identity, Transform.Translation + m_ParentTransform.Translation);

	glm::mat4 mvp;
	if (m_ParentTransform.Flipped)
		mvp = m_Proj * translation * m_FlipRotation * rotation * scale;
	else
		mvp = m_Proj * translation * rotation * scale;

	m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_MVP", mvp);
	m_Shader->SetUniform2f("u_CoordAdd1", 0.0f, m_SmokeUp);
	m_Shader->SetUniform2f("u_CoordAdd2", 0.0f, m_Smoke2Up);
	m_Shader->SetUniform1f("u_Alpha", 1.0f - m_AlphaReference);

	m_Renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
}

void Smoke::OnImGuiRender()
{
}
