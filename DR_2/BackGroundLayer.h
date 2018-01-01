#pragma once
#include "ECS_Manager.h"
#include "Components.h"
#include "TransformComponent.h"
#include "Animation.h"
#include "D2D1Texture.h"
class BackGroundLayer :
	public GameObject
{
	float scrollSpeed;
	std::unique_ptr<D2D1Texture> image;

public:
	BackGroundLayer(const Vec2f& anchor,const float& scrollSpeed,const std::wstring& fileName);
	~BackGroundLayer();
	void Draw()override;
	void Init()override;
	void Update(const float& dt)override;
	void SetScroll(const float & speed);
	void Translate(const Vec2f& offset);
};

