#pragma once

#include "SoundManager.h"
#include "DirectWrite.h"
#include "Graphics.h"
class Locator
{
	static SoundManager* soundEngine;
	static ID2D1RenderTarget* renderTarget;
	static TextHandler* textManager;
	static ID2D1SolidColorBrush* solidBrush;
	static Graphics* gfx;
public:
	static void SetGraphics(Graphics* g);
	static void SetSolidBrush(ID2D1SolidColorBrush* b);
	static void SetD2DRenderTarget(ID2D1RenderTarget* rt);
	static void SetSoundManager(SoundManager* sm);
	static void SetTextManager(TextHandler* tx);
	static ID2D1RenderTarget* D2DRenderTarget();
	static ID2D1SolidColorBrush* SolidBrush();
	static SoundManager* Audio();
	static Graphics *Gfx();
	
};