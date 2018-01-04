#pragma once
#include "ECS_Manager.h"
#include "Components.h"
class Image :
	public Component
{
	ID2D1Bitmap* image;
	Transform* transform;
public:
	Image( ID2D1Bitmap* image);
	~Image();
	void Init()override;
	void Draw()override;
};

