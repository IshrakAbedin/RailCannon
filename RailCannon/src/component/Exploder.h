#pragma once

#include <memory>

#include "component/Drawable.h"
#include "VertexBufferLayout.h"
#include "Texture.h"


class Exploder : public Drawable
{
private:
	bool m_IsExploding;
	float m_BaseScaleFactor;
	float m_AddedScaleFactor;
	float m_AddedScaleFactorIncrement;
	float m_TextureAlpha;
	float m_TextureAlphaDecrement;
	unsigned int m_TickRemaining;

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<Texture> m_Texture;


	glm::mat4 m_Proj; // Camera is discarded
	glm::mat4 m_Model;

	Renderer m_Renderer;

public:
	Exploder();
	~Exploder();

	inline bool GetIsExploding() const { return m_IsExploding; }

	inline float GetBaseScaleFactor() const { return m_BaseScaleFactor; }
	inline void SetBaseScaleFactor(const float scaleFactor) { m_BaseScaleFactor = scaleFactor; }

	inline float GetAddedScaleFactorIncrement() const { return m_AddedScaleFactorIncrement; }
	inline void SetAddedScaleFactorIncrement(const float scaleFactorIncrement) { m_AddedScaleFactorIncrement = scaleFactorIncrement; }

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnImGuiRender() override;

	void ExplodeAtFor(glm::mat4 model, const int tickCount);
	void ResetExplosion();
};