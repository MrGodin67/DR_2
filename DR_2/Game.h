#pragma once
#include "Graphics.h"
#include "Camera.h"
#include "SoundManager.h"
#include "ViewPortMain.h"
#include "MapFileParser.h"
#include "Arena.h"
#include "ECS_Manager.h"
#include "Player.h"
#include "Collider.h"
#include "BackGroundLayer.h"
#include "Emitter.h"
#include "Particle.h"
#include "Fountain.h"
#include "TextureManager.h"
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
	std::unique_ptr<TextureManager> m_ImageMgr;
	std::unique_ptr<D2D1Texture> m_entityTileSet;
		
	std::vector<Collider*> m_colliders;

	// GameObjects handled by ECS
	Arena* m_currentArena = nullptr;
	Player* m_pPlayer;
	BackGroundLayer* background1;
	BackGroundLayer* background2;
	
	std::vector<Fountain*> fountains;
	Emitter* emit;
	Emitter* emit2;
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
	void InitializePLayer();
	void HandleMap();
	void HandleInput();
	void DoCollisions();
	
	Vec2i ConvertToTileLocation(const Vec2f& worldPos);
	Vec2f MouseWorldSpace();
	void DrawBackground();
	void LoadImages();

};
