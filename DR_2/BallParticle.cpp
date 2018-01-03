#include "stdafx.h"
#include "BallParticle.h"
#include "Arena.h"
#include "MapTile.h"
BallParticle::BallParticle(const Vec2f& size,
	const Animation::Sequence& seq, const float& lifeSpan, const bool& diffuse,
	const bool& doRotate)
	:Particle(size,seq,lifeSpan,diffuse,doRotate)
{
	
	
}


BallParticle::~BallParticle()
{
}

void BallParticle::Init()
{
	AddGroup(groupCollider);
	AddGroup(groupParticle);
	AddGroup(groupLayerFront);
	Add<Collider>(Vec2f(), Vec2f(Get<Transform>().Boundary().GetWidth(), Get<Transform>().Boundary().GetHeight()) * 0.5f);
}

void BallParticle::SetNumberOfBounces(const int & value)
{
	numBounces = value;
}

void BallParticle::SetBounceYVelocity(const float & value)
{
	YBounceVel = YBounceVelConst = value;
}

void BallParticle::Draw()
{
	if (!done)
		Get<Animation>().Draw();
}

void BallParticle::Reset(const Vec2f & pos, const Vec2f & vel, const float & zScaler)
{
	Get<Transform>().position = pos;
	Get<Transform>().velocity = vel;
	Zscaler = zScaler;
	life = lifeSpan;
	done = false;
	Get<Animation>().SetTransparency(1.0f);
	YBounceVel = YBounceVelConst;
}

void BallParticle::ResolveCollision(Collider * other)
{
	/*Collider* thisCollider = &Get<Collider>();
	if (other == thisCollider)
		return;
	Collision collision = thisCollider->AABBCollision(other->AABB());
	if (collision.intersecting )
	{
		
			thisCollider->StaticCollisionCorrection(other->AABB(), collision);
			switch (collision.side)
			{
			case COLLISION_LEFT:
			case COLLISION_RIGHT:
				Get<Transform>().velocity.x = -Get<Transform>().velocity.x;
				Get<Transform>().velocity.x *= 0.89f;
				break;
			case COLLISION_BOTTOM:
				YBoundVel *= 0.999f;
				if (YBoundVel < 0.001f)
				{
					YBoundVel = 0.0f;

				}
				Get<Transform>().velocity.y = YBoundVel;
				break;
			case COLLISION_TOP:
				Get<Transform>().velocity.y = -Get<Transform>().velocity.y;

			}
		
	}*/
	if (!done)
	{
		
		std::vector<Collider*> colliders = manager->GetMapColliders(MapTile::MakeIndices(Get<Transform>().position));
		if (colliders.size() > 0)
		{
			std::sort(colliders.begin(), colliders.end(), [this](Collider* a, Collider* b)
			{
				return (Get<Transform>().Center() - a->AABB().Center()).LenSq() <
					(Get<Transform>().Center() - b->AABB().Center()).LenSq();
			});
		}
		for (auto& col : colliders)
		{
			Collision collision = Get<Collider>().AABBCollision(col->AABB());
			if (collision.intersecting)
			{

				Get<Collider>().StaticCollisionCorrection(col->AABB(), collision);
				switch (collision.side)
				{
				case COLLISION_LEFT:
				case COLLISION_RIGHT:
					Get<Transform>().velocity.x = -Get<Transform>().velocity.x;
					Get<Transform>().velocity.x *= 0.89f;
					break;
				case COLLISION_BOTTOM:
					YBounceVel *= 0.599f;
					
					Get<Transform>().velocity.y = YBounceVel;
					break;
				case COLLISION_TOP:
					Get<Transform>().velocity.y = -Get<Transform>().velocity.y;

				}

			}
		}
	}
}
	
