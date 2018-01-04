#include "stdafx.h"
#include "Game.h"
#include "Direct3DWindow.h"
#include "Components.h"
#include "MapTile.h"
#include "Animation.h"
#include "Collider.h"
#include "Animation.h"
#include "BallParticle.h"
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
	LoadImages();
	
		
	m_EntityMgr = std::make_unique<ECS_Manager>();
	
	LoadLevel(1);
	LoadAudio();
	

	InitializePLayer();
	background1 = &m_EntityMgr->AddObject<BackGroundLayer>(Vec2f(0.0f, 0.0f), 0.0f, L"assets\\back1.png");
	background2 = &m_EntityMgr->AddObject<BackGroundLayer>(Vec2f(0.0f, 500.0f - 128.0f ), 0.0f, L"assets\\back2.png");
	
	
	
	
	
	emit = &m_EntityMgr->AddObject<Emitter>(Vec2f(400.0f, 840.f));
	emit->SetSpawnInterval(0.06f);
	emit->SetRandomVelocityConstrants(Vec2f(-100.0f, 100.0f), Vec2f(-200.0f, -100.0f));
	Animation::Sequence seq;
	seq.current_index = 0llu;
	seq.frameDelay = 0.0f;
	seq.image = Locator::Images()->GetImage("particle_star");
	seq.srcRects.push_back(RectF(0.0f, 0.0f, 256.0f, 256.0f));
	seq.timer = 0.0f;
	RandG randG;
	for (int c = 0; c < 25; c++)
	{
		int result = randG.Get<int>(0, 10);
		BallParticle* p = &emit->AddPartical<BallParticle>( Vec2f(32.0f, 32.0f),
			seq, 12.20f, false, true);
		p->SetGravity(gGravity* 0.25f);
		p->SetDoScale(false);
		p->SetNumberOfBounces(4);
		p->SetBounceYVelocity(-200.0f);
	}
	
	fountains.push_back(&m_EntityMgr->AddObject<Fountain>(Vec2f(400.0f, 548.f), Vec2f(32.0f, 32.0f),100.0f));
	fountains.push_back(&m_EntityMgr->AddObject<Fountain>(Vec2f(1127.0f, 1057.f), Vec2f(32.0f, 32.0f), 100.0f));
	fountains.push_back(&m_EntityMgr->AddObject<Fountain>(Vec2f(2515.0f, 737.f), Vec2f(32.0f, 32.0f), 100.0f));
	fountains.push_back(&m_EntityMgr->AddObject<Fountain>(Vec2f(500.0f, 548.f), Vec2f(32.0f, 32.0f), 100.0f));

	positionText = &m_EntityMgr->AddObject<Text>(Vec2f(10.0f,10.0f), Vec2f(300.0f,32.0f));
	positionText->SetFormat(Locator::Text()->GetFormat("Tahoma12"));
	
}

Game::~Game()
{
	
	
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
	m_cam.Update(deltaTime);
	
	m_pPlayer->DoTranslation(-m_cam.GetPosition());
	
	emit->DoTranslation(-m_cam.GetPosition());
	background2->Translate(Vec2f(-m_cam.GetPosition().x*0.15f, 0.0f));
	
	positionText->SetText(L"Position : " + std::to_wstring(m_pPlayer->Center().x) + L" " + std::to_wstring(m_pPlayer->Center().y));
	for (auto& it : fountains)
	{
		it->DoTranslation(-m_cam.GetPosition());
		it->DoEnableByDistanceTo(m_pPlayer->Center());
	}
	// update physics
	if ((m_pPlayer->Center() - emit->GetPosition()).Len() < 400.0f)
	{
		emit->Start();
		
	}
	else
		emit->Stop();
	m_EntityMgr->Update(deltaTime);
	
	
	
	// handle physics results

	HandleMap();
	DoCollisions();
	//HandleUnits();
	//~
	// refresh objects and groups, removes dead or inactive ect.. 
	m_EntityMgr->Refresh();
	
	
	m_soundFX->PlayQueue();

	return S_OK;
}

