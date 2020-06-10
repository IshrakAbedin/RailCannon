#include "Tank.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

#include <GLFW/glfw3.h>

Tank::Tank(bool flipped, std::string identifier)
	:Tank::Tank(flipped, identifier, "res/textures/debug/Default.png")
{
}

Tank::Tank(bool flipped, std::string identifier, std::string texturePath)
	:Possessable(),
	m_Identifier(identifier), m_TexturePath(texturePath), 
	m_Proj(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f)),
	m_RotationRate(0.5f), m_FlipRotation(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0))),
	m_Model(glm::mat4(1.0f)), m_TopLeftPoint(glm::vec2(-0.5f, 0.5f)), m_BottomRightPoint(glm::vec2(0.5f, -0.5f))
{
	Transform.Translation = glm::vec3(0.0f, 0.0f, 0.0f);
	Transform.Rotation = 0.0f;
	Transform.Scale = glm::vec3(1.0f, 1.0f, 0.0f);
	Transform.Flipped = flipped;

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
	m_Texture = std::make_unique<Texture>(m_TexturePath);
	m_Shader->SetUniform1i("u_Texture", 0); // This value must match previous texture binding slot number

	// Building Muzzle
	m_Muzzle = std::make_unique<Muzzle>(Transform);
}

Tank::~Tank()
{
}

void Tank::OnUpdate(float deltaTime)
{
	m_Muzzle->OnUpdate(deltaTime);
	
	if (LeftKeyDown) LeftKeyAction();
	else if (RightKeyDown) RightKeyAction();
	else if (UpKeyDown) UpKeyAction();
	else if (DownKeyDown) DownKeyAction();
	else if (FireKeyDown) FireKeyAction();
}

void Tank::OnRender()
{
	m_Renderer.EnableBlend();

	m_Texture->Bind();


	glm::mat4 identity(1.0f);
	glm::mat4 rotation = glm::rotate(identity, glm::radians(Transform.Rotation), glm::vec3(0, 0, 1));
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

	m_Muzzle->OnRender();
}

void Tank::OnImGuiRender()
{
	ImGui::Begin(m_Identifier.c_str());
	ImGui::SliderFloat3("Tank_Translate", glm::value_ptr(Transform.Translation), -20.0f, 20.0f);
	ImGui::SliderFloat("Tank_Rotate", &Transform.Rotation, -180.0f, 180.0f);
	ImGui::SliderFloat3("Tank_Scale", glm::value_ptr(Transform.Scale), 0.0f, 10.0f);
	ImGui::SliderFloat("Angle", &m_Muzzle->Transform.Rotation, m_Muzzle->GetRotationMin(), m_Muzzle->GetRotationMax());
	if (ImGui::Button("Raise"))
		RaiseMuzzle();
	ImGui::SameLine();
	if (ImGui::Button("Lower"))
		LowerMuzzle();
	ImGui::End();

	m_Muzzle->OnImGuiRender();
}

void Tank::RaiseMuzzle()
{
	m_Muzzle->IncrementRotation(m_RotationRate);
}

void Tank::LowerMuzzle()
{
	m_Muzzle->DecrementRotation(m_RotationRate);
}

BoundingRectangle Tank::GetBoundingRectangle()
{
	glm::vec4 tl(m_TopLeftPoint.x, m_TopLeftPoint.y, 0.0f, 1.0f);
	glm::vec4 br(m_BottomRightPoint.x, m_BottomRightPoint.y, 0.0f, 1.0f);

	tl = m_Model * tl;
	br = m_Model * br;
	return BoundingRectangle(glm::vec2(tl.x, tl.y), glm::vec2(br.x, br.y));
}

void Tank::OnPossess()
{
	FlushKeyPresses();
	IsPossessed = true;
	if (OnPossessCallback)
		OnPossessCallback();
}

void Tank::OnDepossess()
{
	FlushKeyPresses();
	IsPossessed = false;
	if (OnDepossessCallback)
		OnDepossessCallback();
}

void Tank::KeyCallbackRedirect(int key, int scancode, int action, int mods)
{
	if (IsControllerOn) 
	{
		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
			LeftKeyDown = true;
		else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
			RightKeyDown = true;
		else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
			UpKeyDown = true;
		else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
			DownKeyDown = true;
		else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
			FireKeyAction();
		else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
			LeftKeyDown = false;
		else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
			RightKeyDown = false;
		else if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
			UpKeyDown = false;
		else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
			DownKeyDown = false;

	}	
}

void Tank::LeftKeyAction()
{
	Transform.Translation.x -= 0.02f;
}

void Tank::RightKeyAction()
{
	Transform.Translation.x += 0.02f;
}

void Tank::UpKeyAction()
{
	RaiseMuzzle();
}

void Tank::DownKeyAction()
{
	LowerMuzzle();
}

void Tank::FireKeyAction()
{
	OnDepossess();
}

void Tank::FlushKeyPresses()
{
	LeftKeyDown = false;
	RightKeyDown = false;
	UpKeyDown = false;
	DownKeyDown = false;
	FireKeyDown = false;
}


