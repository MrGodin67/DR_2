#include "stdafx.h"
#include "MapTile.h"
#include "Locator.h"
#include "Collider.h"
Vec2f MapTile::dimensions = { 8.0f,8.0f };
ID2D1Bitmap* MapTile::tileSet = nullptr;
void MapTile::Init()
{
	if (!passable)
	{
		AddGroup(groupCollider);
	}
	AddGroup(groupMap);
}
bool MapTile::Passable() const
{
	return passable;
}
MapTile::MapTile(const Vec2f& position,
	const RectF& srcRect, const bool& passable)
	:srcRect(srcRect),passable(passable)
{
	Add<Transform>(position, dimensions);
	if (!passable)
		Add<Collider>(position + (dimensions*0.5f), dimensions * 0.5f);

	mapLocation.row = position.y / dimensions.height;
	mapLocation.column = position.x / dimensions.width;

}


MapTile::~MapTile()
{
}

void MapTile::Draw()
{
	Locator::Gfx()->DrawSprite(Get<Transform>().Matrix(),
		Get<Transform>().Boundary().ToD2D(), tileSet, &srcRect.ToD2D());
}

void MapTile::SetTileAttributes(const Vec2f& dims, ID2D1Bitmap* image)
{
	dimensions = dims;
	tileSet = image;
}

Vec2f MapTile::GetDimensions()
{
	return dimensions;
}

Vec2i MapTile::MapLocation() const
{
	return mapLocation;
}
