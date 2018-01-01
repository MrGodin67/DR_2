#pragma once
#include "ECS_Manager.h"
class Walking :
	public PlayerState
{
	float dir = 1.0f;
	Transform* transform;
public:
	Walking(const float& direction);
	~Walking();
	void Update(const float& dt)override;
	void Init()override;
	void SetDirection(const float& direction);
	void CheckMapCollision(class Collider* collider)override;
};

