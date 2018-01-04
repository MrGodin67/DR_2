#pragma once
#include "D2D1Texture.h"
class TextureManager
{
	std::unordered_map < std::string, std::unique_ptr<D2D1Texture>> images;
public:
	TextureManager();
	~TextureManager();
	ID2D1Bitmap* AddImage(ID2D1RenderTarget* rt, const std::string& id, const std::wstring& filePath);
	ID2D1Bitmap* GetImage(const std::string& id);
};

