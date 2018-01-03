#pragma once
#include <map>
#include "Vec2.h"

class ECS_Manager;

class Transform;
class Component;
class GameObject;
using ComponentID = std::size_t;
using GroupID = std::size_t;

inline ComponentID getUniqueComponentTypeID()
{
	static ComponentID cID = 0;
	return cID++;
}
template <typename T>
inline ComponentID getComponentTypeID() noexcept
{
	static_assert(std::is_base_of<Component, T>::value,
		"T must must inherit from Component");
	static ComponentID id = getUniqueComponentTypeID();
	return id;
}

constexpr static  ComponentID maxComponents = 32;
constexpr static GroupID maxGroups = 32;
using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;
enum GroupLabels : std::size_t;// must define



class Component
{

public:
	virtual ~Component() {}
	GameObject* owner = nullptr;
	virtual void Update(const float& dt) {}
	virtual void Init() {};
	virtual void Draw() {};
	virtual void Fade(const float& dt) {}
};

class GameObject
{
protected:
	ComponentBitSet m_componentBitset;
	ComponentArray m_componentArray;
	GroupBitSet m_groupBitSet;
	std::vector<std::unique_ptr<Component>> m_components;
	bool m_active = true;
	
	
public:
	GameObject(){};
	virtual ~GameObject() {}
	ECS_Manager* manager;
	// Virtual functions
	virtual void Draw();
	virtual void Update(const float& dt);
	virtual void DoTranslation(const Vec2f& offset);
	//	
	bool Alive()const;
	void Destroy();
	virtual void Init() {};
	template <typename Type>
	void Remove()
	{
		// cannot remove transform
		if (std::is_same<Type, Transform>::value)
			return;

		bool result = Has<Type>();
		if (result)
		{
			m_components.erase(std::remove_if(std::begin(m_components), std::end(m_components),
				[=](const std::unique_ptr<Component>& comp)
			{
				return comp.get() == m_componentArray[getComponentTypeID<Type>()];
			})
				, std::end(m_components));
			m_componentArray[getComponentTypeID<Type>()] = nullptr;
			m_componentBitset[getComponentTypeID<Type>()] = false;
		}
	}
	template <typename Type>
	bool Has()const
	{
		return m_componentBitset[getComponentTypeID<Type>()];
	}

	template <typename Type, typename... TArgs>
	Type& Add(TArgs&&... mArgs)
	{
		assert(!Has<Type>());
		Type* comp(new Type(std::forward<TArgs>(mArgs)...));
		std::unique_ptr<Component> uPtr{ comp };
		m_components.emplace_back(std::move(uPtr));
		m_componentArray[getComponentTypeID<Type>()] = comp;
		m_componentBitset[getComponentTypeID<Type>()] = true;
		comp->owner = this;
		comp->Init();

		return *comp;
	}

	template<typename Type>
	Type& Get()const
	{

		auto ptr(m_componentArray[getComponentTypeID<Type>()]);
		return *static_cast<Type*>(ptr);
	}

	bool HasGroup(const GroupID& id)const;
	void AddGroup(const GroupID& id);
	void RemoveGroup(const GroupID& id);
	Vec2f Center()const;
};




using GameObjectGroup = std::array<std::vector<GameObject*>, maxGroups>;

enum GroupLabels : std::size_t
{
	groupRender,
	groupCollider,
	groupMap,
	groupLevel,
	groupBackGround,
	groupParticle,
	groupEmitters,
	groupLayerFront,
	groupLayerBack,
	numberOfGroupLabels,
	groupNone
};




class ECS_Manager
{
	friend class GameObject;
	std::vector<std::unique_ptr<GameObject>> objects;
	std::map<std::size_t, std::vector<GameObject*>> groupedByType;
	GameObjectGroup groupedByAction;
private:
	template <typename T>
	auto GetID() const noexcept
	{
		return typeid(T).hash_code();
	}
	template <typename TContainer>
	void CleanUpDestroyed(TContainer& cont)
	{
		cont.erase(std::remove_if(std::begin(cont), std::end(cont),
			[](const auto& p)
		{
			return !p->Alive();
		}),std::end(cont));
	}
public:
	template <typename TContainer>
	void CleanUpGroups(TContainer& cont,const std::size_t& id)
	{
		cont.erase(std::remove_if(std::begin(cont), std::end(cont),
			[id](const auto& p)
		{
			return !p->Alive() || !p->HasGroup(id);
		}), std::end(cont));
	}

public:
	ECS_Manager();
	virtual ~ECS_Manager();

	class MapTile* GetTile(const Vec2i& location);
	std::vector<MapTile*> GetMapPartition(const Vec2i& location, const int& objectCount);
	std::vector<class Collider*> GetMapColliders(const Vec2i& location);
	std::vector<class Collider*> GetMapColliders(const Vec2i& startIndicies, const Vec2i& endIndices);
	std::vector<class Collider*> GetVerticalMapColliders(const Vec2i& location, const int& dir);
	std::vector<class Collider*> GetHorizontalMapColliders(const Vec2i& location, const int& dir);
	template <typename T, typename... TArgs>
	T& AddObject(TArgs&&... mArgs)
	{
		
		T* ent(new T(std::forward<TArgs>(mArgs)...));
		std::unique_ptr<T> uPtr{ ent };
		objects.emplace_back(std::move(uPtr));
	    ent->manager = this;
		ent->Init();
		groupedByType[GetID<T>()].emplace_back(ent);
		return *ent;
	}
	template <typename T, typename TPredicate>
	void ForAllOfType(const TPredicate&& mFunction)
	{
		auto& vec(groupedByType[GetID<T>()]);

		for (auto i(0u); i < vec.size(); ++i)
			mFunction(static_cast<T&>(*vec[i]));
	}
	virtual void AddToGroup(GameObject* ent, const GroupID& id)
	{
		
		groupedByAction[id].emplace_back(ent);
	}
	virtual void Refresh()
	{
		for (auto i(0u); i < maxGroups; i++)
		{
			auto& v(groupedByAction[i]);
			CleanUpGroups(v, i);
		}
		for (auto& pair : groupedByType) 
			CleanUpDestroyed(pair.second);

		CleanUpDestroyed(objects);
	}
	virtual void Draw()
	{
		// draw background first
		for (auto& it : groupedByAction[groupBackGround])
			it->Draw();
		// TODO : layers
		for (auto& it : groupedByAction[groupRender])
			it->Draw();
	}
	virtual void Update(const float& dt)
	{
		// prepare for culling 
		groupedByAction[groupRender].clear();
		for (auto& it : objects)
		{
			if (it->HasGroup(groupBackGround))
			{
				it->Update(dt);
			}

			if (it->HasGroup(groupMap) || it->HasGroup(groupParticle))
				continue;
			it->Update(dt);
		}
	}
	std::vector<GameObject*> GetGroupAction(const GroupID& id);
};

class Player;
class PlayerState : public Component
{
protected:
	PlayerState* pPrevious;
	
public:
	
	virtual void SetPrevious(PlayerState* state)
	{pPrevious = state;};
	PlayerState* GetPrevious();
	virtual void Transition(std::size_t index) {};
	virtual void Transition(PlayerState* state) {};
	virtual void CheckMapCollision(class Collider* collider) = 0;
};

