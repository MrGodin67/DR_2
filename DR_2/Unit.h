#pragma once
#include "ECS_Manager.h"
#include "Components.h"
#include "Collider.h"
#include "MapTile.h"
class UnitItem;
class Unit :
	public GameObject
{
	std::vector<UnitItem*> m_items;
	ID2D1Bitmap* image;
	RectF srcRect;
	bool selected;
	
public:
	Unit(const Vec2f& position,const Vec2f& size, 
		const RectF& srcRect, ID2D1Bitmap* image);
	virtual ~Unit();
	MapTile* currentTile = nullptr;
	virtual void SetPath(const std::vector<Vec2f>& positions);;
	void AddItem(UnitItem* item);
	void Draw()override;
	bool Selected()const;
	void Selected(const bool& val);
	void Update(const float& dt)override;
	void UpdateTile();
	Vec2i MapLocation();
	bool Moving();
	
};

class UnitItem : public GameObject
{
	Unit* owner;
	
public:
	UnitItem(Unit* owner)
		:owner(owner)
	{}
	virtual void Init()override
	{
		Vec2f pos = owner->Get<Transform>().position;
		Vec2f size = Vec2f(owner->Get<Transform>().Boundary().GetWidth(),
			owner->Get<Transform>().Boundary().GetHeight());
		Add<Transform>(pos, size);

	};
};

