#include "stdafx.h"
#include "Arena.h"
#include "Locator.h"
#include "MapTile.h"

Arena::Arena(const LayerData& data)
	:data(data)
{
	
	mapTileSet = std::make_unique<D2D1Texture>(Locator::D2DRenderTarget(), std::wstring(data.tileSetFilename));
	assert(mapTileSet->GetBitmap());
	MapTile::SetTileAttributes({ (float)data.drawWidth,(float)data.drawHeight }, mapTileSet->GetBitmap());
	
	
	
}


Arena::~Arena()
{
}

void Arena::Create(std::unique_ptr<ECS_Manager>& manager)
{
	
	const int map_columns = data.width / data.cellWidth;
	const int image_columns = (int)mapTileSet->Width() / data.cellWidth;
	// TODO Number of layers
	
	Vec2f position = { 0.0f,0.0f };

		for (int r = 0; r < data.height; r++)
		{
			position.x = 0.0f;
			for (int c = 0; c < data.width; c++)
			{
				const int index = r * data.width + c;
				const int mapTag = data.layers[0][index];
				
				if (mapTag != -1)
				{
					bool passable = false;
					float x = (float)(mapTag % image_columns) * (float)data.cellWidth;
					float y = (float)(mapTag / image_columns) * (float)data.cellHeight;
					D2D1_RECT_F srcRect = { x,y,x + (float)data.cellWidth, y + (float)data.cellHeight };
					manager->AddObject<MapTile>(position, RectF(srcRect),passable);
					
				}
				else
				{
					// dummy tile
					manager->AddObject<MapTile>(position, RectF(), true);
				}
				position.x += (float)data.drawWidth;
			}
			position.y += (float)data.drawHeight;
		}
		

}

void Arena::Init()
{
	AddGroup(groupLevel);
}
