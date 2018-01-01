#pragma once
#include "Components.h"
#include "Particle.h"
#include "TransformComponent.h"
class Emitter :
	public GameObject
{
	std::vector<Particle*> m_objects;
	
public:
	struct EmitterDesc
	{
		float timer;
		float interval;
		Vec2f position;
		std::size_t maxSpawnCount;
		bool destroyWhenDone = false;
		bool randomSpawnVel = false;
		bool active = false;
	};
	EmitterDesc data;
	std::size_t spawnCount;
	RandG randG;
	void Refresh()
	{
		m_objects.erase(std::remove_if(m_objects.begin(),m_objects.end(),
			[](Particle* p) 
		{
			if(p->Done())
				p->Destroy();// prepare for entity manager
			return p->Done();
		}
		), m_objects.end());
	}
public:
	Emitter(EmitterDesc& desc)
		:data(desc)
	{
		Add<Transform>(desc.position,Vec2f(32.0f,32.0f));

	};
	virtual ~Emitter() 
	{
		for (auto& it : m_objects)
			it->Destroy();
	};
	template<typename Type,typename... TArgs>
	Type& AddPartical(TArgs&&... inArgs)
	{
		m_objects.push_back( &manager->AddObject<Particle>(std::forward<TArgs>(inArgs)...));
		return *m_objects.back();
	}
	void DoTransform(const Vec2f& offset)
	{
		for (auto& it : m_objects)
			it->Get<Transform>().Translate(offset);
	};
	void Update(const float& dt)override
	{
		if (data.destroyWhenDone)
			Refresh();
		for (auto& i : m_objects)
		{
			
			if (i->Done())
			{
				if (!data.destroyWhenDone)
				{
					std::swap(i, m_objects.back());
				}
				
			}
			
			i->Update(dt);
			
		}
		if (data.active)
		{
			if ((data.timer += dt) >= data.interval)
			{
				Spawn();
				data.timer = 0.0f;
			}
		}
	};
	void Spawn()
	{
		for (auto& i : m_objects)
		{
			if (i->Done())
			{
				i->Reset(data.position, Vec2f(randG.Get<float>(-100.0f, 100.0f), randG.Get<float>(-100.0f, -50.0f)));
				std::swap(i, m_objects.back());
				return;
			}
		}
	}
	void Draw()override
	{
		for (auto& i : m_objects)
		{
			if(!i->Done())
			   i->Draw();
		}
	};
	void Init()override
	{
		AddGroup(groupMap);
	};
	void Start()
	{
		data.active = true;
	};
	void Stop()
	{
		data.active = false;
	}
	Vec2f GetPosition()
	{
		return Get<Transform>().Center();
	}

};

