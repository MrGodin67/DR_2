#pragma once
#include "ECS_Manager.h"
#include "D2D1Texture.h"
#include "MapFileParser.h"
#include "ECS_Manager.h"

////////////////////
// DATA
///////////////////


/////////////////
// OBJECT
////////////////
class Arena : public GameObject
{
	LayerData data = {};
	std::unique_ptr<D2D1Texture> mapTileSet;
	
public:
	Arena(const LayerData& data);
	virtual ~Arena();
	vint Create(std::unique_ptr<ECS_Manager>& manager);
	const LayerData& GetData() { return data; }
	void Init()override;
};

