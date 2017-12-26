#pragma once
#include "Vec2.h"
#include "Rect.h"
#include "Locator.h"
class ViewPort
{
	RectF m_view;
	ID2D1Layer *pLayer = nullptr;
	ID2D1SolidColorBrush* brush = nullptr;
	float m_transparency = 1.0f;
public:
	
	ViewPort(const RectF& position);
	~ViewPort();

	float Width();
	float Height();
	RectF GetViewRect();
	Vec2f GetViewTopLeft();
	void BeginScene();
	void EndScene();
	void ResetTransparency();

};