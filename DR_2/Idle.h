#pragma once
#include "Components.h"
class Idle :
	public PlayerState
{
	Transform* transform;
	
public:
	Idle();
	~Idle();
	void Init()override;
	void Update(const float& dt)override;
	void Transition(PlayerState* state)override;
	void CheckMapCollision(class Collider* collider)override ;
};

