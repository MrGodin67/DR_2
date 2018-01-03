#include "stdafx.h"
#include "Particle.h"
#include "Collider.h"




Particle::Particle( const Vec2f & size, 
	const Animation::Sequence & seq, const float & lifeSpan, const bool & diffuse, 
	const bool & doRotate)
	:lifeSpan(lifeSpan), life(lifeSpan), diffuse(diffuse), doRotate(doRotate)
{
	Add<Transform>(Vec2f(), size);
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

void Particle::SetDoScale(const bool & value)
{
	doScale = value;
}

void Particle::Reset(const Vec2f & pos, const Vec2f & vel, const float & zScaler)
{
	Get<Transform>().position = pos;
	Get<Transform>().velocity = vel;
	Zscaler = zScaler;
	ZScaler2 = 1.0f;
	life = lifeSpan;
	done = false;
	Get<Animation>().SetTransparency(1.0f);
	
}

void Particle::Update(const float & dt)
{
	

	
	
	if (!done)
	{
		if ((life -= dt) > 0.0f)
		{

			if (diffuse)
			{
				if(life < (lifeSpan * 0.5f))
				   Get<Animation>().SetTransparency(life / lifeSpan);
			}
			
			
			{
				Get<Transform>().velocity.y += gravity;
				Get<Transform>().Update(dt);
			}
			if (doRotate)
			{
				Vec2f v = Get<Transform>().velocity;
				Get<Transform>().rotation_angle += v.Normalize().x;// .Angle();
				Get<Transform>().Rotate();
			}
			if (doScale)
			{
				float sign = Sign<float>(Zscaler);
				float scaler = (1.0f - (life / lifeSpan));
				float result;
				sign < 0.0f ? result = 1.0f + scaler : result = 1.0f - scaler;
				Get<Transform>().Scale({ result,result });

				if (sign < 0.0f)
				{
					if (!HasGroup(groupLayerFront))
					    AddGroup(groupLayerFront);
				}
				else
				{
					if (!HasGroup(groupLayerBack))
					   AddGroup(groupLayerBack);
				}
			}
			
		}
		else
		{
			done = true;
			Get<Transform>().Scale({ 1.0f,1.0f });
			ZScaler2 = 1.0f;
			if (doScale)
			{
				if (HasGroup(groupLayerBack))
					RemoveGroup(groupLayerBack);
				if (HasGroup(groupLayerFront))
					RemoveGroup(groupLayerFront);
			}
		}
	}
	if (Has<Collider>())
		Get<Collider>().Update( dt);

}

void Particle::Draw()
{
	if (!done)
		Get<Animation>().Draw();
}

void Particle::Init()
{
	AddGroup(groupParticle);
	AddGroup(groupLayerFront);
}

/*if(vortices.size() > 0)
			{
				vortices[0].Update(dt);
				Vec2f delta  = Get<Transform>().position - vortices[0].pos;
				float speed = vortices[0].speed;
				float vx = -delta.y*speed + vortices[0].vel.x;
				float vy = delta.x*speed + vortices[0].vel.y;
				float factor = 1.0f / (1.0f + (delta.x*delta.x + delta.y*delta.y) / vortices[0].scale);
				Get<Transform>().velocity.x += (vx - Get<Transform>().velocity.x)*factor;
				Get<Transform>().velocity.y += (vy - Get<Transform>().velocity.y)*factor;
				Get<Transform>().velocity.Normalize();
				}*/