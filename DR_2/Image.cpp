#include "stdafx.h"
#include "Image.h"

#include "Locator.h"
Image::Image( ID2D1Bitmap* image )
	:image(image)
{
	
}


Image::~Image()
{
}

void Image::Init()
{
	transform = &owner->Get<Transform>();
}

void Image::Draw()
{
	Locator::Gfx()->DrawSprite(transform->Matrix(), transform->Boundary().ToD2D(),
		image);
}
