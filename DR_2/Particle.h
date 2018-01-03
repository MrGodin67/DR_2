#pragma once
#include "Components.h"
#include "Animation.h"


class Vortex
{
public:
	Vec2f pos;
	Vec2f vel;
	float factor;
	float scale;
	float speed;
	void Update(const float& dt)
	{
		float damping = 1.0f - 0.005f;

		vel.x *= damping;
		vel.y *= damping;
		pos.x += vel.x;
		pos.y += vel.y;
	}
	Vortex(const Vec2f& in_pos)
	{
		RandG randG;
		pos = in_pos;
		vel.x = randG.Get<float>(10.0f, 100.0f);
		vel.y = randG.Get<float>(-10.0f, 10.0f);
		scale = 1.2f;
		scale *= scale;
		speed = randG.Get<float>(10.0f, 20.0f);
		speed = speed;
	};
};

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
	std::vector<Vortex> vortices;
public:
	Particle() = default;
	Particle( const Vec2f& size,
		const Animation::Sequence& seq, const float& lifeSpan, const bool& diffuse,
		const bool& doRotate);
	virtual ~Particle();
	bool Done()const;
	void Done(const bool& value);
	void SetGravity(const float& gravity);
	void SetDoScale(const bool& value);
	virtual void Reset(const Vec2f& pos, const Vec2f& vel, const float& zScaler);
	void Update(const float& dt)override;
	void Draw()override;
	void Init()override;
	void AddVortex(const Vec2f& pos)
	{
		vortices.emplace_back(pos);
	}
	
};

