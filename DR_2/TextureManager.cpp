#include "stdafx.h"
#include "TextureManager.h"


TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}

ID2D1Bitmap * TextureManager::GetImage(const std::string & id)
{
	assert(images[id]->GetBitmap());
	return images[id]->GetBitmap();
}

ID2D1Bitmap * TextureManager::AddImage(ID2D1RenderTarget * rt, const std::string& id, const std::wstring& filePath)
{
	if (images[id].get())
		return images[id]->GetBitmap();
	images[id] = std::make_unique<D2D1Texture>(rt, filePath);
	assert(images[id]->GetBitmap());
	return images[id]->GetBitmap();
}
