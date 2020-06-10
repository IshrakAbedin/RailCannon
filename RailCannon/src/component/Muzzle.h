#pragma once

#include <memory>

#include "Drawable.h"
#include "VertexBufferLayout.h"
#include "Texture.h"


class Muzzle : public Drawable
{
public:
	Transformation Transform;
private:
	Transformation& m_ParentTransform;

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<Texture> m_Texture;

	glm::mat4 m_Proj; // Camera is discarded
	glm::mat4 m_FlipRotation;

	float m_RotationMin, m_RotationMax;

	Renderer m_Renderer;

public:
	Muzzle(Transformation& parentTransform);
	~Muzzle();

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnImGuiRender() override;

	inline float GetRotationMin() const { return m_RotationMin; }
	inline void SetRotationMin(float rotationMin) { m_RotationMin = rotationMin; }

	inline float GetRotationMax() const { return m_RotationMax; }
	inline void SetRotationMax(float rotationMax) { m_RotationMax = rotationMax; }

	void IncrementRotation(float amount);
	void DecrementRotation(float amount);
};