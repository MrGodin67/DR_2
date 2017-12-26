#include "stdafx.h"
#include "TransformComponent.h"



Transform::Transform( const Vec2f& position, const Vec2f& size)
	:position(position),size(size)
{
}


Transform::~Transform()
{
}

void Transform::Update(const float & dt)
{
	position += velocity * acceleration * dt;
	center =  position + (size * 0.5f) ;
}

void Transform::Init()
{
	center = position + (size * 0.5f);
}

void Transform::Rotate()
{
	matRotation = D2D1::Matrix3x2F::Rotation(rotation_angle, { center.x,center.y });
}

void Transform::Scale(const Vec2f & dimensions)
{
	
	matScaling = D2D1::Matrix3x2F::Scale({ dimensions.x, dimensions.y }, {center.x,center.y});
}

void Transform::Translate(const Vec2f & offset)
{
	matTranslation = D2D1::Matrix3x2F::Translation({ offset.x,offset.y });
}

D2D1_MATRIX_3X2_F Transform::Matrix()const
{
	return matRotation * matScaling * matTranslation;
}

RectF Transform::Boundary() const
{
	return{position.x,position.y,position.x + size.width,position.y + size.height};
}

Vec2f Transform::Center() const
{
	return center;
}
