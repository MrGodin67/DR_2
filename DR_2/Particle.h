#pragma once
#include "Components.h"
#include "Animation.h"
class Particle :
	public GameObject
{
protected:
	float lifeSpan;
	float life = 0.0f;
	bool diffuse;
	bool doRotate = false;
	bool done = true;
	bool doScale = false;
	float Zscaler = 1.0f;
	float ZScaler2 = 1.0f;
	float gravity = 0.0f;
public:
	Particle() = default;
	Particle(const Vec2f& pos, const Vec2f& vel, const Vec2f& size,
		const Animation::Sequence& seq, const float& lifeSpan, const bool& diffuse,
		const bool& doRotate);
	virtual ~Particle();
	bool Done()const;
	void Done(const bool& value);
	void SetGravity(const float& gravity);
	void SetDoScale(const bool& value);
	virtual void Reset(const Vec2f& pos, const Vec2f& vel, const float& zScaler);
	void Update(const float& dt)override;
	//void Draw()override;
	void Init()override;
	
};

