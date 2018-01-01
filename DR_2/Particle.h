#pragma once
#include "ECS_Manager.h"
#include "Animation.h"
class Particle :
	public GameObject
{
protected:
	float lifeSpan;
	float life = 0.0f;
	bool diffuse;
	bool spawned = false;
	bool doRotate = false;
public:
	Particle() = default;
	Particle(const Vec2f& pos, const Vec2f& vel, const Vec2f& size,
		const Animation::Sequence& seq, const float& lifeSpan, const bool& diffuse,
		const bool& doRotate)
		:lifeSpan(lifeSpan),life(lifeSpan),diffuse(diffuse),doRotate(doRotate)
	{
		Add<Transform>(pos, size).velocity = vel;
		Get<Transform>().acceleration = 120.0f;
		Add<Animation>().AddSequence("default",seq);
	};
	virtual ~Particle();
	bool Done() { return life <= 0.0f; }
	void Reset(const Vec2f& pos, const Vec2f& vel)
	{ 
		Get<Transform>().position = pos;
		Get<Transform>().velocity = vel;
		life = lifeSpan;
		spawned = true;
	}
	void Update(const float& dt)override
	{
		
			life -= dt;
			spawned = !Done();
			if (doRotate)
			{
				Get<Transform>().rotation_angle = Get<Transform>().velocity.Angle();
				Get<Transform>().Rotate();
			}
			if (diffuse && !Done())
			{

				Get<Animation>().SetTransparency(life / lifeSpan);

			}
			Get<Transform>().velocity.y += gGravity * 0.025f;
			Get<Transform>().Update(dt);
		
	}
	void Draw()override
	{
		if(spawned)
		   Get<Animation>().Draw();
	}
	
};

