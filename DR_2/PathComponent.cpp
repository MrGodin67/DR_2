#include "stdafx.h"
#include "PathComponent.h"
#include "Locator.h"
Path::Path()
{
}

Path::Path(const std::vector<Vec2f>& data)
	:positions(data)
{
	std::vector<Vec2f>p;
	positions.push_back(positions.back());

	for (float i = 0.0f; i < (float)positions.size() - 3; i += 0.15f)
	{
		p.push_back(GetSplinePoint(i));
	}
	p.push_back(positions.back());
	positions = p;
}


Path::~Path()
{
}

void Path::Init()
{
	transform = &owner->Get<Transform>();
	transform->rotation_angle = (positions[currentIndex] - transform->Center()).Angle() + 90.0f;
	transform->Rotate();
	transform->velocity = (positions[currentIndex] - transform->Center()).Normalize();
}

void Path::Update(const float & dt)
{
	if (moving && !done)
	{
		transform->Rotate();
		float lenSq = (transform->Center() - positions[currentIndex]).LenSq();
		if (lenSq < sq<float>((transform->acceleration*dt)))
		{
			currentIndex++;
			done = currentIndex >= positions.size();
			if (!done)
			{
				transform->rotation_angle = (positions[currentIndex] - transform->Center()).Angle() + 90.0f;
				transform->Rotate();
				transform->velocity = (positions[currentIndex] - transform->Center()).Normalize();
			}
			else
			{
				
				transform->velocity = Vec2f(0.0f, 0.0f);
			}

		}
	}
}



bool Path::Completed() const
{
	return done;
}

bool Path::Moving() const
{
	return moving;
}

void Path::Moving(const bool & value)
{
	moving = value;
}

Vec2f Path::GetSplinePoint(float t)
{
	int p0, p1, p2, p3;

	p1 = (int)t + 1;
	p2 = p1 + 1;
	p3 = p2 + 1;
	p0 = p1 - 1;


	t = t - (int)t;

	float sqt = sq<float>(t);// squared t
	float ct = sqt * t; //cubed t

	float q1 = -ct + 2.0f*sqt - t;
	float q2 = 3.0f*ct - 5.0f*sqt + 2.0f;
	float q3 = -3.0f*ct + 4.0f*sqt + t;
	float q4 = ct - sqt;

	float tx = 0.5f * (positions[p0].x * q1 + positions[p1].x * q2 + positions[p2].x * q3 + positions[p3].x * q4);
	float ty = 0.5f * (positions[p0].y * q1 + positions[p1].y * q2 + positions[p2].y * q3 + positions[p3].y * q4);

	return{ tx, ty };
}
