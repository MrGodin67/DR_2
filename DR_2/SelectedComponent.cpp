#include "stdafx.h"
#include "SelectedComponent.h"
#include "Locator.h"
void SelectedRect::Update(const float & dt)
{
	meterPts[0] = { transform->Boundary().left + 4.0f,transform->Boundary().top };
	meterPts[1] = { transform->Boundary().right - 4.0f,transform->Boundary().top };
}

void SelectedRect::Draw()
{
	if (selected)
	{
		Locator::Gfx()->DrawRectangle(matTranslation, transform->Boundary().ToD2D(),
			D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f));

		Locator::Gfx()->DrawLine(matTranslation, { meterPts[0].x,meterPts[0].y },
		{ meterPts[1].x,meterPts[1].y }, D2D1::ColorF(D2D1::ColorF::Blue),4.0f);
	}
}

void SelectedRect::Init()
{
	transform = &owner->Get<Transform>();
	
	meterPts[0] = { transform->Boundary().left + 4.0f,transform->Boundary().top };
	meterPts[1] = { transform->Boundary().right - 4.0f,transform->Boundary().top };

	width = meterPts[1].x - meterPts[0].x;
}

void SelectedRect::Translate(const Vec2f & offset)
{
	matTranslation = D2D1::Matrix3x2F::Translation(offset.x, offset.y);
}
