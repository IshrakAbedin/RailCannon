#include "Tank.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

#include <GLFW/glfw3.h>

Tank::Tank(float hp, bool flipped, std::string identifier)
	:Tank::Tank(hp, flipped, identifier, "res/textures/cannon/")
{
}

Tank::Tank(float hp, bool flipped, std::string identifier, std::string textureDirectory)
	:Tank::Tank(hp, flipped, identifier, textureDirectory, nullptr)
{
}

Tank::Tank(float hp, bool flipped, std::string identifier, std::string textureDirectory, Wind* wind)
	: Possessable(),
	m_Hp(hp), m_LeftBoundary(-16.0f), m_RightBoundary(16.0f), m_Wind(wind),
	m_MovementRate(0.02f), m_VelocityMin(0.3f), m_VelocityMax(1.0f), m_VelocityDelta(0.01f),
	m_MediumBallCount(2), m_HeavyBallCount(1),
	m_Identifier(identifier), m_TextureDirectory(textureDirectory), 
	m_Proj(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f)),
	m_RotationRate(0.5f), m_Velocity(0.5f),
	m_FlipRotation(glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0, 1, 0))),
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
	m_Texture = std::make_unique<Texture>(m_TextureDirectory + "CannonBody.png");
	m_Shader->SetUniform1i("u_Texture", 0); // This value must match previous texture binding slot number

	// Building Muzzle
	m_Muzzle = std::make_unique<Muzzle>(Transform, m_TextureDirectory + "CannonMuzzle.png");

	m_CannonBall = std::make_unique<CannonBall>(Transform, true, m_TextureDirectory + "ball/");
	m_CannonBall->Transform.Scale = glm::vec3(0.15f);
	m_CannonBall->SetOnCollisionCallback( [&](){OnDepossess(); });

	m_Smoke = std::make_unique<Smoke>(m_Hp, Transform, m_TextureDirectory + "smoke/");
	m_Smoke->Transform.Translation.y = 0.708f;
}

Tank::~Tank()
{
}

void Tank::OnUpdate(float deltaTime)
{
	m_Smoke->OnUpdate(deltaTime);
	m_Muzzle->OnUpdate(deltaTime);
	m_CannonBall->OnUpdate(deltaTime);
	
	if (LeftKeyDown) LeftKeyAction();
	else if (RightKeyDown) RightKeyAction();
	else if (UpKeyDown) UpKeyAction();
	else if (DownKeyDown) DownKeyAction();
	else if (VelocityIncrementKeyDown) VelocityIncrementKeyAction();
	else if (VelocityDecrementKeyDown) VelocityDecrementKeyAction();
}

void Tank::OnRender()
{
	//m_Smoke->OnRender();
	m_CannonBall->OnRender();
	m_Muzzle->OnRender();

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

	// Render Smoke on top
	m_Smoke->OnRender();
}

void Tank::OnImGuiRender()
{
	m_Smoke->OnImGuiRender();
	m_Muzzle->OnImGuiRender();
	m_CannonBall->OnImGuiRender();



	ImGui::Begin(m_Identifier.c_str());
	//ImGui::SliderFloat3("Smoke_Translation", glm::value_ptr(m_Smoke->Transform.Translation), -10.0f, 10.f);
	ImGui::TextColored(ImVec4(1 - m_Hp, 0, m_Hp, 1), "Health:");
	//ImGui::SameLine();
	ImGui::ProgressBar(m_Hp);
	//ImGui::SliderFloat3("Tank_Translate", glm::value_ptr(Transform.Translation), -20.0f, 20.0f);
	//ImGui::SliderFloat("Tank_Rotate", &Transform.Rotation, -180.0f, 180.0f);
	//ImGui::SliderFloat3("Tank_Scale", glm::value_ptr(Transform.Scale), 0.0f, 10.0f);
	ImGui::Text("Ammo: AP[inf] HE[%d] APHE[%d]", m_MediumBallCount, m_HeavyBallCount);

	if(IsControllerOn)
	{
		ImGui::SliderFloat("Angle", &m_Muzzle->Transform.Rotation, m_Muzzle->GetRotationMin(), m_Muzzle->GetRotationMax());
		ImGui::SliderFloat("Power", &m_Velocity, m_VelocityMin, m_VelocityMax);

		/*if (ImGui::Button("Raise"))
			RaiseMuzzle();
		ImGui::SameLine();
		if (ImGui::Button("Lower"))
			LowerMuzzle();*/

		ImGui::Text("Fire:");
		ImGui::SameLine();
		if (ImGui::Button("AP"))
			FireLightBall();

		if (m_MediumBallCount > 0u)
		{
			ImGui::SameLine();
			if (ImGui::Button("HE"))
				FireMediumBall();
		}

		if (m_HeavyBallCount > 0u)
		{
			ImGui::SameLine();
			if (ImGui::Button("APHE"))
				FireHeavyBall();
		}
	}

	/*ImGui::SameLine();
	if (ImGui::Button("Stop"))
		m_CannonBall->ResetProjectile();*/

	ImGui::End();
}

void Tank::TakeDamage(float amount)
{
	m_Hp -= amount;
	if (m_Hp < 0.0f)
		m_Hp = 0.0f;
}

