#pragma once

#include "glm/glm.hpp"

#include <vector>

struct BoundingRectangle
{
	glm::vec2 TopLeftPoint;
	glm::vec2 BottomRightPoint;

	BoundingRectangle() 
		: TopLeftPoint(glm::vec2(0.0f)), BottomRightPoint(glm::vec2(0.0f)) {}

	BoundingRectangle(glm::vec2 topLeftPoint, glm::vec2 bottomRightPoint) 
		: TopLeftPoint(topLeftPoint), BottomRightPoint(bottomRightPoint) {}
};

class PassiveCollider
{
public:
	virtual BoundingRectangle GetBoundingRectangle() = 0;
};

class ActiveCollider : public PassiveCollider
{
private:
	std::vector<PassiveCollider*> m_Collidables;
	bool m_IsCollisionOn;
public:
	ActiveCollider(bool collisionOn);
	~ActiveCollider();

	inline void RegisterCollidable(PassiveCollider* collidable) { m_Collidables.push_back(collidable); }
	inline std::vector<PassiveCollider*> GetRegisteredCollidables() const { return m_Collidables; }

	inline bool IsCollisionOn() const { return m_IsCollisionOn; }
	inline void SetCollisionOn(bool collisionOn) { m_IsCollisionOn = collisionOn; }

	std::vector<PassiveCollider*> CheckCollision();

private:
	bool IsColliding(BoundingRectangle item1bound, BoundingRectangle item2bound);
};