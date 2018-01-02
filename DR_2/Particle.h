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
	bool Done()const { return done; }
	void Done(const bool& value) { done = value; }
	void Reset(const Vec2f& pos, const Vec2f& vel)
	{ 
		Get<Transform>().position = pos;
		Get<Transform>().velocity = vel;
		life = lifeSpan;
		done = false;
		
	}
	void Update(const float& dt)override
	{
		if (!done)
		{
			if ((life -= dt) > 0.0f)
			{

				if (diffuse)
				{
					float trans = life / lifeSpan;
					Get<Animation>().SetTransparency(trans);

				}
				Get<Transform>().velocity.y += gGravity * 0.15f;
				Get<Transform>().Update(dt);
				if (doRotate)
				{
					Get<Transform>().rotation_angle = Get<Transform>().velocity.Angle();
					Get<Transform>().Rotate();
				}
			}
			else
			{
				done = true;
			}
		}
	}
	void Draw()override
	{
		if(!done)
		   Get<Animation>().Draw();
	}
	void Init()override
	{
		AddGroup(groupParticle);
	}
	
};

