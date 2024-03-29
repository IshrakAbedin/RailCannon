#pragma once

#include <string>

#include "Drawable.h"
#include "Possessable.h"
#include "SimpleCollision2D.h"
#include "Damageable.h"
#include "Muzzle.h"
#include "CannonBall.h"
#include "Smoke.h"
#include "Wind.h"

class Tank : public Drawable, public Possessable, public PassiveCollider, public Damageable
{
public:
	Transformation Transform;
private:
	// Gameplay Components
	float m_Hp;
	float m_LeftBoundary;
	float m_RightBoundary;
	float m_MovementRate;
	float m_RotationRate;
	float m_Velocity;
	float m_VelocityMin;
	float m_VelocityMax;
	float m_VelocityDelta;

	unsigned int m_MediumBallCount;
	unsigned int m_HeavyBallCount;

	std::string m_Identifier;
	std::string m_TextureDirectory;

	glm::vec2 m_TopLeftPoint;
	glm::vec2 m_BottomRightPoint;

	std::unique_ptr<Muzzle> m_Muzzle;
	std::unique_ptr<CannonBall> m_CannonBall;
	std::unique_ptr<Smoke> m_Smoke;

	Wind* m_Wind;

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
	Tank(float hp, bool flipped, std::string identifier);
	Tank(float hp, bool flipped, std::string identifier, std::string textureDirectory);
	Tank(float hp, bool flipped, std::string identifier, std::string textureDirectory, Wind* wind);
	~Tank();

	inline float GetHp() const { return m_Hp; }
	inline void SetHp(const float hp) { m_Hp = hp; }

	inline float GetLeftBoundary() const { return m_LeftBoundary; }
	inline void SetLeftBoundary(const float leftBoundary) { m_LeftBoundary = leftBoundary; }

	inline float GetRightBoundary() const { return m_RightBoundary; }
	inline void SetRightBoundary(const float rightBoundary) { m_RightBoundary = rightBoundary; }

	inline float GetMovementRate() const { return m_MovementRate; }
	inline void SetMovementRate(const float movementRate) { m_MovementRate = movementRate; }

	inline float GetVelocity() const { return m_Velocity; }
	inline void SetVelocity(const float velocity) { m_Velocity = velocity; }

	inline float GetMuzzleRotation() const { return m_Muzzle->Transform.Rotation; }
	inline void SetMuzzleRotationToMin() { m_Muzzle->SetRotationToMin(); }
	inline void SetMuzzleRotationToMax() { m_Muzzle->SetRotationToMax(); }

	inline unsigned int GetMediumBallCount() const { return m_MediumBallCount; }
	inline void SetMediumBallCount(const unsigned int mediumBallCount) { m_MediumBallCount = mediumBallCount; }

	inline unsigned int GetHeavyBallCount() const { return m_HeavyBallCount; }
	inline void SetHeavyBallCount(const unsigned int heavyBallCount) { m_HeavyBallCount = heavyBallCount; }

	inline std::string GetIdentifier() const { return m_Identifier; }
	inline void SetIdentifier(const std::string& identifier) { m_Identifier = identifier; }

	inline void SetWind(Wind* wind) { m_Wind = wind; }

	inline void ResetCannonBall() { m_CannonBall->ResetProjectile(); }

	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnImGuiRender() override;

	void TakeDamage(float amount);


	void RaiseMuzzle();
	void LowerMuzzle();
	void IncrementVelocity();
	void DecrementVelocity();

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
	bool VelocityIncrementKeyDown = false;
	bool VelocityDecrementKeyDown = false;

	void LeftKeyAction();
	void RightKeyAction();
	void UpKeyAction();
	void DownKeyAction();
	void VelocityIncrementKeyAction();
	void VelocityDecrementKeyAction();

	void FlushKeyPresses();

	void FireCannonBall(CannonBallType cannonBallType);
	void FireLightBall();
	void FireMediumBall();
	void FireHeavyBall();

	void BoundTranslation();
};