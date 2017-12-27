#include "stdafx.h"
#include "Unit.h"
#include "Locator.h"
#include "PathComponent.h"
#include "SelectedComponent.h"
#include "newAStar.h"
Unit::Unit(const Vec2f& position, const Vec2f& size,
	const RectF& srcRect, ID2D1Bitmap* image)
	:srcRect(srcRect),image(image)
{
	Add<Transform>(position,size);
	Add<Collider>(position + (size * 0.5f), size * 0.5f);
	Add<SelectedRect>();
}


Unit::~Unit()
{
}

void Unit::SetPath(const std::vector<Vec2f>& positions)
{
	Remove<Path>();
	Add<Path>(positions);
	
}

void Unit::AddItem(UnitItem * item)
{
	m_items.push_back((UnitItem*)&manager->AddObject<UnitItem>(this));
}
void Unit::Draw()
{
	Locator::Gfx()->DrawSprite(Get<Transform>().Matrix(),
		Get<Transform>().Boundary().ToD2D(), image, &srcRect.ToD2D());
	Get<SelectedRect>().Draw();
}

bool Unit::Selected() const
{
	return selected;
}

void Unit::Selected(const bool & val)
{
	selected = val;
}

void Unit::Update(const float & dt)
{
	for (auto& it : m_components)
		it->Update(dt);

	UpdateTile();
}

void Unit::UpdateTile()
{
	Vec2i pos = { (int)(Get<Transform>().Center().x / MapTile::GetDimensions().width),
		(int)(Get<Transform>().Center().y / MapTile::GetDimensions().height)};
	MapTile * tile =  manager->GetTile(pos);
	if (tile != currentTile)
	{
		if (currentTile)
		{
			currentTile->Passable(true);
			currentTile->RemoveUnit(this);
		}
		currentTile = tile;
		currentTile->AddUnit(this);
		
	}
	if (Has<Path>())
	{
		if (Get<Path>().Completed())
		{
			Remove<Path>();
			
		}
	}
}

Vec2i Unit::MapLocation()
{
	  return currentTile->MapLocation() ;
}

bool Unit::Moving()
{
	return Has<Path>();
}




