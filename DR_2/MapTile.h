#pragma once
#include "Components.h"
class Unit;
class MapTile :
	public GameObject
{
	bool passable;
	RectF srcRect;
	static Vec2f dimensions;
	static ID2D1Bitmap* tileSet;
	Vec2i mapLocation;
	std::vector<Unit*> m_occupants;
public:
	MapTile(const Vec2f& position,const RectF& srcRect,const bool& passable);
	virtual ~MapTile();
	void Draw()override;
	void Init()override;
	bool Passable()const;
	void Passable(const bool& val);
	static void SetTileAttributes(const Vec2f& dims, ID2D1Bitmap* image);
	static Vec2f GetDimensions();
	Vec2i MapLocation()const;
	void AddUnit(Unit* unit);
	void RemoveUnit(Unit* unit);
};

