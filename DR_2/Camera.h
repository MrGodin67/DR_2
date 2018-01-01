#pragma once



#include "Vec2.h"
#include "Rect.h"

class Camera 
{
	
public:
	Camera(const float& width, const float& height);
	
	Vec2f GetPosition()const;
	void ConfineToMap(const RectF& map_frame);
	void Resize(const float& w, const float& h);
	Vec2f ConvertToWorldSpace(const Vec2f& in_pos);
	RectF GetViewFrame()const;
	void Update(const float& dt);
	void SetFocusPoint(const Vec2f& focus_point);
	void SetPanSpeed(const float& pan_speed);
	Vec2f Movement() { return ScrollDiff; }
private:
	
	Vec2f pos;
	Vec2f center;
	Vec2f focalPoint; 
	Vec2f ScrollDiff;
	float scrollSpeed = 4.0f;
	RectF mapFrame;
	RectF viewFrame;
	float view_width, view_height;
};
