#pragma once
#include "Components.h"
#include "TransformComponent.h"
class Jump :
	public PlayerState
{
	Transform* transform;
public:
	Jump();
	~Jump();
	void Init()override;
	void Update(const float& dt)override;
	void ApplyForce(const float& inertia);
	void CheckMapCollision(class Collider* collider)override;
};

