#include "stdafx.h"
#include "Game.h"
#include "Direct3DWindow.h"
#include "Components.h"
#include "MapTile.h"
#include "Animation.h"
#include "Collider.h"
#include "Animation.h"
static const vstring mapFilenames = 
{
	{"map1.txt"},
	{"assets\\plat1.txt"}
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
	
	
		
	m_EntityMgr = std::make_unique<ECS_Manager>();
	
	LoadLevel(1);
	LoadAudio();
	

	InitializePLayer();
	background1 = &m_EntityMgr->AddObject<BackGroundLayer>(Vec2f(0.0f, 0.0f), 0.0f, L"assets\\back1.png");
	background2 = &m_EntityMgr->AddObject<BackGroundLayer>(Vec2f(0.0f, 500.0f - 128.0f ), 0.0f, L"assets\\back2.png");
	m_particle = std::make_unique<D2D1Texture>(Locator::D2DRenderTarget(), L"assets\\particle.png");
	
	Emitter::EmitterDesc desc;
	desc.destroyWhenDone = false;
	desc.interval = 0.5f;
	desc.position = Vec2f(400.0f, 600.f);
	desc.timer = 0.0f;
	
	emit = &m_EntityMgr->AddObject<Emitter>(desc);
	Animation::Sequence seq;
	seq.current_index = 0llu;
	seq.frameDelay = 0.0f;
	seq.image = m_particle->GetBitmap();
	seq.srcRects.push_back(RectF(0.0f, 0.0f, 256.0f, 256.0f));
	seq.timer = 0.0f;
	
	for (int c = 0; c < 46; c++)
	{
		emit->AddPartical<Particle>(Vec2f(400.0f, 600.f), Vec2f(0.0f, 0.0f), Vec2f(64.0f, 64.0f),
			seq, 8.0f, true, true);
	}
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
	
	if (FAILED(hr = ConstructScene(deltaTime))) 
	{ return false; }
	if (FAILED(hr = RenderScene())) { return false; }
	return true;
}

HRESULT Game::ConstructScene(const float& deltaTime)
{
	// get user input
	HandleInput();
	m_pPlayer->Update(deltaTime);
	m_cam.SetFocusPoint(m_pPlayer->Get<Transform>().Center());
	m_pPlayer->Get<Transform>().Translate(-m_cam.GetPosition());
	background2->Translate(Vec2f(-m_cam.GetPosition().x*0.15f, 0.0f));
	
	
	// update physics
	if ((m_pPlayer->Center() - emit->GetPosition()).Len() < 50.0f)
		emit->Start();
	else
		emit->Stop();
	m_EntityMgr->Update(deltaTime);
	emit->Update(deltaTime);
	// handle physics results
	HandleMap();
	DoCollisions();
	//HandleUnits();
	//~
	// refresh objects and groups, removes dead or inactive ect.. 
	m_EntityMgr->Refresh();
	m_cam.Update(deltaTime);
	emit->DoTransform(-m_cam.GetPosition());
	m_soundFX->PlayQueue();

	return S_OK;
}

HRESULT Game::RenderScene()
{
	HRESULT hr;
	
	hr = gfx.BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	if (FAILED(hr)) { return hr; }
	
	m_vpMain.BeginScene();
	//DrawBackground();
	m_EntityMgr->Draw();
	 m_pPlayer->Draw();
	 emit->Draw();
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
	
	
	std::string fileName = mapFilenames[index];

	MapFileParser parser;
	LayerData data = parser(fileName);
	SafeDelete(&m_currentArena);

	m_currentArena = &m_EntityMgr->AddObject<Arena>(data);
	
	 m_currentArena->Create(m_EntityMgr);
	
	m_cam.ConfineToMap(RectF(0.0f, 0.0f, (float)(data.width * data.drawWidth), (float)(data.height * data.drawHeight)));
	m_cam.SetFocusPoint(Vec2f(600.0f,800.0f));
	int y = 0;
}

