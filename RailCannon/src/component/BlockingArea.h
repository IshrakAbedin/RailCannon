#pragma once

#include <string>
#include <memory>

#include "Renderer.h"
#include "Drawable.h"
#include "SimpleCollision2D.h"

class BlockingArea : public Drawable, public PassiveCollider
{
public:
	Transformation Transform;
private:
	std::string m_Identifier;

	glm::vec2 m_TopLeftPoint;
	glm::vec2 m_BottomRightPoint;

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<Shader> m_Shader;

	glm::mat4 m_Proj; // Camera is discarded
	glm::mat4 m_Model;

	Renderer m_Renderer;

public:
	BlockingArea(std::string identifier);
	~BlockingArea();

	inline std::string GetIdentifier() const { return m_Identifier; }
	inline void SetIdentifier(const std::string& identifier) { m_Identifier = identifier; }

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnImGuiRender() override;

	BoundingRectangle GetBoundingRectangle() override;
};