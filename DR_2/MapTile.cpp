#include "stdafx.h"
#include "MapTile.h"
#include "Locator.h"
#include "Collider.h"
#include "Unit.h"
#include "newAStar.h"
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
void MapTile::Passable(const bool & val)
{
	passable = val;
	Locator::PathFinder()->GetPathNode(mapLocation)->passable = val;
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

void MapTile::AddUnit(Unit * unit)
{
	auto& it = std::find(m_occupants.begin(), m_occupants.end(), unit);
	if (it != m_occupants.end())
		m_occupants.push_back(unit);
}

void MapTile::RemoveUnit(Unit * unit)
{
	m_occupants.erase(std::remove_if(m_occupants.begin(),m_occupants.end(),
		[unit](Unit* out_unit) 
	{
		return unit == out_unit;
	}), m_occupants.end());

}
