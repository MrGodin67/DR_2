#pragma once



#include "Vec2.h"
#include "Rect.h"

class Camera 
{
	
public:// next = vp
	Camera(const float& width, const float& height);
	
	Vec2f GetPos();
	void ConfineToMap(const RectF& map_frame);
	void Resize(const float& w, const float& h);
	void UpdatePosition(const Vec2f& in_pos);
	Vec2f ConvertToWorldSpace(const Vec2f& in_pos);
	RectF GetViewFrame()const;
	void Scroll(const Vec2f& dir);
private:
	
	Vec2f pos;
	Vec2f center;
	Vec2f scroll_pos;
	RectF mapFrame;
	RectF viewFrame;
	float screen_width, screen_height;
};
