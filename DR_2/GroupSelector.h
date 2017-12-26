#pragma once
#include "ECS_Manager.h"
#include "Rect.h"
#include "Vec2.h"
#include "TransformComponent.h"
class Unit;
class GroupSelector :
	public GameObject
{
	RectF frame;
	Vec2f startPos;
	Vec2f endPos;
	std::vector<Unit*> selectedUnits;
	bool dragging = false;
public:
	GroupSelector();
	~GroupSelector();
	void Init()override;
	void Draw()override;
	void Start(const Vec2f& initialPt);
	void Drag(const Vec2f& currentPt);
	void End();
	bool Active()const;
	std::vector<Unit*>& SelectedUnits();
	void FlushSelected();
	bool HasSelectedUnits();
};

