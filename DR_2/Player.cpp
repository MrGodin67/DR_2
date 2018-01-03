#include "stdafx.h"
#include "Player.h"
#include "Locator.h"
#include "Animation.h"
#include "MapTile.h"
#include "Emitter.h"
#include "Particle.h"
void Player::DoIdle()
{
	
	horizDirection == -1.0f ? Get<Animation>().StartSequenceByName("left_idle") : 
		Get<Animation>().StartSequenceByName("right_idle");
	stateFlags[psWalking] = false;
	stateFlags[psIdle] = true;
	jetPack->Stop();
	
}
void Player::DoJump()
{
	horizDirection == -1.0f ? Get<Animation>().StartSequenceByName("left_jump"):
		Get<Animation>().StartSequenceByName("right_jump");
  	
	if (!stateFlags[psJumping])
	{
		Get<Transform>().velocity.y = -(gGravity* 40.0f);
		stateFlags[psJumping] = true;
		jetPack->Start();
	}
		
		
}

void Player::DoWalk()
{
	if (!stateFlags[psJumping])
	{

		if (horizDirection == -1.0f)
			Get<Animation>().StartSequenceByName("left_walk");
		else
			Get<Animation>().StartSequenceByName("right_walk");
	}
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
	if (Get<Transform>().velocity.y >= 0.0f)
		jetPack->Stop();
	jetPack->SetPosition(Vec2f(Get<Transform>().Center().x - 10.0f, Get<Transform>().Center().y - 10.0f));
	jetPack->Update(dt);
}

void Player::Draw()
{
	for (auto& it : m_components)
		it->Draw();
}
void Player::Init()
{
	jetPackParticle = std::make_unique<D2D1Texture>(Locator::D2DRenderTarget(), L"assets\\particle3.png");
	assert(jetPackParticle->GetBitmap());
	
	jetPack = &manager->AddObject<Emitter>(Get<Transform>().Center());
	jetPack->SetSpawnInterval(0.06f);
	jetPack->SetRandomVelocityConstrants(Vec2f(-25.0f, 25.0f), Vec2f(15.0f, 20.0f));
	
	Animation::Sequence seq;
	seq.current_index = 0llu;
	seq.frameDelay = 0.0f;
	seq.image = jetPackParticle->GetBitmap();
	seq.srcRects.push_back(RectF(0.0f, 0.0f, 256.0f, 256.0f));
	seq.timer = 0.0f;
	RandG randG;
	for (int c = 0; c < 18; c++)
	{
		int result = randG.Get<int>(0, 10);

		
		Particle* p = &jetPack->AddPartical<Particle>(Vec2f(16.0f, 16.0f),
			seq, 0.60f, false, true);
		p->SetGravity(gGravity* 0.25f);
		p->SetDoScale(false);
	
		
	}
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
void Player::ResolveCollisions()
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
				//Get<Transform>().velocity.x = 0.0f;
				break;
			case COLLISION_BOTTOM:
				stateFlags[psJumping] = false;
				Get<Transform>().velocity.y = 0.0f;
				jetPack->Stop();
				break;
			case COLLISION_TOP:
				Get<Transform>().velocity.y = 0.0f;
				jetPack->Stop();
				break;
			};

		}
	}
	
}
void Player::DoTranslation(const Vec2f & offset)
{
	Get<Transform>().Translate(offset);
	jetPack->DoTranslation(offset);
}