HRESULT Game::RenderScene()
{
	HRESULT hr;
	
	hr = gfx.BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	if (FAILED(hr)) { return hr; }
	
	m_vpMain.BeginScene();
	
	m_EntityMgr->Draw();
	 m_pPlayer->Draw();
	
	m_vpMain.EndScene();
	


	m_vpUI.BeginScene();
	
	// TODO render UI controls here
	RenderUI();
	
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
	
	Vec2f size = { 42.0f,48.0f };
	Vec2f position = { 100.0f,100.0f };
	m_pPlayer = (Player*)&m_EntityMgr->AddObject<Player>(position,size);
	m_pPlayer->Get<Transform>().acceleration = 20.20f;
	m_pPlayer->Get<Transform>().friction = 0.920f;
	size.height -= 5.0f;
	m_pPlayer->Add<Collider>(position + (size * 0.5f), size*0.5f);
	m_pPlayer->Add<Input>(window.kbd, window.mouse);
	m_pPlayer->AddGroup(groupMap);


	Animation* an = &m_pPlayer->Get<Animation>();
	Animation::Sequence seq;
	seq.current_index = 0;
	seq.frameDelay = 0.10f;
	seq.image = Locator::Images()->GetImage("robot");
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
	seq.image = Locator::Images()->GetImage("robot2");
	seq.srcRects.push_back(RectF(0.0f, 0.0f, 64.0f, 64.0f));
	an->AddSequence("left_jump", seq);

	seq.srcRects.clear();
	seq.image = Locator::Images()->GetImage("robot2");
	seq.srcRects.push_back(RectF(128.0f, 64.0f, 192.0f, 128.0f));
	an->AddSequence("right_jump", seq);
}

void Game::HandleMap()
{
	m_colliders.clear();
	auto& back_vec = m_EntityMgr->GetGroupAction(groupLayerBack);
	
	for (int i : Iterate(0, (int)back_vec.size()))
	{
		if (back_vec[i]->Get<Transform>().Boundary().Overlaps(m_cam.GetViewFrame()))
		{
			back_vec[i]->AddGroup(groupRender);
		}
		else
		{
			back_vec[i]->RemoveGroup(groupRender);
		}
	}
	m_EntityMgr->ForAllOfType<MapTile>([this](auto& obj)
	{

		if (!obj.Passable())
		{
			if (obj.Get<Transform>().Boundary().Overlaps(m_cam.GetViewFrame()))
			{

				obj.AddGroup(groupRender);
				obj.Get<Transform>().Translate(-m_cam.GetPosition());
				//m_colliders.push_back(&obj.Get<Collider>());
			}
			else
			{
				obj.RemoveGroup(groupRender);
			}
			
		}
	});

	m_EntityMgr->ForAllOfType<BallParticle>([this](auto& ball)
	{
		ball.ResolveCollision(0);
	});
	m_pPlayer->AddGroup(groupRender);
	auto& front_vec = m_EntityMgr->GetGroupAction(groupLayerFront);
	
	for (int i : Iterate(0, (int)front_vec.size()))
	{
		if (front_vec[i]->Get<Transform>().Boundary().Overlaps(m_cam.GetViewFrame()))
		{
			front_vec[i]->AddGroup(groupRender);
		}
		else
		{
			front_vec[i]->RemoveGroup(groupRender);
		}
	}
}

void Game::HandleInput()
{

	
}

void Game::DoCollisions()
{
		m_pPlayer->ResolveCollisions();
}

void Game::RenderUI()
{
	auto& vec = m_EntityMgr->GetGroupAction(groupUI);
	for (auto& it : vec)
	{
		it->DoTranslation(m_vpUI.GetViewTopLeft());
		it->Draw();
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


void Game::LoadImages()
{
	m_ImageMgr = std::make_unique<TextureManager>();
	m_ImageMgr->AddImage(Locator::D2DRenderTarget(), "particle_green", L"assets\\particle.png");
	m_ImageMgr->AddImage(Locator::D2DRenderTarget(), "particle_star", L"assets\\particle2.png");
	m_ImageMgr->AddImage(Locator::D2DRenderTarget(), "particle_fire", L"assets\\particle3.png");
	m_ImageMgr->AddImage(Locator::D2DRenderTarget(), "particle_blue", L"assets\\particle4.png");
	m_ImageMgr->AddImage(Locator::D2DRenderTarget(), "fountain_blue", L"assets\\emitter2.png");
	m_ImageMgr->AddImage(Locator::D2DRenderTarget(), "fountain_red", L"assets\\emitter1.png");
	m_ImageMgr->AddImage(Locator::D2DRenderTarget(), "robot", L"assets\\robo64x64.png");
	m_ImageMgr->AddImage(Locator::D2DRenderTarget(), "robot2", L"assets\\robo2_64x64.png");
	m_ImageMgr->AddImage(Locator::D2DRenderTarget(), "colin", L"assets\\colin.png");
	Locator::SetImageManager(m_ImageMgr.get());
}