void Game::InitializePLayer()
{
	m_moveImage = std::make_unique<D2D1Texture>(Locator::D2DRenderTarget(), L"assets\\robo64x64.png");
	assert(m_moveImage->GetBitmap());
	m_moveImage2 = std::make_unique<D2D1Texture>(Locator::D2DRenderTarget(), L"assets\\robo2_64x64.png");
	assert(m_moveImage2->GetBitmap());


	Vec2f size = { 32.0f,42.0f };
	Vec2f position = { 100.0f,100.0f };
	m_pPlayer = (Player*)&m_EntityMgr->AddObject<Player>(position,size);
	m_pPlayer->Get<Transform>().acceleration = 20.20f;
	m_pPlayer->Get<Transform>().friction = 0.920f;
	m_pPlayer->Add<Collider>(position + (size * 0.5f), size*0.5f);
	m_pPlayer->Add<Input>(window.kbd, window.mouse);
	m_pPlayer->AddGroup(groupMap);


	Animation* an = &m_pPlayer->Get<Animation>();
	Animation::Sequence seq;
	seq.current_index = 0;
	seq.frameDelay = 0.080f;
	seq.image = m_moveImage->GetBitmap();
	seq.srcRects.push_back(RectF(0.0f, 0.0f, 64.0f, 64.0f));
	an->AddSequence("right_idle", seq);


	seq.srcRects.clear();
	seq.srcRects.push_back(RectF(64.0f, 0.0f, 128.0f, 64.0f));
	seq.srcRects.push_back(RectF(128.0f, 0.0f, 192.0f, 64.0f));
	seq.srcRects.push_back(RectF(192.0f, 0.0f, 256.0f, 64.0f));
	an->AddSequence("right_walk", seq);


	seq.srcRects.clear();
	seq.srcRects.push_back(RectF(192.0f, 128.0f, 256.0f, 192.0f));
	an->AddSequence("left_idle", seq);

	seq.srcRects.clear();
	seq.srcRects.push_back(RectF(128.0f, 128.0f, 192.0f, 192.0f));
	seq.srcRects.push_back(RectF(64.0f, 128.0f, 128.0f, 192.0f));
	seq.srcRects.push_back(RectF(0.0f, 128.0f, 64.0f, 192.0f));
	an->AddSequence("left_walk", seq);

	seq.srcRects.clear();
	seq.image = m_moveImage2->GetBitmap();
	seq.srcRects.push_back(RectF(0.0f, 0.0f, 64.0f, 64.0f));
	an->AddSequence("left_jump", seq);

	seq.srcRects.clear();
	seq.image = m_moveImage2->GetBitmap();
	seq.srcRects.push_back(RectF(128.0f, 64.0f, 192.0f, 128.0f));
	an->AddSequence("right_jump", seq);
}

void Game::HandleMap()
{
	m_colliders.clear();
	m_EntityMgr->ForAllOfType<MapTile>([this](auto& obj)
	{

		if (obj.Get<Transform>().Boundary().Overlaps(m_cam.GetViewFrame()))
		{

			obj.AddGroup(groupRender);
			obj.Get<Transform>().Translate(-m_cam.GetPosition());
			m_colliders.push_back(&obj.Get<Collider>());
		}
		else
		{
			obj.RemoveGroup(groupRender);
		}

	});
}

void Game::HandleInput()
{

	
	

	
	Mouse::Event mouseEvent = window.mouse.Read();
	if (mouseEvent.GetType() == Mouse::Event::RRelease)
	{
		
	}
	if (mouseEvent.GetType() == Mouse::Event::LRelease)
	{
		
	}
	if (mouseEvent.GetType() == Mouse::Event::LPress && window.kbd.KeyIsPressed(VK_CONTROL))
	{
	
		
	}
	
	HandleMultiSelectedUnits();
		
	
}

void Game::DoCollisions()
{

	for (auto& it : m_colliders)
		m_pPlayer->ResolveCollision(it);
}
	


void Game::HandleMultiSelectedUnits()
{
	
	

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

void Game::DrawBackground()
{
	gfx.DrawSprite(D2D1::Matrix3x2F::Identity(), m_vpMain.GetViewRect().ToD2D(),
		backGround->GetBitmap());

	//gfx.DrawFilledRectangle(D2D1::Matrix3x2F::Identity(), m_vpMain.GetViewRect().ToD2D(),
	//	D2D1::ColorF(.85f, .85f, .85f, 0.45f));

}

void Game::GetSelectedUnit(const Vec2f & pos)
{
	
	
	

}




