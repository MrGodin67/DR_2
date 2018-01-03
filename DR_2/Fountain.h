#pragma once
#include "ECS_Manager.h"
#include "Components.h"
#include "D2D1Texture.h"
#include "Emitter.h"
class Fountain :
	public GameObject
{
	std::unique_ptr<D2D1Texture> image;
	std::unique_ptr<D2D1Texture> m_particle3;
	Emitter* emitter;
	float distanceToActivate;
public:
	Fountain() = default;
	Fountain(const Vec2f& pos,const Vec2f& size,const float& distanceToActivate);
	~Fountain();
	void Init()override;
	void AddEmitter(Emitter* e);
	void Draw()override;
	void Update(const float& dt)override;
	void DoEnableByDistanceTo(const Vec2f& objectPosition);
	void DoTranslation(const Vec2f& offset)override;
	
};

