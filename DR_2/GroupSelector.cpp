#include "stdafx.h"
#include "GroupSelector.h"
#include "SelectedComponent.h"
#include "Locator.h"
#include "Unit.h"
#include "newAStar.h"
GroupSelector::GroupSelector()
{
}


GroupSelector::~GroupSelector()
{
}

void GroupSelector::Init()
{
	Add<Transform>();
	AddGroup(groupMap);
}

void GroupSelector::Draw()
{
	if (dragging)
	{
		Locator::Gfx()->DrawRectangle(Get<Transform>().Matrix(), frame.ToD2D(),
			D2D1::ColorF(0.0f, 1.0f, 0.0f, 1.0f));
	}
}

void GroupSelector::Start(const Vec2f & initialPt)
{
	selectedUnits.clear();
	Get<Transform>().position =  startPos = initialPt;
	dragging = true;
}

void GroupSelector::Drag(const Vec2f & currentPt)
{
	if (dragging)
	{
		frame = RectF(startPos, currentPt);
	}
	

}

void GroupSelector::End()
{
	dragging = false;
	manager->ForAllOfType<Unit>([this](auto& obj)
	{
		if (obj.Get<Transform>().Boundary().Overlaps(frame))
		{
			//obj.currentTile->Passable(true);
			obj.Get<SelectedRect>().selected = true;
			selectedUnits.push_back(&obj);
		}
		else
		{
			obj.Get<SelectedRect>().selected = false;
		}
	});
}

bool GroupSelector::Active() const
{
	return dragging;
}

std::vector<Unit*>& GroupSelector::SelectedUnits()
{
	return selectedUnits;
}

void GroupSelector::FlushSelected()
{
	for (int i : Iterate(0, selectedUnits.size()))
		selectedUnits[i]->Get<SelectedRect>().selected = false;
	selectedUnits.clear();
}

bool GroupSelector::HasSelectedUnits()
{
	return selectedUnits.size() > 1;
}
