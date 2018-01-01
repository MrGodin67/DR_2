#include "stdafx.h"
#include "Walking.h"
#include "TransformComponent.h"
#include "Collider.h"
#include "Player.h"
Walking::Walking(const float& direction)
	:dir(direction)
{
}


Walking::~Walking()
{
}

void Walking::Update(const float & dt)
{
	transform->velocity.x += (dir * transform->acceleration);
}

void Walking::Init()
{
	transform = &owner->Get<Transform>();
	// dir will one of three states -1,0,1
	transform->velocity.x = dir * fabsf(transform->velocity.x);
	
}

void Walking::SetDirection(const float & direction)
{
	dir = direction;
}

void Walking::CheckMapCollision(Collider * collider)
{
	Collider* OwnerCollider = &owner->Get<Collider>();
	Collision collision = OwnerCollider->AABBCollision(collider->AABB());
	if (collision.intersecting)
	{
		OwnerCollider->StaticCollisionCorrection(collider->AABB(), collision);
		
	}
	/*else
	{
		Player* player = dynamic_cast<Player*>(owner);
		player->SetState(psJumping);
	}*/

}
