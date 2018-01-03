#pragma once
#include "Particle.h"
#include "Collider.h"
class BallParticle :
	public Particle
{
	int numBounces;
	float YBounceVel;
	float YBounceVelConst;
public:
	BallParticle() = default;
	BallParticle(const Vec2f& size,
		const Animation::Sequence& seq, const float& lifeSpan, const bool& diffuse,
		const bool& doRotate);
	virtual ~BallParticle();
	void Init()override;
	void ResolveCollision(Collider* other);
	void SetNumberOfBounces(const int& value);
	void SetBounceYVelocity(const float& value);
	void Draw()override;
	void Reset(const Vec2f& pos, const Vec2f& vel, const float& zScaler)override;
};

