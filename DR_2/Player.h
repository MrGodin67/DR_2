#pragma once
#include "ECS_Manager.h"

#include "D2D1Texture.h"
#include "Collider.h"
#include "Input.h"
#include "Emitter.h"
#include "Particle.h"
enum PlayerStates : std::size_t
{
	psIdle,
	psWalking,
	psJumping,
	psNumberOf
};
class Player :
	public GameObject
{
	std::unique_ptr<D2D1Texture> jetPackParticle;
	std::bitset<ieNumberOf> inputFlags;
	std::bitset<psNumberOf> stateFlags;
	Vec2f maxVelocity = {100.0f,500.0f};
	float horizDirection = 0.0f;
	Emitter* jetPack;
	void DoIdle();
	void DoJump();
	void DoWalk();
public:
	Player(const Vec2f& pos, const Vec2f& size);
	virtual ~Player();
	virtual void Update(const float& dt)override;	
	virtual void Draw()override;
	virtual void Init()override;
	void CapVelocity();
	void SetMaxVelocity(const Vec2f& vel);
	void SetInputFlags(std::bitset<ieNumberOf>& flags);
	void ResolveCollisions();
	void DoTranslation(const Vec2f& offset)override;
};

