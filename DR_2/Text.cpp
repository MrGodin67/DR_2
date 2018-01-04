#include "stdafx.h"
#include "Text.h"
#include "Components.h"
#include "Locator.h"

Text::Text(const Vec2f& pos, const Vec2f size)
{
	Add<Transform>(pos, size);
}


Text::~Text()
{
}

void Text::SetText(const std::wstring & txt)
{
	text = txt;
}

void Text::SetFormat(IDWriteTextFormat * txtFormat)
{
	pFormat = txtFormat;
}

void Text::SetColor(const float & r, const float & g, const float & b, const float & a)
{
	color = D2D1::ColorF(r, g, b, a);
}

void Text::SetColor(const D2D1_COLOR_F & c)
{
	color = c;
}

void Text::Init()
{
	AddGroup(groupUI);
}

void Text::Draw()
{
	Locator::Gfx()->RenderText(Get<Transform>().Matrix(),text, pFormat, Get<Transform>().Boundary().ToD2D(), color);
}
