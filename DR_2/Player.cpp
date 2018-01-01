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
		Get<Transform>().velocity.y = -(gGravity* 30.0f);// *92.5f;
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
	m_moveImage = std::make_unique<D2D1Texture>(Locator::D2DRenderTarget(), L"assets\\robo64x64.png");
	assert(m_moveImage->GetBitmap());
	
	Add<Transform>(pos, size);
	Animation * an = &Add<Animation>();
	Animation::Sequence seq;
	seq.current_index = 0;
	seq.frameDelay = 0.080f;
	seq.image = m_moveImage->GetBitmap();
	seq.srcRects.push_back(RectF(0.0f, 0.0f, 64.0f, 64.0f));
	an->AddSequence("right_idle", seq);


	seq.srcRects.clear();
	seq.srcRects.push_back(RectF(64.0f, 0.0f, 128.0f, 64.0f));
	seq.srcRects.push_back(RectF(128.0f, 0.0f, 192.0f, 64.0f));
	seq.srcRects.push_back(RectF(192.0f, 0.0f, 256.0f, 64.0f));
	an->AddSequence("right_walk", seq);
	
	
	seq.srcRects.clear();
	seq.srcRects.push_back(RectF(192.0f, 128.0f, 256.0f, 192.0f));
	an->AddSequence("left_idle", seq);

	seq.srcRects.clear();
	seq.srcRects.push_back(RectF(128.0f, 128.0f, 192.0f, 192.0f));
	seq.srcRects.push_back(RectF(64.0f, 128.0f, 128.0f, 192.0f));
	seq.srcRects.push_back(RectF(0.0f, 128.0f, 64.0f, 192.0f));
	an->AddSequence("left_walk", seq);

	
}


Player::~Player()
{
}

void Player::Update(const float & dt)
{
	CapVelocity();
	
	Get<Input>().Update(dt);
	// do velocity mutations befor final update


	
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
		if (collision.side == COLLISION_BOTTOM)
		{
			stateFlags[psJumping] = false;
			
		}
	}

}
;

