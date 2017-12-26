#pragma once
#include "Graphics.h"
#include "Camera.h"
#include "SoundManager.h"
#include "ViewPortMain.h"
#include "MapFileParser.h"
#include "Arena.h"
#include "ECS_Manager.h"
#include "PathManager.h"
#include "Unit.h"
#include "GroupSelector.h"
enum  VIEWPORTS : int
{
	vp_MAIN,
	vp_UI,
	vp_count
};
class Game
{
	
	class Direct3DWindow& window;
	Graphics gfx;
	ViewPort m_vpMain;
	ViewPort m_vpUI;
	Camera m_cam;
	std::unique_ptr<SoundManager> m_soundFX;
	std::unique_ptr<ECS_Manager> m_EntityMgr;
	std::unique_ptr<PathfindingManager> m_pathMgr;
	std::unique_ptr<D2D1Texture> m_entityTileSet;

	Arena* m_currentArena = nullptr;
	D2D1Texture * backGround;
	D2D1Texture * sidePanel;
	D2D1Texture * bases;
	GroupSelector* groupSelector;
	std::vector<Unit*> testUnits;
	Unit* currentUnit = nullptr;
public:
	Game(class Direct3DWindow& wnd);
	~Game();
	bool Play(const float& deltaTime);
	HRESULT ConstructScene(const float& deltaTime);
	HRESULT RenderScene();
	
	////////////////////////////////////
	// LOGIC
	////////////////////////////////////

private:
	void EndApp();
	void LoadAudio();
	void LoadLevel(const std::size_t& index);
	void HandleMap();
	void HandleInput();
	void HandleUnits();
	void GetSelectedUnit(const Vec2f& pos);
	void HandleMultiSelectedUnits();
	Vec2i ConvertToTileLocation(const Vec2f& worldPos);

};
