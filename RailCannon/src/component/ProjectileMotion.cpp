#include "ProjectileMotion.h"

#include "glm/glm.hpp"

ProjectileMotion::ProjectileMotion(float gravity, float deltaTime)
	:XOffset(0.0f), YOffset(0.0f), 
	m_Gravity(gravity), m_Time(0.0f), m_DeltaTime(deltaTime), m_Velocity(0.0f), m_Angle(0.0f),
	m_IsOnMotion(false)
{
}

ProjectileMotion::~ProjectileMotion()
{
}

void ProjectileMotion::FireProjectile(float velocity, float angle)
{
	ResetProjectile();

	m_Velocity = velocity;
	m_Angle = angle;
	m_IsOnMotion = true;
}

void ProjectileMotion::ResetProjectile()
{
	m_IsOnMotion = false;
	XOffset = 0.0f;
	YOffset = 0.0f;
	m_Time = 0.0f;
}

void ProjectileMotion::UpdateProjectile()
{
	if (m_IsOnMotion) 
	{
		m_Time += m_DeltaTime;
		XOffset = GetX();
		YOffset = GetY();
	}
}

float ProjectileMotion::GetX()
{
	return m_Velocity * m_Time * cos(glm::radians(m_Angle));
}

float ProjectileMotion::GetY()
{
	return m_Velocity * m_Time * sin(glm::radians(m_Angle)) - 0.5f * m_Gravity * m_Time * m_Time;
}
