#pragma once
#include "ECS_Manager.h"
#include "TransformComponent.h"
class Collision
{
public:
	Vec2f distance = { 0.0f,0.0f };
	bool intersecting = false;
	Collision() = default;
	Collision(Vec2f dist, bool intersect)
		:distance(dist), intersecting(intersect)
	{}
};
class BoundingBox
{
public:
	Vec2f center;
	Vec2f half_extents;
	BoundingBox() = default;
	BoundingBox(const Vec2f& c, const Vec2f& ext)
		:center(c), half_extents(ext)
	{}
	Vec2f Center()const { return center; }
	Vec2f HalfExtents()const { return half_extents; }
	

};

class Sphere
{
public:
	Sphere() = default;
	Sphere(const Vec2f& position, const float& radius)
		:center(position), radius(radius)
	{}
	Vec2f center;
	float radius;
};

/*
Collision GetCollision(const BoundingBox& other)
{
	Vec2f distance = center - other.Center();
	distance.x = fabsf(distance.x);
	distance.y = fabsf(distance.y);
	Vec2f result = distance - (half_extents - other.HalfExtents());
	return Collision(result, result.x < 0.0f && result.y < 0.0f);
};
void CollisionCorrection(const BoundingBox& other, const Collision& collision)
{
	Vec2f correction = other.Center() - center;
	if (collision.distance.x > collision.distance.y)
	{
		if (correction.x > 0.0f)
		{
			center.x += collision.distance.x;
		}
		else
		{
			center.x += -collision.distance.x;
		}
	}
	else
	{
		if (correction.y > 0.0f)
		{
			center.y += collision.distance.y;
		}
		else
		{
			center.y += -collision.distance.y;
		}
	}

}*/

class Collider : public Component
{
	Sphere collisionSphere;
	Transform* transform;
	BoundingBox collisionBox;
public:
	Collider() = default;
	Collider(const Vec2f& center, const Vec2f& extents)
		:collisionBox(center, extents),collisionSphere(center, std::max(extents.width, extents.height))
	{}
	~Collider();
	// debug
	bool doDraw = false;
	virtual void Init()
	{
		transform = &owner->Get<Transform>();
	}
	virtual void Update(const float& dt)
	{
		collisionBox.center = collisionSphere.center = transform->Center();
	}
	virtual void Draw()override;
	BoundingBox AABB() { return collisionBox; }
	Sphere Sphere() { return collisionSphere; }
	Collision AABBCollision(const BoundingBox& other)
	{
		Vec2f distance = collisionBox.center - other.center;
		distance = distance.Absolute();

		Vec2f result = distance - (collisionBox.half_extents + other.half_extents);
		return Collision(result, result.x < 0.0f && result.y < 0.0f);
	};
	void StaticCollisionCorrection(const BoundingBox& other, const Collision& collision)
	{
		Vec2f correction = other.Center() - collisionBox.center;
		if (collision.distance.x > collision.distance.y)
		{
			if (correction.x > 0.0f)
			{
				collisionBox.center.x += collision.distance.x;
			}
			else
			{
				collisionBox.center.x += -collision.distance.x;
			}
		}
		else
		{
			if (correction.y > 0.0f)
			{
				collisionBox.center.y += collision.distance.y;
			}
			else
			{
				collisionBox.center.y += -collision.distance.y;
			}
		}
		transform->position = collisionBox.center + -(collisionBox.half_extents);
	}
};
