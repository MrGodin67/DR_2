#include "stdafx.h"
#include "Particle.h"





Particle::Particle(const Vec2f & pos, const Vec2f & vel, const Vec2f & size, 
	const Animation::Sequence & seq, const float & lifeSpan, const bool & diffuse, 
	const bool & doRotate)
	:lifeSpan(lifeSpan), life(lifeSpan), diffuse(diffuse), doRotate(doRotate)
{
	Add<Transform>(pos, size).velocity = vel;
	Get<Transform>().acceleration = 120.0f;
	Add<Animation>().AddSequence("default", seq);
};

Particle::~Particle()
{
}

bool Particle::Done() const
{
	return done;
}

void Particle::Done(const bool & value)
{
	if (value)
	{
		Get<Transform>().Scale({ 1.0f,1.0f });
	}
	done = value;
}

void Particle::SetGravity(const float & gravity)
{
	this->gravity = gravity;
}

void Particle::Reset(const Vec2f & pos, const Vec2f & vel, const float & zScaler)
{
	Get<Transform>().position = pos;
	Get<Transform>().velocity = vel;
	Zscaler = zScaler;
	ZScaler2 = 1.0f;
	life = lifeSpan;
	done = false;

}

void Particle::Update(const float & dt)
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
			Get<Transform>().velocity.y += gravity;
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
			Get<Transform>().Scale({ 1.0f,1.0f });
			ZScaler2 = 1.0f;
		}
	}
}

void Particle::Draw()
{
	if (!done)
		Get<Animation>().Draw();
}

void Particle::Init()
{
	AddGroup(groupParticle);
}

