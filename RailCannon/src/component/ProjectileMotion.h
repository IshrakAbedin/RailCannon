#pragma once

class ProjectileMotion
{
public:
	float XOffset, YOffset;

private:
	float m_Gravity, m_Time, m_DeltaTime, m_Velocity, m_Angle;
	bool m_IsOnMotion;

public:
	ProjectileMotion(float gravity, float deltaTime);
	~ProjectileMotion();

	inline float GetGravity() const { return m_Gravity; }
	inline void SetGravity(const float gravity) { m_Gravity = gravity; }

	inline float GetDeltaTime() const { return m_DeltaTime; }
	inline void SetDeltaTime(const float deltaTime) { m_DeltaTime = deltaTime; }

	inline bool GetIsOnMotion() const { return m_IsOnMotion; }

	void FireProjectile(float velocity, float angle);
	void ResetProjectile();
	void UpdateProjectile(); // Must be run on tick to recalculate offsets

private:
	float GetX();
	float GetY();
};