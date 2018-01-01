#include "stdafx.h"
#include "Player.h"
#include "Locator.h"
#include "Animation.h"
#include "MapTile.h"

void Player::DoIdle()
{
	
	horizDirection == -1.0f ? Get<Animation>().StartSequenceByName("left_idle") : 
		Get<Animation>().StartSequenceByName("right_idle");
	stateFlags[psWalking] = false;
	stateFlags[psIdle] = true;
	
	
}
void Player::DoJump()
{
	horizDirection == -1.0f ? Get<Animation>().StartSequenceByName("left_idle"):
		Get<Animation>().StartSequenceByName("right_idle");
  	
	if (!stateFlags[psJumping])
	{
		Get<Transform>().velocity.y = -(gGravity* 30.0f);
		stateFlags[psJumping] = true;
	}
		
		
}

void Player::DoWalk()
{

	if (horizDirection == -1.0f)
		Get<Animation>().StartSequenceByName("left_walk");
	else
		Get<Animation>().StartSequenceByName("right_walk");
	stateFlags[psIdle] = false;
	stateFlags[psWalking] = true;
}



Player::Player(const Vec2f& pos,  const Vec2f& size)
{
	
	
	Add<Transform>(pos, size);
	Add<Animation>();
	

	
}


Player::~Player()
{
}

void Player::Update(const float & dt)
{
	CapVelocity();
	
	Get<Input>().Update(dt);
	// do velocity mutations before final update


	
	if (stateFlags[psWalking])
	    Get<Transform>().velocity.x += (horizDirection * Get<Transform>().acceleration);

	if (stateFlags[psIdle])
	{
		Get<Transform>().velocity.x *= Get<Transform>().friction;
		if (fabsf(Get<Transform>().velocity.x) < 0.5f)
			Get<Transform>().velocity.x = 0.0f;
	}
	

	Get<Transform>().velocity.y += gGravity;
	Get<Transform>().Update(dt);

	Get<Collider>().Update(dt);
	// update animation with new positions
	Get<Animation>().Update(dt);

}

void Player::Draw()
{
	for (auto& it : m_components)
		it->Draw();
}

void Player::Init()
{
}

void Player::CapVelocity()
{
	Get<Transform>().velocity.x = std::max(-maxVelocity.x, Get<Transform>().velocity.x);
	Get<Transform>().velocity.x = std::min(maxVelocity.x, Get<Transform>().velocity.x);
	Get<Transform>().velocity.y = std::max(-maxVelocity.y, Get<Transform>().velocity.y);
	Get<Transform>().velocity.y = std::min(maxVelocity.y, Get<Transform>().velocity.y);
}
void Player::SetMaxVelocity(const Vec2f & vel)
{
	maxVelocity = vel;
}
void Player::SetInputFlags(std::bitset<ieNumberOf>& flags)
{
	
	inputFlags = flags;
	if (inputFlags[ieLeft])
	{
		horizDirection = -1.0f;
		DoWalk();
	}

	if (inputFlags[ieRight])
	{
		horizDirection = 1.0f;
		DoWalk();
	}
	if (!inputFlags[ieLeft] && !inputFlags[ieRight] && !inputFlags[ieSpace])
	{
		DoIdle();
	}
	if (inputFlags[ieSpace] && !stateFlags[psJumping])
	{
		DoJump();

	}
	
}


void Player::ResolveCollision(Collider * other)
{
	Collider* thisCollider = &Get<Collider>();
	if (other == thisCollider)
		return;
	Collision collision = thisCollider->AABBCollision(other->AABB());
	if (collision.intersecting)
	{
		thisCollider->StaticCollisionCorrection(other->AABB(), collision);
		switch (collision.side)
		{
		case COLLISION_LEFT:
		case COLLISION_RIGHT:
			Get<Transform>().velocity.x = 0.0f;
			break;
		case COLLISION_BOTTOM:
			stateFlags[psJumping] = false;
			Get<Transform>().velocity.y = 0.0f;
			break;
		case COLLISION_TOP:
			Get<Transform>().velocity.y = 0.0f;
			break;
		}

		
	}

}
;

