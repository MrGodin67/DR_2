#include "stdafx.h"
#include "Collider.h"




Collider::~Collider()
{
}

void Collider::Draw()
{
}

void Collider::SphericalCollisionCorrection(GameObject * other, const Collision & collision,  bool isStatic)
{
	
	GameObject* A = owner;
	GameObject* B = other;
	Transform* ATrans = &owner->Get<Transform>();
	Transform* BTrans = &other->Get<Transform>();
	
	Vec2f delta = ATrans->Center() - BTrans->Center();
	// minimum translation distance to push balls apart after intersecting
	float distance = delta.Len();
	Vec2f mtd = delta * 
		(A->Get<Collider>().GetSphere().radius + 
			B->Get<Collider>().GetSphere().radius - distance) / distance;
	Vec2f tempMTD = mtd;
	mtd.Normalize();
		// resolve intersection --
		// inverse mass quantities
		float im1 = 1 / 2.25f;
		float im2 = 1 / 1.25f;


		// impact speed
		Vec2f v = (ATrans->velocity - BTrans->velocity).Normalize();
		float vn = v.x*mtd.x + v.y*mtd.y;

		// sphere intersecting but moving away from each other already
		if (vn > 0.0f) 
			return;

		//// collision impulse
		float i = (-(1.0f + 1.25f) * vn) / (im1 + im2);
		Vec2f impulse = tempMTD * i;

		//// change in momentum
		Vec2f Avel = ATrans->velocity + impulse * im1;
		Vec2f Bvel = BTrans->velocity - impulse * im2;
		Avel.Normalize();
		Bvel.Normalize();
		ATrans->velocity = Avel;
		BTrans->velocity = Bvel;
		
		
	
	
}

Collision Collider::Spherical(const Sphere & other)
{
	
	float dist = (other.center - collisionSphere.center).LenSq();
	return Collision({dist,dist}, dist < sq<float>(other.radius + collisionSphere.radius),0);
	
}
