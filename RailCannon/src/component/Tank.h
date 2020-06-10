#pragma once

#include <string>

#include "Drawable.h"
#include "Muzzle.h"
#include "CannonBall.h"
#include "Possessable.h"
#include "SimpleCollision2D.h"
#include "Damageable.h"

class Tank : public Drawable, public Possessable, public PassiveCollider, public Damageable
{
public:
	Transformation Transform;
private:
	float m_Hp;
	std::string m_Identifier;
	std::string m_TexturePath;

	glm::vec2 m_TopLeftPoint;
	glm::vec2 m_BottomRightPoint;

	std::unique_ptr<Muzzle> m_Muzzle;
	std::unique_ptr<CannonBall> m_CannonBall;

	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<Texture> m_Texture;

	glm::mat4 m_Proj; // Camera is discarded
	glm::mat4 m_Model;
	glm::mat4 m_FlipRotation;

	float m_RotationRate;
	float m_Velocity;

	Renderer m_Renderer;

public:
	Tank(float hp, bool flipped, std::string identifier);
	Tank(float hp, bool flipped, std::string identifier, std::string texturePath);
	~Tank();

	inline std::string GetIdentifier() const { return m_Identifier; }
	inline void SetIdentifier(const std::string& identifier) { m_Identifier = identifier; }

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnImGuiRender() override;

	void TakeDamage(float amount);


	void RaiseMuzzle();
	void LowerMuzzle();

	BoundingRectangle GetBoundingRectangle() override;

	inline void RegisterCannonBallCollidable(PassiveCollider* Collider) { m_CannonBall->RegisterCollidable(Collider); }

	// Controller methods
	void OnPossess();
	void OnDepossess();

	void KeyCallbackRedirect(int key, int scancode, int action, int mods);

private:
	bool LeftKeyDown = false;
	bool RightKeyDown = false;
	bool UpKeyDown = false;
	bool DownKeyDown = false;
	bool FireKeyDown = false;

	void LeftKeyAction();
	void RightKeyAction();
	void UpKeyAction();
	void DownKeyAction();
	void FireKeyAction();

	void FlushKeyPresses();
};