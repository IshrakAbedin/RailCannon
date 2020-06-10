#include "Rectangle.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"

Rectangle::Rectangle(bool flipped, std::string identifier, bool collisionOn)
	:Rectangle::Rectangle(flipped, identifier, collisionOn, "res/textures/debug/Default.png")
{
}

Rectangle::Rectangle(bool flipped, std::string identifier, bool collisionOn, std::string texturePath)
	:ActiveCollider(collisionOn) ,m_Identifier(identifier), m_TexturePath(texturePath),
	m_Proj(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f)),
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
	m_Texture = std::make_unique<Texture>(m_TexturePath);
	m_Shader->SetUniform1i("u_Texture", 0); // This value must match previous texture binding slot number
}

Rectangle::~Rectangle()
{
}

void Rectangle::OnUpdate(float deltaTime)
{
}

void Rectangle::OnRender()
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
}

void Rectangle::OnImGuiRender()
{
	std::string boundInfo = "Top: " + std::to_string(GetBoundingRectangle().TopLeftPoint.x) + ", " + std::to_string(GetBoundingRectangle().TopLeftPoint.y) +
		" Bottom: " + std::to_string(GetBoundingRectangle().BottomRightPoint.x) + ", " + std::to_string(GetBoundingRectangle().BottomRightPoint.y);

	ImGui::Begin(m_Identifier.c_str());
	ImGui::SliderFloat3("Rec_Translate", glm::value_ptr(Transform.Translation), -20.0f, 20.0f);
	ImGui::SliderFloat("Rec_Rotate", &Transform.Rotation, -180.0f, 180.0f);
	ImGui::SliderFloat3("Rec_Scale", glm::value_ptr(Transform.Scale), 0.0f, 10.0f);
	ImGui::Text(boundInfo.c_str());
	auto collidedObjectPtrs = CheckCollision();
	if (!collidedObjectPtrs.empty())
	{
		ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), "Collision Detected with: ");
		for (auto* objectPtr : collidedObjectPtrs)
		{
			Rectangle* r = dynamic_cast<Rectangle*>(objectPtr);
			if (r)
				ImGui::TextColored(ImVec4(0.2f, 0.3f, 0.75f, 1.0f), r->GetIdentifier().c_str());
		}
	}
	ImGui::End();
}

BoundingRectangle Rectangle::GetBoundingRectangle()
{
	glm::vec4 tl(m_TopLeftPoint.x, m_TopLeftPoint.y, 0.0f, 1.0f);
	glm::vec4 br(m_BottomRightPoint.x, m_BottomRightPoint.y, 0.0f, 1.0f);

	tl = m_Model * tl;
	br = m_Model * br;
	return BoundingRectangle(glm::vec2(tl.x, tl.y), glm::vec2(br.x, br.y));
}
