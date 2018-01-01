#include "stdafx.h"
#include "Player.h"
#include "Locator.h"
#include "Animation.h"
#include "MapTile.h"
#include "Idle.h"
#include "Walking.h"
#include "Jump.h"
void Player::DoIdle()
{
	if (Get<Transform>().horizontalDirection == -1.0f)
		Get<Animation>().StartSequenceByName("left_idle");
	else
		Get<Animation>().StartSequenceByName("right_idle");
	pCurrentState = states[psIdle];

}
void Player::DoJump()
{
	if (jumped)return;
  		if (Get<Transform>().horizontalDirection == -1.0f)
			Get<Animation>().StartSequenceByName("left_idle");
        else
  			Get<Animation>().StartSequenceByName("right_idle");

		
			if (!Get<Transform>().jumping)
			{
				//Jump* jump = dynamic_cast<Jump*>(states[psJumping]);
				Get<Transform>().velocity.y = -(gGravity* 30.0f);// *92.5f;
               //	pCurrentState = states[psJumping];
				Get<Transform>().jumping = true;
			}
		
	
}

void Player::DoWalk()
{

	if (Get<Transform>().horizontalDirection == -1.0f)
		Get<Animation>().StartSequenceByName("left_walk");
	else
		Get<Animation>().StartSequenceByName("right_walk");

	pCurrentState = states[psWalking];
	Walking* walk = dynamic_cast<Walking*>(states[psWalking]);
	
	
}



Player::Player(const Vec2f& pos,  const Vec2f& size)
{
	m_moveImage = std::make_unique<D2D1Texture>(Locator::D2DRenderTarget(), L"assets\\robo64x64.png");
	assert(m_moveImage->GetBitmap());
	
	Add<Transform>(pos, size);
	Animation * an = &Add<Animation>();
	Animation::Sequence seq;
	seq.current_index = 0;
	seq.frameDelay = 0.20f;
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


	states[psIdle] = &Add<Idle>();
	states[psWalking] = &Add<Walking>(1.0f);
	states[psJumping] = &Add<Jump>();
	pCurrentState = states[psJumping];
}


Player::~Player()
{
}

void Player::Update(const float & dt)
{
	CapVelocity();
	
	Get<Input>().Update(dt);
	// do velocity mutations befor final update
	pCurrentState->Update(dt);
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
		Get<Transform>().horizontalDirection = -1.0f;
		DoWalk();
	}
   if (inputFlags[ieRight])
	{
	   Get<Transform>().horizontalDirection = 1.0f;
		DoWalk();
		
	}
    if (inputFlags[ieDown])
   {
	   //Get<Animation>().StartSequenceByName()
   }
  
	if (inputFlags[ieSpace] && !jumped)
	{
		DoJump();
		
	} 
	if (!inputFlags[ieLeft] && !inputFlags[ieRight] && !inputFlags[ieSpace])
		DoIdle();
	
}
void Player::SetState(std::size_t state_id)
{
	switch (state_id)
	{
	case psIdle:
		DoIdle();
		break;
	case psWalking:
		DoWalk();
		break;
	case psJumping:
		DoJump();
		break;
	
	};
}
PlayerState * Player::GetState()
{
	return dynamic_cast<PlayerState*>(pCurrentState);
}
float Player::HorizontalDirection() const
{
	return horizontalDirection;
}
;

