#include "stdafx.h"
#include "Animation.h"
#include "Locator.h"

Animation::Animation()
{
}


Animation::~Animation()
{
}

void Animation::AddSequence(const std::string name, const Sequence & seq)
{
	sequences[name] = seq;
	// default to last seq
	currentSequence = &sequences[name];
}

void Animation::StartSequenceByName(const std::string & name)
{
	currentSequence = &sequences[name];
	
}

void Animation::Draw()
{
	Locator::Gfx()->DrawSprite(transform->Matrix(),
		transform->Boundary().ToD2D(), currentSequence->image, 
		&currentSequence->GetCurrentSrcRect().ToD2D());

}

void Animation::Update(const float & dt)
{
	// if only one frame, don't do sequence
	if (sequences.size() > 1)
	{
		if ((currentSequence->timer += dt) > currentSequence->frameDelay)
		{
			index++;
			currentSequence->current_index = index % currentSequence->srcRects.size();
			currentSequence->timer = 0.0f;
		}

	}
}

void Animation::Init()
{
	transform = &owner->Get<Transform>();
}

Animation::Sequence * Animation::CurrentSequence()
{
	return currentSequence;
}

