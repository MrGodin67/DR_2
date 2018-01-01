#include "stdafx.h"
#include "Locator.h"

ID2D1RenderTarget* Locator::renderTarget = nullptr;
SoundManager* Locator::soundEngine = nullptr;
TextHandler* Locator::textManager = nullptr;
ID2D1SolidColorBrush* Locator::solidBrush = nullptr;
Graphics* Locator::gfx = nullptr;

void Locator::SetGraphics(Graphics * g)
{
	gfx = g;
}
void Locator::SetSolidBrush(ID2D1SolidColorBrush * b)
{
	solidBrush = b;
}

void Locator::SetD2DRenderTarget(ID2D1RenderTarget* rt)
{
	renderTarget = rt;
}

void Locator::SetSoundManager(SoundManager * sm)
{
	soundEngine = sm;
}

void Locator::SetTextManager(TextHandler * tx)
{
	textManager = tx;
}



ID2D1RenderTarget * Locator::D2DRenderTarget()
{
	return renderTarget;
}

ID2D1SolidColorBrush * Locator::SolidBrush()
{
	return solidBrush;
}

SoundManager * Locator::Audio()
{
	return soundEngine;
}

Graphics * Locator::Gfx()
{
	return gfx;
}






