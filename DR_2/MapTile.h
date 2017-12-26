#pragma once
#include "Components.h"
class MapTile :
	public GameObject
{
	bool passable;
	RectF srcRect;
	static Vec2f dimensions;
	static ID2D1Bitmap* tileSet;
	Vec2i mapLocation;
public:
	MapTile(const Vec2f& position,const RectF& srcRect,const bool& passable);
	virtual ~MapTile();
	void Draw()override;
	void Init()override;
	bool Passable()const;
	static void SetTileAttributes(const Vec2f& dims, ID2D1Bitmap* image);
	static Vec2f GetDimensions();
	Vec2i MapLocation()const;
};

