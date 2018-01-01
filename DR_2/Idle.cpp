#include "stdafx.h"
#include "Idle.h"
#include "Player.h"
#include "Collider.h"

Idle::Idle()
{
}


Idle::~Idle()
{
}

void Idle::Init()
{
	transform = &owner->Get<Transform>();
	//pPrevious->SetPrevious(this);
}

void Idle::Update(const float & dt)
{
	transform->velocity.x *= transform->friction;
	if (fabsf(transform->velocity.x) < 0.5f)
		transform->velocity.x = 0.0f;

}

void Idle::Transition(PlayerState * state)
{

}

void Idle::CheckMapCollision(Collider * collider)
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
