#pragma once

#include <memory>

#include "component/Drawable.h"
#include "VertexBufferLayout.h"
#include "Texture.h"


class Grid : public Drawable
{
public:
	Transformation Transform;
private:

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
	Grid();
	~Grid();

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnImGuiRender() override;
};
