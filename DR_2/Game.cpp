#include "stdafx.h"
#include "Game.h"
#include "Direct3DWindow.h"
#include "Components.h"
#include "MapTile.h"
#include "Unit.h"
#include "SelectedComponent.h"
#include "newAStar.h"
static const vstring mapFilenames = 
{
	{"map1.txt"},
	{"map2.txt"}
};

Game::Game(Direct3DWindow & wnd)
	:
	window(wnd),
	gfx(wnd.ScreenWidth(),wnd.ScreenHeight(),wnd.WindowHandle(),
		true, FULL_SCREEN,1000.0f,0.01f),
	m_vpMain(RectF(0.0f, 0.0f, 800, 500)),
	m_vpUI(RectF(0.0f, 500.0f, 800, 600)),
	m_cam(m_vpMain.Width(), m_vpMain.Height())
{
	Locator::SetGraphics(&gfx);
	m_entityTileSet = MakeUnique<D2D1Texture>(Locator::D2DRenderTarget(),L"assets\\playerunits.png");
	
		
	m_EntityMgr = std::make_unique<ECS_Manager>();
	m_pathMgr = MakeUnique<PathfindingManager>();
	Locator::SetPathFinding(m_pathMgr.get());
	LoadLevel(0);
	LoadAudio();
	float x = 37.5f;
	float y = 37.5f;
	int inc = 0;
	for (int c = 0; c < 12; c++)
	{
		inc = c % 4;
		testUnits.push_back(&m_EntityMgr->AddObject<Unit>(Vec2f(x * (2 + inc), y * (2 + inc)), Vec2f(24.0f, 24.0f),
			RectF(inc * 64.0f, 0.0f, (inc + 1) * 64.0f, 64.0f), m_entityTileSet->GetBitmap()));
		testUnits.back()->Get<Transform>().acceleration = 100.0f;
		testUnits.back()->UpdateTile();
		 x += 2.5f;
	}
	currentUnit = testUnits[0];
	currentUnit->Get<SelectedRect>().selected = true;

	groupSelector = &m_EntityMgr->AddObject<GroupSelector>();
}

Game::~Game()
{
	SafeDelete(&backGround);
	SafeDelete(&sidePanel);
	SafeDelete(&bases);
}

bool Game::Play(const float& deltaTime)
{
	HRESULT hr;
	// check if any queued requests are done
	m_pathMgr->checkForDone();
	if (FAILED(hr = ConstructScene(deltaTime))) 
	{ return false; }
	if (FAILED(hr = RenderScene())) { return false; }
	return true;
}

HRESULT Game::ConstructScene(const float& deltaTime)
{
	// get user input
	//if (currentUnit)
		//m_cam.UpdatePosition(currentUnit->Get<Transform>().Center());

	
	HandleInput();
	// update physics
	m_EntityMgr->Update(deltaTime);
	// handle physics results
	HandleMap();
	HandleUnits();
	//~
	// refresh objects and groups, removes dead or inactive ect.. 
	m_EntityMgr->Refresh();
	
	
	return S_OK;
}

HRESULT Game::RenderScene()
{
	HRESULT hr;
	m_soundFX->PlayQueue();
	hr = gfx.BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	if (FAILED(hr)) { return hr; }
	
	m_vpMain.BeginScene();
	 m_EntityMgr->Draw();
	m_vpMain.EndScene();

	m_vpUI.BeginScene();
	// TODO render UI controls here
	m_vpUI.EndScene();
	hr = gfx.EndScene();
	if (FAILED(hr)) { return hr; }
	return hr;
}

void Game::EndApp()
{
	PostQuitMessage(0);
}

void Game::LoadAudio()
{
	m_soundFX = std::make_unique<SoundManager>();
	m_soundFX->AddSound("buildcompleted", L"assets\\media\\constructioncompleted.wav");
	m_soundFX->AddSound("newunitavaliable", L"assets\\media\\newunit.wav");
	m_soundFX->AddSound("buildstarted", L"assets\\media\\constructionstarted.wav");
	m_soundFX->AddSound("newfacility", L"assets\\media\\newfacility.wav");
	Locator::SetSoundManager(m_soundFX.get());

}

void Game::LoadLevel(const std::size_t& index)
{
	backGround = new D2D1Texture(Locator::D2DRenderTarget(), L"assets\\back1.png");
	assert(backGround->GetBitmap());
	sidePanel = new D2D1Texture(Locator::D2DRenderTarget(), L"assets\\side.png");
	assert(sidePanel->GetBitmap());
	
	std::string fileName = mapFilenames[index];

	MapFileParser parser;
	LayerData data = parser(fileName);
	SafeDelete(&m_currentArena);

	m_currentArena = &m_EntityMgr->AddObject<Arena>(data);
	
	vint mapData = m_currentArena->Create(m_EntityMgr);
	m_pathMgr->Initialize(mapData, Vec2i(data.width, data.height), Vec2f((float)data.cellWidth,(float)data.cellHeight));
	
	m_cam.ConfineToMap(RectF(0.0f, 0.0f, data.width * data.cellWidth, data.height * data.cellHeight));
	m_cam.UpdatePosition(Vec2f(0.0f, 0.0f));
	int y = 0;
}

