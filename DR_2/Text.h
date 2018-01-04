#pragma once
#include "ECS_Manager.h"
class Text :
	public GameObject
{
	IDWriteTextFormat* pFormat = nullptr;
	std::wstring text;
	D2D1_COLOR_F color = D2D1::ColorF(1.0f,1.0f,1.0f,1.0f);
public:
	Text(const Vec2f& pos, const Vec2f size);
	~Text();
	void SetText(const std::wstring& txt);
	void SetFormat(IDWriteTextFormat* txtFormat);
	void SetColor(const float& r, const float& g, const float& b, const float& a);
	void SetColor(const D2D1_COLOR_F& c);
	void Init()override;
	void Draw()override;

};

