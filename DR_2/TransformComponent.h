#pragma once

#include "ECS_Manager.h"
#include "Vec2.h"
#include "Rect.h"
class Transform :
	public Component
{
	
	Vec2f center;
	Vec2f scale = {1.0f,1.0f};
	Vec2f size;
	D2D1_MATRIX_3X2_F matRotation = D2D1::Matrix3x2F::Identity();
	D2D1_MATRIX_3X2_F matScaling = D2D1::Matrix3x2F::Identity();
	D2D1_MATRIX_3X2_F matTranslation = D2D1::Matrix3x2F::Identity();
	
public:
	Transform() = default;
	Transform(const Vec2f& position,const Vec2f& size);
	~Transform();
	void Update(const float& dt)override;
	void Init()override;
	void Rotate();
	void Scale(const Vec2f& dimensions);
	void Translate(const Vec2f& offset);
	D2D1_MATRIX_3X2_F Matrix()const;
	RectF Boundary()const;
	Vec2f Center()const;

public:
	Vec2f position = { 0.0f , 0.0f };
	Vec2f velocity = { 0.0001f , 0.0001f };
	float rotation_angle = 0.0f;
	float acceleration = 0.0f;
	float friction = 0.98f;
	Vec2f maxVelocity = {100.0f,100.0f};
	
};