void Tank::RaiseMuzzle()
{
	m_Muzzle->IncrementRotation(m_RotationRate);
}

void Tank::LowerMuzzle()
{
	m_Muzzle->DecrementRotation(m_RotationRate);
}

void Tank::IncrementVelocity()
{
	m_Velocity += m_VelocityDelta;
	if (m_Velocity > m_VelocityMax)
		m_Velocity = m_VelocityMax;
}

void Tank::DecrementVelocity()
{
	m_Velocity -= m_VelocityDelta;
	if (m_Velocity < m_VelocityMin)
		m_Velocity = m_VelocityMin;
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
	IsControllerOn = true;
	if (OnPossessCallback)
		OnPossessCallback();
}

void Tank::OnDepossess()
{
	FlushKeyPresses();
	IsControllerOn = false;
	IsPossessed = false;
	if (OnDepossessCallback)
		OnDepossessCallback();
}

void Tank::KeyCallbackRedirect(int key, int scancode, int action, int mods)
{
	if (IsControllerOn) 
	{
		if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_A)  && action == GLFW_PRESS)
			LeftKeyDown = true;
		else if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && action == GLFW_PRESS)
			RightKeyDown = true;
		else if ((key == GLFW_KEY_UP || key == GLFW_KEY_W) && action == GLFW_PRESS)
			UpKeyDown = true;
		else if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_S) && action == GLFW_PRESS)
			DownKeyDown = true;
		else if ((key == GLFW_KEY_E || key == GLFW_KEY_KP_5) && action == GLFW_PRESS)
			VelocityIncrementKeyDown = true;
		else if ((key == GLFW_KEY_Q || key == GLFW_KEY_KP_4) && action == GLFW_PRESS)
			VelocityDecrementKeyDown = true;

		else if ((key == GLFW_KEY_SPACE || key == GLFW_KEY_1 || key == GLFW_KEY_KP_1) && action == GLFW_PRESS)
			FireLightBall();
		else if ((key == GLFW_KEY_2 || key == GLFW_KEY_KP_2) && action == GLFW_PRESS)
			FireMediumBall();
		else if ((key == GLFW_KEY_3 || key == GLFW_KEY_KP_3) && action == GLFW_PRESS)
			FireHeavyBall();

		else if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && action == GLFW_RELEASE)
			LeftKeyDown = false;
		else if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && action == GLFW_RELEASE)
			RightKeyDown = false;
		else if ((key == GLFW_KEY_UP || key == GLFW_KEY_W) && action == GLFW_RELEASE)
			UpKeyDown = false;
		else if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_S) && action == GLFW_RELEASE)
			DownKeyDown = false;
		else if ((key == GLFW_KEY_E || key == GLFW_KEY_KP_5) && action == GLFW_RELEASE)
			VelocityIncrementKeyDown = false;
		else if ((key == GLFW_KEY_Q || key == GLFW_KEY_KP_4) && action == GLFW_RELEASE)
			VelocityDecrementKeyDown = false;

	}	
}

void Tank::LeftKeyAction()
{
	Transform.Translation.x -= m_MovementRate;
	BoundTranslation();
}

void Tank::RightKeyAction()
{
	Transform.Translation.x += m_MovementRate;
	BoundTranslation();
}

void Tank::UpKeyAction()
{
	RaiseMuzzle();
}

void Tank::DownKeyAction()
{
	LowerMuzzle();
}

void Tank::VelocityIncrementKeyAction()
{
	IncrementVelocity();
}

void Tank::VelocityDecrementKeyAction()
{
	DecrementVelocity();
}

void Tank::FlushKeyPresses()
{
	LeftKeyDown = false;
	RightKeyDown = false;
	UpKeyDown = false;
	DownKeyDown = false;
	VelocityIncrementKeyDown = false;
	VelocityDecrementKeyDown = false;
}

void Tank::FireCannonBall(CannonBallType cannonBallType)
{
	m_CannonBall->SetCurrentType(cannonBallType);

	if(m_Wind)
		m_CannonBall->FireProjectile(m_Velocity + m_Wind->GetWindVelocity(Transform.Flipped), m_Muzzle->Transform.Rotation);
	else
		m_CannonBall->FireProjectile(m_Velocity, m_Muzzle->Transform.Rotation);
	
	IsControllerOn = false;
	//OnDepossess();
}

void Tank::FireLightBall()
{
	FireCannonBall(CannonBallType::LIGHT);
}

void Tank::FireMediumBall()
{
	if (m_MediumBallCount > 0u)
	{
		m_MediumBallCount--;
		FireCannonBall(CannonBallType::MEDIUM);
	}
}

void Tank::FireHeavyBall()
{
	if (m_HeavyBallCount > 0u)
	{
		m_HeavyBallCount--;
		FireCannonBall(CannonBallType::HEAVY);
	}
}

void Tank::BoundTranslation()
{
	if (Transform.Translation.x < m_LeftBoundary)
		Transform.Translation.x = m_LeftBoundary;
	else if (Transform.Translation.x > m_RightBoundary)
		Transform.Translation.x = m_RightBoundary;
}


