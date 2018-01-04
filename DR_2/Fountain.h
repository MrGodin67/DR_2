#pragma once
#include "ECS_Manager.h"
#include "Components.h"
#include "D2D1Texture.h"
#include "Emitter.h"
class Fountain :
	public GameObject
{
	
	Emitter* emitter;
	float distanceToActivate;
	ID2D1Bitmap* particle;
	
public:
	Fountain() = default;
	Fountain(const Vec2f& pos,const Vec2f& size,const float& distanceToActivate,ID2D1Bitmap* image,ID2D1Bitmap* particle);
	~Fountain();
	void Init()override;
	void AddEmitter(Emitter* e);
	void Draw()override;
	void Update(const float& dt)override;
	void DoEnableByDistanceTo(const Vec2f& objectPosition);
	void DoTranslation(const Vec2f& offset)override;
	
};

