#include "stdafx.h"
#include "Emitter.h"




;

void Emitter::Spawn()
{
	for (auto& it : m_objects)
	{

		if (it->Done())
		{
			Vec2f resultVelocity;
			if (!m_randomVelocityDescribed && m_doRandomSpawn)
			{
				it->Reset(Get<Transform>().Center(),GetRandomVector(),
					randG.Get<float>(-10.0f, 10.0f));
			}
			else if (m_doRandomSpawn)
			{

				it->Reset(Get<Transform>().Center(), Vec2f(randG.Get<float>(m_velocityRandomConstrantsX.min, m_velocityRandomConstrantsX.max),
					randG.Get<float>(m_velocityRandomConstrantsY.min, m_velocityRandomConstrantsY.max)),
					randG.Get<float>(-10.0f, 10.0f));
			}
			else
			{
				it->Reset(Get<Transform>().Center(),m_velocity,randG.Get<float>(-10.0f, 10.0f));
			}
			return;
		}
	}
}

void Emitter::Refresh()
{
	if (m_destroyParticle)
	{
		m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(),
			[](Particle* p)
		{
			if (p->Done())
				p->Destroy();// prepare for entity manager
			return p->Done();
		}
		), m_objects.end());
	};
	std::sort(m_objects.begin(), m_objects.end(),
		[](Particle* a, Particle* b)->bool
	{
		return (int)a->Done() < (int)b->Done();
	});
}

Emitter::Emitter(const Vec2f & spawnPosition)
{
	Add<Transform>(spawnPosition, Vec2f(3.0f, 3.0f));

}

Emitter::~Emitter()
{
	for (auto& it : m_objects)
		it->Destroy();
}
void Emitter::DoTransform(const Vec2f & offset)
{
	for (auto& it : m_objects)
		it->Get<Transform>().Translate(offset);
}
void Emitter::Update(const float & dt)
{

	Refresh();
	if (this->m_initalized)
	{
		m_renderObjects.clear();
		if ((m_timer += dt) >= m_interval)
		{
			Spawn();
			m_timer = 0.0f;
		}

		for (auto& iter : m_objects)
		{
			iter->Update(dt);
			if (!iter->Done())
			{
				m_renderObjects.push_back(iter);
			};
		}
	}
	// allow objects to finish if this is stopped
	else if (m_renderObjects.size() > 0)
	{
		for (auto& iter : m_objects)
		{
			iter->Update(dt);

		}
	}

}
void Emitter::Draw()
{
	for (auto& it : m_renderObjects)
	{
		if (!it->Done())
			it->Draw();
	}
}
void Emitter::Init()
{
	AddGroup(groupEmitters);
}
void Emitter::Stop(const bool & fullstop)
{
	if (fullstop)
	{
		for (auto& it : m_objects)
			it->Done(true);
	}
	m_initalized = false;
}
void Emitter::SetLooping(const bool & value)
{
	m_loop = value;
}
void Emitter::SetSpawnInterval(const float & value)
{
	m_interval = value;
}
void Emitter::SetDoRandomSpawn(const bool & value)
{
	m_doRandomSpawn = value;
}
void Emitter::SetDoDestroyParticle(const bool & value)
{
	m_destroyParticle = value;
}
void Emitter::DoDestroyThisWhenDone(const bool & value)
{
	m_destroyThisWhenDone = value;
}
Vec2f Emitter::GetPosition()
{
	return Get<Transform>().Center();
}
void Emitter::SetRandomVelocityConstrants(const Vec2f & X, const Vec2f & Y)
{
	m_velocityRandomConstrantsX = X;
	m_velocityRandomConstrantsY = Y;
	m_randomVelocityDescribed = true;
}
void Emitter::SetVelocity(const Vec2f & value)
{
	m_velocity = value;
}

void Emitter::SetWind(const Vec2f & value)
{
	m_wind = value;
}

