#include "stdafx.h"
#include "ECS_Manager.h"
#include "MapTile.h"
#include "Arena.h"
#include "Collider.h"
#include "TransformComponent.h"



void GameObject::Draw()
{
	for (auto& comp : m_components)
		comp->Draw();
}
void GameObject::Update(const float & dt)
{
	for (auto& comp : m_components)
		comp->Update(dt);
}
void GameObject::DoTranslation(const Vec2f & offset)

{
	if (Has<Transform>())
		Get<Transform>().Translate(offset);
};
;


bool GameObject::Alive() const
{
	return m_active;
}

void GameObject::Destroy()
{
	m_active = false;
}

bool GameObject::HasGroup(const GroupID & id) const
{
	return m_groupBitSet[id];
}

void GameObject::AddGroup(const GroupID & id)
{
	m_groupBitSet[id] = true;
	manager->AddToGroup(this, id);
}

void GameObject::RemoveGroup(const GroupID & id)
{
	m_groupBitSet[id] = false;
}

Vec2f GameObject::Center() const

{
	if (Has<Transform>())
		return Get<Transform>().Center();
	return{ 0.0f,0.0f };
}









ECS_Manager::ECS_Manager()
{
}


ECS_Manager::~ECS_Manager()
{
}

MapTile * ECS_Manager::GetTile(const Vec2i & location)
{
	auto& level(groupedByType[GetID<Arena>()]);
	auto& vec(groupedByType[GetID<MapTile>()]);
	Arena* a = (Arena*)level[0];
	return (MapTile*)vec[location.row *( a->GetData().width )  + location.column];
	
}

std::vector<MapTile*> ECS_Manager::GetMapPartition(const Vec2i & location, const int & objectCount)
{
	std::vector<MapTile*> list;
	auto& level(groupedByType[GetID<Arena>()]);
	Arena* arena = (Arena*)level[0];
	int step = 1;
	while (list.size() < objectCount)
	{
		list.clear();
		for (int r = -step; r <= step; r++)
		{
			for (int c = -step; c <= step; c++)
			{
				int x = location.x + c;
				int y = location.y + r;
				if (x < 0)x = 0;
				if (x > arena->GetData().width)
					x = arena->GetData().width;
				if (y < 0)y = 0;
				if (y > arena->GetData().height)
					y = arena->GetData().height;

				if (GetTile(Vec2i(x, y))->Passable())
				{
					auto& it = std::find(list.begin(), list.end(), GetTile(Vec2i(x, y)));
					if (it == list.end())
					{
						list.push_back(GetTile(Vec2i(x, y)));
					}
				}
			}
		}
		
	
		
		
		step++;
		if (step > objectCount)
		{
			return std::vector<MapTile*>();
		}
	}
	
	return list;
}

std::vector<Collider*> ECS_Manager::GetMapColliders(const Vec2i & location)
{
	std::vector<Collider*> list;
	auto& level(groupedByType[GetID<Arena>()]);
	Arena* arena = (Arena*)level[0];
	int step = 1;
	
		list.clear();
		for (int r = -step; r <= step; r++)
		{
			for (int c = -step; c <= step; c++)
			{
				int x = location.x + c;
				int y = location.y + r;
				if (x < 0)x = 0;
				if (x >= arena->GetData().width)
					x = arena->GetData().width-1;
				if (y < 0)y = 0;
				if (y >= arena->GetData().height)
					y = arena->GetData().height-1;

				if (!GetTile(Vec2i(x, y))->Passable())
				{
					auto& it = std::find(list.begin(), list.end(), &GetTile(Vec2i(x, y))->Get<Collider>());
					if (it == list.end())
					{
						list.push_back(&GetTile(Vec2i(x, y))->Get<Collider>());
					}
				}
			
		}




	}

	return list;
}

std::vector<Collider*> ECS_Manager::GetMapColliders(const Vec2i & startIndicies, const Vec2i & endIndices)
{
	std::vector<Collider*> list;

	for (int r = startIndicies.y; r <= endIndices.y; r++)
	{
		for (int c = startIndicies.x; c <= endIndices.x; c++)
		{
			
			MapTile* tile = GetTile({ r,c });
			assert(tile);
			if (!tile->Passable())
				list.push_back(&tile->Get<Collider>());
		}
	}
	
	return list;
}

std::vector<class Collider*> ECS_Manager::GetVerticalMapColliders(const Vec2i & location, const int & dir)
{
	return std::vector<class Collider*>();
}

std::vector<class Collider*> ECS_Manager::GetHorizontalMapColliders(const Vec2i & location, const int & dir)
{
	return std::vector<class Collider*>();
}


std::vector<GameObject*> ECS_Manager::GetGroupAction(const GroupID & id)
{
	return groupedByAction[id];
}

PlayerState * PlayerState::GetPrevious()
{
	return pPrevious;
}


