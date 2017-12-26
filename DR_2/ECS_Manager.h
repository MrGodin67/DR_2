#pragma once
#include <map>
#include "Vec2.h"

class ECS_Manager;
/*class GameObject
{
	bool alive = true;
public:
	GameObject() = default;
	virtual ~GameObject() {}
	ECS_Manager* manager = nullptr;
	bool Alive()const 
	{
		return alive;
	}
	void Destroy()
	{
		alive = false;
	}
	virtual void Update(const float& dt) {};
	virtual void Draw() {};

};*/





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

};






enum GroupLabels : std::size_t
{
	groupRender,
	groupCollider,
	groupMap,
	groupLevel,
	numberOfGroupLabels,
	groupNone
};




class ECS_Manager
{
	friend class GameObject;
	std::vector<std::unique_ptr<GameObject>> objects;
	std::map<std::size_t, std::vector<GameObject*>> groupedByType;
	std::array<std::vector<GameObject*>, maxGroups> groupedByAction;
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
	void ForAllOfType(TPredicate&& mFunction)
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
		for (auto& it : groupedByAction[groupRender])
			it->Draw();
	}
	virtual void Update(const float& dt)
	{
		// prepare for culling 
		groupedByAction[groupRender].clear();
		for (auto& it : objects)
		{
			if (it->HasGroup(groupMap))
				continue;
			it->Update(dt);
		}
	}
};
/*mgr.forAll<Player>([this](auto& player)
                {
                    // Player-bullet interaction.
                    mgr.forAll<Bullet>([this, &player](auto& bullet)
                        {
                            if(bullet.collidesWith(player))
                            {
                                bullet.kill();
                                player.kill();
                            }
                        });

                    // Player-boss interaction.
                    mgr.forAll<Boss>([this, &player](auto& boss)
                        {
                            const auto& px(player.getPos().x);
                            const auto& bx(boss.getPos().x);

                            boss.setDir(px > bx ? 0.f : ssvu::pi);
                        });

                    // Player firing.
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
                        mgr.emplace<PlayerBullet>(player.getPos());
                });

            mgr.forAll<PlayerBullet>([this](auto& pb)
                {
                    // Player bullet-boss interaction.
                    mgr.forAll<Boss>([this, &pb](auto& boss)
                        {
                            if(pb.collidesWith(boss))
                            {
                                boss.damage(1);
                                pb.kill();
                            }
                        });
                });

mgr.refresh();*/
