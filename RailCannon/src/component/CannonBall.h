#pragma once

#include <string>
#include <memory>
#include <functional>

#include "Renderer.h"
#include "Texture.h"

#include "Drawable.h"
#include "ProjectileMotion.h"
#include "SimpleCollision2D.h"

#include "Exploder.h"

class CannonBall : public Drawable, public ProjectileMotion, public ActiveCollider
{
public:
	Transformation Transform;
private:
	Transformation & m_ParentTransform;
	std::string m_TexturePath;

	glm::vec2 m_TopLeftPoint;
	glm::vec2 m_BottomRightPoint;

	std::unique_ptr<Exploder> m_Exploder;

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<Texture> m_Texture;

	glm::mat4 m_Proj; // Camera is discarded
	glm::mat4 m_Model;
	glm::mat4 m_FlipRotation;

	Renderer m_Renderer;

	float m_Velocity;
	float m_Angle;
	int m_FlipMultiplier;

	std::function<void()> m_OnCollisionCallback;

public:
	CannonBall(Transformation& parentTransform, bool collisionOn);
	CannonBall(Transformation& parentTransform, bool collisionOn, std::string texturePath);
	~CannonBall();

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnImGuiRender() override;

	inline void SetOnCollisionCallback(const std::function<void()>& callbackMethod) { m_OnCollisionCallback = callbackMethod;}

	BoundingRectangle GetBoundingRectangle() override;
};