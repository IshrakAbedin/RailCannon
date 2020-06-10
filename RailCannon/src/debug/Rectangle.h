#pragma once

#pragma once

#include <string>

#include "component/Drawable.h"
#include "component/Muzzle.h"
#include "component/Possessable.h"
#include "component/SimpleCollision2D.h"

class Rectangle : public Drawable, public ActiveCollider
{
public:
	Transformation Transform;
private:
	std::string m_Identifier;
	std::string m_TexturePath;

	glm::vec2 m_TopLeftPoint;
	glm::vec2 m_BottomRightPoint;

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<Texture> m_Texture;

	glm::mat4 m_Proj; // Camera is discarded
	glm::mat4 m_Model;
	glm::mat4 m_FlipRotation;

	Renderer m_Renderer;

public:
	Rectangle(bool flipped, std::string identifier, bool collisionOn);
	Rectangle(bool flipped, std::string identifier, bool collisionOn, std::string texturePath);
	~Rectangle();

	inline std::string GetIdentifier() const { return m_Identifier; }
	inline void SetIdentifier(const std::string& identifier) { m_Identifier = identifier; }

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnImGuiRender() override;

	BoundingRectangle GetBoundingRectangle() override;
};