void Game::HandleMap()
{
	m_EntityMgr->ForAllOfType<MapTile>([this](auto& obj)
	{

		if (obj.Get<Transform>().Boundary().Overlaps(m_cam.GetViewFrame()))
		{

			obj.AddGroup(groupRender);
			obj.Get<Transform>().Translate(-m_cam.GetPos());
		}
		else
		{
			obj.RemoveGroup(groupRender);
		}

	});
}

void Game::HandleInput()
{

	Vec2f vel = { 0.0f,0.0f };
	if (window.kbd.KeyIsPressed('W'))
		vel.y = -10.0f;
	if (window.kbd.KeyIsPressed('S'))
		vel.y = 10.0f;
	if (window.kbd.KeyIsPressed('A'))
		vel.x = -10.0f;
	if (window.kbd.KeyIsPressed('D'))
		vel.x = 10.0f;
	m_cam.Scroll(vel);




	Mouse::Event mouseEvent = window.mouse.Read();
	if (mouseEvent.GetType() == Mouse::Event::RRelease)
	{
		if (currentUnit)
		{
			currentUnit->currentTile->Passable(true);
			m_pathMgr->requestPath(currentUnit->Get<Transform>().position, MouseWorldSpace(), currentUnit);
			
		}
		if(groupSelector->HasSelectedUnits())
		{
			Vec2f mousePosInWorld = MouseWorldSpace();
			MapTile* tile = m_EntityMgr->GetTile(ConvertToTileLocation(mousePosInWorld));
			
			if (tile)
			{
				if (!tile->Passable())
					return;
				auto& vec = groupSelector->SelectedUnits();
				std::sort(vec.begin(),vec.end(),
					[mousePosInWorld](auto& a, auto& b)
				{
					return (mousePosInWorld - a->Center()).LenSq() <
						(mousePosInWorld - b->Center()).LenSq();
				});
				auto& list = m_EntityMgr->GetMapPartition(tile->MapLocation(), (int)vec.size());
				for (int i : Iterate(0, (int)vec.size()))
				{
					
					Unit* unit = vec[i];
					unit->currentTile->Passable(true);
				    m_pathMgr->requestPath(unit->Center(), 
						list[i]->Get<Transform>().Center(), unit);
				}
			}
		}
	}
	if (mouseEvent.GetType() == Mouse::Event::LRelease)
	{
		if (groupSelector->Active())
		{
			groupSelector->End();
		}
		else
		{
			GetSelectedUnit(MouseWorldSpace());
		}
	}
	if (mouseEvent.GetType() == Mouse::Event::LPress && window.kbd.KeyIsPressed(VK_CONTROL))
	{
		groupSelector->Start(MouseWorldSpace());
		
	}
	
	HandleMultiSelectedUnits();
		
	
}

void Game::HandleUnits()
{
	m_EntityMgr->ForAllOfType<Unit>([this](auto& obj)
	{
		if (!m_pathMgr->PendingPaths())
		{
			obj.currentTile->Passable(obj.Moving());
		}
	  if (obj.Get<Transform>().Boundary().Overlaps(m_cam.GetViewFrame()))
		{

			obj.AddGroup(groupRender);
			obj.Get<Transform>().Translate(-m_cam.GetPos());
			obj.Get<SelectedRect>().Translate(-m_cam.GetPos());
		}
		else
		{
			obj.RemoveGroup(groupRender);
		}

	});
	m_EntityMgr->ForAllOfType<GroupSelector>([this](auto& obj)
	{
		obj.Get<Transform>().Translate(-m_cam.GetPos());
		obj.AddGroup(groupRender);
	});
}

void Game::HandleMultiSelectedUnits()
{
	if (groupSelector->Active())
	{
		groupSelector->Drag(MouseWorldSpace());
	}
	else
	{
		if (groupSelector->SelectedUnits().size() > 0)
		{
			//currentUnit->Get<SelectedRect>().selected = false;
			currentUnit = 0;
			if (groupSelector->SelectedUnits().size() == 1)
			{
				currentUnit = groupSelector->SelectedUnits()[0];
				groupSelector->FlushSelected();
				currentUnit->Get<SelectedRect>().selected = true;
			
				return;
			}
			
		}
	}

}

Vec2i Game::ConvertToTileLocation(const Vec2f & worldPos)
{
	Vec2f pos = worldPos;
	pos.x = (pos.x / m_currentArena->GetData().cellWidth);
	pos.y = (pos.y / m_currentArena->GetData().cellHeight);
	return Vec2i(pos);
}

Vec2f Game::MouseWorldSpace()
{
	return m_cam.ConvertToWorldSpace(Vec2f((float)window.mouse.GetPosX(), (float)window.mouse.GetPosY()));
}

void Game::GetSelectedUnit(const Vec2f & pos)
{
	
	m_EntityMgr->ForAllOfType<Unit>([this,pos](auto& obj)
	{

		if (obj.Get<Transform>().Boundary().Contains(pos))
		{
			groupSelector->FlushSelected();
			if (!obj.Get<SelectedRect>().selected)
			{
				if(currentUnit)
				   currentUnit->Get<SelectedRect>().selected = false;
				obj.Get<SelectedRect>().selected = true;
				currentUnit = &obj;
				
				return;
			}
		}
		

	});
	

}




