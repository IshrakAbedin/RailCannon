#pragma once

#include <memory>

#include "component/Drawable.h"
#include "VertexBufferLayout.h"
#include "Texture.h"


class Smoke : public Drawable
{
public:
	Transformation Transform;
private:
	Transformation & m_ParentTransform;
	std::string m_TextureDirectory;

	float& m_AlphaReference;

	float m_SmokeUp;
	float m_SmokeUpIncrement;
	float m_Smoke2Up;
	float m_Smoke2UpIncrement;

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<Texture> m_TextureSmoke1;
	std::unique_ptr<Texture> m_TextureSmoke2;
	std::unique_ptr<Texture> m_TextureSmokeAlphaMask;

	glm::mat4 m_Proj; // Camera is discarded
	glm::mat4 m_Model;
	glm::mat4 m_FlipRotation;

	Renderer m_Renderer;

public:
	Smoke(float& alphaReference, Transformation& parentTransform, std::string textureDirectory);
	~Smoke();

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnImGuiRender() override;
};
