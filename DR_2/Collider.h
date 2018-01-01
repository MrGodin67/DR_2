#pragma once
#include "ECS_Manager.h"
#include "TransformComponent.h"

#define COLLISION_LEFT 1
#define COLLISION_RIGHT 2
#define COLLISION_TOP 3
#define COLLISION_BOTTOM 4

class Collision
{
public:
	Vec2f distance = { 0.0f,0.0f };
	bool intersecting = false;
	int side = -1;
	Collision() = default;
	Collision(Vec2f dist, bool intersect, int side)
		:distance(dist), intersecting(intersect),side(side)
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
	Sphere GetSphere() { return collisionSphere; }
	Collision AABBCollision(const BoundingBox& other)
	{
		Vec2f distance = collisionBox.center - other.center;
		distance = distance.Absolute();

		Vec2f result = distance - (collisionBox.half_extents + other.half_extents);
		Vec2f correction = other.Center() - collisionBox.center;
		int side;
		if (distance.x > distance.y)
		{
			if (correction.x > 0.0f) // left
			{
				side = COLLISION_RIGHT;
			}
			else // right
			{
			
				side = COLLISION_LEFT;
			}
		}
		else
		{
			if (correction.y > 0.0f) // bottom
			{
			
				side = COLLISION_BOTTOM;
			}
			else // top
			{
				
				side = COLLISION_TOP;
			}
		}
		return Collision(result, result.x < 0.0f && result.y < 0.0f,side);
	};
	Collision Spherical(const Sphere& other);
	void SphericalCollisionCorrection(class GameObject* other, const Collision& collision,bool isStatic = false);
	int StaticCollisionCorrection(const BoundingBox& other, const Collision& collision)
	{
		Vec2f correction = other.Center() - collisionBox.center;
		int side;
		if (collision.distance.x > collision.distance.y)
		{
			if (correction.x > 0.0f) // left
			{
				collisionBox.center.x += collision.distance.x;
				transform->velocity.x = 0.0f;
			}
			else // right
			{
				collisionBox.center.x += -collision.distance.x;
				transform->velocity.x = 0.0f;
			
			}
		}
		else
		{
			if (correction.y > 0.0f) // bottom
			{
				collisionBox.center.y += collision.distance.y;
				transform->velocity.y = 0.0f;
			}
			else // top
			{
				collisionBox.center.y += -collision.distance.y;
				transform->velocity.y = 0.0f;
				
			}
		}
		transform->position = collisionBox.center + -(collisionBox.half_extents);
		return -1;
	}
};

struct CollisionPair
{
	Collider* collider;
	Collision collision;
	
};
