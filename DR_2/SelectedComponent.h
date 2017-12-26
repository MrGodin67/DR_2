#pragma once

#include "Components.h"
#include "TransformComponent.h"
class SelectedRect : public Component
{
	Transform* transform;
	float width;
	Vec2f meterPts[2];
	D2D1_MATRIX_3X2_F matTranslation = D2D1::Matrix3x2F::Identity();
public:
	SelectedRect() = default;
	void Update(const float& dt)override;
	void Draw()override;
	void Init()override;
	void Translate(const Vec2f& offset);
	bool selected = false;
};