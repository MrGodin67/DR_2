#include "stdafx.h"
#include "Jump.h"
#include "Player.h"
#include "Collider.h"

Jump::Jump()
{
}


Jump::~Jump()
{
}

void Jump::Init()
{
	transform = &owner->Get<Transform>();
}

void Jump::Update(const float & dt)
{
	transform->velocity.y += gGravity;
	transform->velocity.x += (transform->horizontalDirection * transform->acceleration);
}

void Jump::ApplyForce(const float & inertia)
{
     transform->velocity.y = inertia;
}

void Jump::CheckMapCollision(Collider * collider)
{
	Collider* OwnerCollider = &owner->Get<Collider>();
	Collision collision = OwnerCollider->AABBCollision(collider->AABB());
	if (collision.intersecting)
	{
		OwnerCollider->StaticCollisionCorrection(collider->AABB(), collision);
		switch (collision.side)
		{
		case COLLISION_BOTTOM:
		{
			Player* player = dynamic_cast<Player*>(owner);
			player->SetState(psIdle);
		}
		break;
		}
	}
}
