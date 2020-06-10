#include "Muzzle.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

Muzzle::Muzzle(Transformation& parentTransform)
	: m_ParentTransform(parentTransform),
	m_Proj(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f)),
	m_FlipRotation(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0))),
	m_RotationMin(0.0f), m_RotationMax(90.0f)
{
	Transform.Translation = glm::vec3(0.0f, 0.0f, 0.0f);
	Transform.Rotation = 0.0f;
	Transform.Scale = glm::vec3(1.0f, 1.0f, 0.0f);
	Transform.Flipped = parentTransform.Flipped;

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
	m_Texture = std::make_unique<Texture>("res/textures/debug/Axes.png");
	m_Shader->SetUniform1i("u_Texture", 0); // This value must match previous texture binding slot number
}

Muzzle::~Muzzle()
{
}

void Muzzle::OnUpdate(float deltaTime)
{
}

void Muzzle::OnRender()
{
	m_Renderer.EnableBlend();

	m_Texture->Bind();

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

	m_Renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);

}

void Muzzle::OnImGuiRender()
{
	/*ImGui::Begin("MUZZLE DEBUG");
	ImGui::SliderFloat3("Muz_Translate", glm::value_ptr(Transform.Translation), -20.0f, 20.0f);
	ImGui::SliderFloat("Muz_Rotate", &Transform.Rotation, -180.0f, 180.0f);
	ImGui::SliderFloat3("Muz_Transform.Scale", glm::value_ptr(Transform.Scale), 0.0f, 10.0f);
	if (ImGui::ColorButton("Muz_Up", ImVec4(0, 1, 0, 1)))
		IncrementRotation(1.0f);
	ImGui::SameLine();
	if (ImGui::ColorButton("Muz_Down", ImVec4(1, 0, 0, 1)))
		DecrementRotation(1.0f);
	ImGui::End();*/
}

void Muzzle::IncrementRotation(float amount)
{
	Transform.Rotation += amount;
	if (Transform.Rotation > m_RotationMax)
		Transform.Rotation = m_RotationMax;
}

void Muzzle::DecrementRotation(float amount)
{
	Transform.Rotation -= amount;
	if (Transform.Rotation < m_RotationMin)
		Transform.Rotation = m_RotationMin;
}
