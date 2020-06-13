#pragma once

#include <memory>

#include "component/Drawable.h"
#include "VertexBufferLayout.h"
#include "Texture.h"


class Wind : public Drawable
{
public:
	Transformation Transform;
private:
	float m_WindVelocity; // Positive is right
	float m_WindMaxVelocity; // Bidirectional, so range is [-m_WindMaxVelocity, m_WindMaxVelocity]

	float m_TextureAlpha;
	float m_TextureScrollAddition;
	float m_TextureScrollIncrement;
	float m_MaxTextureScrollIncrement; 

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
	Wind(const float maxVelocity);
	~Wind();

	inline float GetWindVelocity(bool reverse) const { return reverse ? (-1 * m_WindVelocity) : m_WindVelocity; }

	inline float GetWindMaxVelocity() const { return m_WindMaxVelocity; }
	inline void SetWindMaxVelocity(const float maxVelocity) { m_WindMaxVelocity = maxVelocity; }

	void ShuffleWindVelocity();

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnImGuiRender() override;
};
