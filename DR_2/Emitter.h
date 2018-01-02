#pragma once
#include "ECS_Manager.h"

#include "Particle.h"


#define OVERLOAD_TIMER

class Emitter : public GameObject
{
private:
	std::vector<Particle*> m_objects;
	std::vector<Particle*> m_renderObjects;

	// user defined
	
	float m_interval = 0.0f;
	float m_particleLifeSpan = 0.0f;
		
	std::size_t m_maxParticles = 0llu;
		
	bool m_doRandomSpawn = true;
	bool m_destroyParticle = false;
	bool m_loop = true;
	bool m_destroyThisWhenDone = false;
	//~ user defined

	// private to this

	std::size_t m_activeParticleCount = 0llu;

	bool m_initalized = false;
	//~ private to this

	float m_timer = FLT_MAX;
	RandG randG;
private:
	
	virtual void Spawn()
	{
		for (auto& it : m_objects)
		{
			if (it->Done())
			{
				it->Reset(Get<Transform>().Center(), Vec2f(randG.Get<float>(-100.0f, 100.0f), randG.Get<float>(-200.0f, -150.0f)));
				return;
			}
		}
	}
	void Refresh()
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
	//~ private to this


public:
	Emitter() = default;
	Emitter::Emitter(const Vec2f & spawnPosition)
	{
		Add<Transform>(spawnPosition, Vec2f(3.0f, 3.0f));

	}
	
	virtual Emitter::~Emitter()
	{
		for (auto& it : m_objects)
			it->Destroy();
	};
	template<typename ParticleType,typename... TArgs>
	ParticleType& AddPartical(TArgs&&... inArgs)
	{
		m_objects.push_back( &manager->AddObject<ParticleType>(std::forward<TArgs>(inArgs)...));
		return *m_objects.back();
	};
	
	
	// virtual functions
	void DoTransform(const Vec2f& offset)
	{
		for (auto& it : m_objects)
			it->Get<Transform>().Translate(offset);
	};
	void Update(const float& dt)override
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
		else if(m_renderObjects.size() > 0)
		{
			for (auto& iter : m_objects)
			{
				iter->Update(dt);
				
			}
		}
		
};
	
	void Draw()override
	{
		for (auto& it : m_renderObjects)
		{
			if(!it->Done())
			   it->Draw();
		}
	};
	void Init()override
	{
		AddGroup(groupEmitters);
	};
	void Start()
	{
		m_initalized = true;
	};
	void Stop(const bool& fullstop = false)
	{
		if (fullstop)
		{
			for (auto& it : m_objects)
				it->Done(true);
		}
		m_initalized = false;
	}
	//~ virtual functions
	void SetLooping(const bool& value)
	{
		m_loop = value;
	};
	void SetSpawnInterval(const float& value)
	{
		m_interval = value;
	};
	void SetDoRandomSpawn(const bool& value)
	{
		m_doRandomSpawn = value;
	};
	void SetDoDestroyParticle(const bool& value)
	{
		m_destroyParticle = value;
	};
	void DoDestroyThisWhenDone(const bool& value)
	{
		m_destroyThisWhenDone = value;
	};
	Vec2f GetPosition()
	{
		return Get<Transform>().Center();
	};
	
};


