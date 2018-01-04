#include "stdafx.h"
#include "Fountain.h"
#include "Animation.h"
#include "Collider.h"
#include "Locator.h"
#include "Image.h"
Fountain::Fountain(const Vec2f& pos, const Vec2f& size, const float& distanceToActivate, ID2D1Bitmap* image, ID2D1Bitmap* particle)
	:distanceToActivate(distanceToActivate),particle(particle)
{
	
	Add<Transform>(pos, size);
	Add<Collider>(pos + size*0.5f, size * 0.5f);
	Add<Image>(image);
	

}


Fountain::~Fountain()
{
}

void Fountain::Init()
{
	
	
	emitter = &manager->AddObject<Emitter>(Get<Transform>().position);
	emitter->SetSpawnInterval(0.09f);
	emitter->SetRandomVelocityConstrants(Vec2f(-100.0f, 100.0f), Vec2f(-200.0f, -100.0f));
	Animation::Sequence seq2;
	seq2.current_index = 0llu;
	seq2.frameDelay = 0.0f;
	seq2.image = particle; 
	seq2.srcRects.push_back(RectF(0.0f, 0.0f, 256.0f, 256.0f));
	seq2.timer = 0.0f;
	RandG randG;
	for (int c = 0; c < 20; c++)
	{
		int result = randG.Get<int>(0, 10);


		Particle* p = &emitter->AddPartical<Particle>(Vec2f(16.0f, 16.0f),
			seq2, 2.20f, true, true);
		p->SetGravity(gGravity* 0.25f);
		p->SetDoScale(true);

	}
	
	AddGroup(groupLayerFront);
	AddGroup(groupCollider);
	
}

void Fountain::AddEmitter(Emitter * e)
{
	emitter = e;
	emitter->SetPosition(Get<Transform>().Center());
}

void Fountain::Draw()
{
	Get<Image>().Draw();
}

void Fountain::Update(const float & dt)
{
	for (auto& it : m_components)
		it->Update(dt);
}

void Fountain::DoEnableByDistanceTo(const Vec2f & objectPosition)
{
	if ((objectPosition - Get<Transform>().Center()).Len() < distanceToActivate)
		emitter->Start();
}

void Fountain::DoTranslation(const Vec2f & offset)
{
	Get<Transform>().Translate(offset);
	emitter->DoTranslation(offset);
}
