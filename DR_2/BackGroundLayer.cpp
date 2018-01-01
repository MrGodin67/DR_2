#include "stdafx.h"
#include "BackGroundLayer.h"
#include "Locator.h"

BackGroundLayer::BackGroundLayer(const Vec2f& anchor, const float& scrollSpeed, const std::wstring& fileName)
	:scrollSpeed(scrollSpeed),image(std::make_unique<D2D1Texture>(Locator::D2DRenderTarget(),fileName))
{

	assert(image->GetBitmap());
	
	Add<Transform>(anchor, Vec2f( image->GetBitmap()->GetSize().width,image->GetBitmap()->GetSize().height ));
	Add<Animation>();
	Animation::Sequence seq;
	seq.current_index = 0llu;
	seq.frameDelay = 0;
	seq.image = image->GetBitmap();
	seq.srcRects.push_back({ 0.0f,0.0f,800.0f,image->GetBitmap()->GetSize().height });
	Get<Animation>().AddSequence("default", seq);

	
}


BackGroundLayer::~BackGroundLayer()
{
}

void BackGroundLayer::Draw()
{
	Get<Animation>().Draw();
}

void BackGroundLayer::Init()
{
	AddGroup(groupBackGround);
	
}

void BackGroundLayer::Update(const float & dt)
{
	if (scrollSpeed != 0.0f)
	{
		Get<Animation>().CurrentSequence()->srcRects[0].left += scrollSpeed ;
		Get<Animation>().CurrentSequence()->srcRects[0].right += scrollSpeed;
		if (Get<Animation>().CurrentSequence()->srcRects[0].right >=
			Get<Animation>().CurrentSequence()->image->GetSize().width)
		{
			Get<Animation>().CurrentSequence()->srcRects[0].left = 0.0f;
			Get<Animation>().CurrentSequence()->srcRects[0].right = 800.0f;
		}
		if (Get<Animation>().CurrentSequence()->srcRects[0].left < 
			0.0f)
		{
			Get<Animation>().CurrentSequence()->srcRects[0].left = Get<Animation>().CurrentSequence()->image->GetSize().width - 800.0f;
			Get<Animation>().CurrentSequence()->srcRects[0].right = Get<Animation>().CurrentSequence()->image->GetSize().width;
		}
	}
}

void BackGroundLayer::SetScroll(const float & speed)
{
	scrollSpeed = speed;
}

void BackGroundLayer::Translate(const Vec2f & offset)
{
	Get<Transform>().Translate(offset);
}
