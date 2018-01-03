#pragma once
#include "ECS_Manager.h"

#include "Particle.h"


#define OVERLOAD_TIMER

class Emitter : public GameObject
{
private:
	std::vector<Particle*> m_objects;
	std::vector<Particle*> m_activeObjects;

	// user defined
	
	float m_interval = 0.0f;
	float m_particleLifeSpan = 0.0f;
		
	std::size_t m_maxParticles = 0llu;
		
	bool m_doRandomSpawn = true;
	bool m_destroyParticle = false;
	bool m_loop = true;
	bool m_destroyThisWhenDone = false;
	
	Vec2f m_velocityRandomConstrantsY = {-1.0f,1.0f};
	Vec2f m_velocityRandomConstrantsX = {-1.0f,1.0f};
	Vec2f m_velocity = { 0.0f,0.0f };
	Vec2f m_wind = { 0.0f,0.0f };
	//~ user defined

	// private to this

	std::size_t m_activeParticleCount = 0llu;

	bool m_initalized = false;
	bool m_randomVelocityDescribed;
	//~ private to this
#ifdef OVERLOAD_TIMER
	float m_timer = FLT_MAX;
#else
    float m_timer = 0.0f;
#endif
RandG randG;
private:
	
	virtual void Spawn();
	virtual void Refresh();
	
	//~ private to this


public:
	Emitter() = default;
	Emitter::Emitter(const Vec2f & spawnPosition);
	
	virtual Emitter::~Emitter();
	template<typename ParticleType,typename... TArgs>
	ParticleType& AddPartical(TArgs&&... inArgs)
	{
		m_objects.push_back( &manager->AddObject<ParticleType>(std::forward<TArgs>(inArgs)...));
		return *(ParticleType*)m_objects.back();
	};
	
	
	// virtual functions
	void DoTranslation(const Vec2f& offset)override;
	void Update(const float& dt)override;
	
	void Draw()override;
	void Init()override;
	void Start()
	{
		m_initalized = true;
	};
	void Stop(const bool& fullstop = false);
	//~ virtual functions
	void SetLooping(const bool& value);
	void SetSpawnInterval(const float& value);
	void SetDoRandomSpawn(const bool& value);
	void SetDoDestroyParticle(const bool& value);
	void DoDestroyThisWhenDone(const bool& value);
	Vec2f GetPosition();
	void SetRandomVelocityConstrants(const Vec2f& X, const Vec2f& Y);
	void SetVelocity(const Vec2f& value);
	void SetWind(const Vec2f& value);
	void SetPosition(const Vec2f& value);
	
};


