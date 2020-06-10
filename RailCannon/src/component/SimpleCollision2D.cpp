#include "SimpleCollision2D.h"

ActiveCollider::ActiveCollider(bool collisionOn)
	: m_IsCollisionOn(collisionOn)
{
}

ActiveCollider::~ActiveCollider()
{
}

std::vector<PassiveCollider*> ActiveCollider::CheckCollision()
{
	std::vector<PassiveCollider*> HitList;

	if (m_IsCollisionOn)
	{
		BoundingRectangle selfBound = GetBoundingRectangle();
		for (auto* collidable : m_Collidables)
		{
			if (IsColliding(this->GetBoundingRectangle(), collidable->GetBoundingRectangle()))
			{
				HitList.push_back(collidable);
			}
		}
	}
	return HitList;
}

bool ActiveCollider::IsColliding(BoundingRectangle item1bound, BoundingRectangle item2bound)
{
	if (item1bound.TopLeftPoint[0] >= item2bound.BottomRightPoint[0] || item2bound.TopLeftPoint[0] >= item1bound.BottomRightPoint[0])
		return false;
	else if (item1bound.TopLeftPoint[1] <= item2bound.BottomRightPoint[1] || item2bound.TopLeftPoint[1] <= item1bound.BottomRightPoint[1])
		return false;
	else 
		return true;
}

//// Returns true if two rectangles (l1, r1) and (l2, r2) overlap 
//bool doOverlap(Point l1, Point r1, Point l2, Point r2)
//{
//	// If one rectangle is on left side of other 
//	if (l1.x >= r2.x || l2.x >= r1.x)
//		return false;
//
//	// If one rectangle is above other 
//	if (l1.y <= r2.y || l2.y <= r1.y)
//		return false;
//
//	return true;
//}