#include "stdafx.h"
#include "Camera.h"


Camera::Camera(const float& width, const float& height)
	:
	pos({ 0.0f,0.0f }),
	center({ width / 2.0f,height / 2.0f }),
	screen_width(width),
	screen_height(height)
{
	viewFrame.left = pos.x;
	viewFrame.top = pos.y;
	viewFrame.right = viewFrame.left + screen_width;
	viewFrame.bottom = viewFrame.top + screen_height;
	Resize(width, height);
	scroll_pos = center;
}

void Camera::Scroll(const Vec2f& dir)
{
	
	scroll_pos += dir;
	if (scroll_pos.x < center.x)scroll_pos.x = center.x;
	if (scroll_pos.x > mapFrame.right -center.x)scroll_pos.x = mapFrame.right - center.x;
	if (scroll_pos.y < center.y)scroll_pos.y = center.y;
	if (scroll_pos.y > mapFrame.bottom - center.y)scroll_pos.y = mapFrame.bottom - center.y;

	this->UpdatePosition(scroll_pos);

}


Vec2f Camera::GetPos() { return pos; }
void Camera::ConfineToMap(const RectF& map_frame)
{ 
	mapFrame = map_frame; 
};
Vec2f Camera:: ConvertToWorldSpace(const Vec2f& in_pos)
{
	return Vec2f (in_pos + this->pos);
}

RectF Camera::GetViewFrame() const
{
	return viewFrame;
}

void Camera::Resize(const float& w, const float& h)
{
	screen_width = w;
	screen_height = h;
	center = Vec2f(screen_width / 2, screen_height / 2);
	viewFrame.left = pos.x;
	viewFrame.top = pos.y;
	viewFrame.right = viewFrame.left + screen_width;
	viewFrame.bottom = viewFrame.top + screen_height;

};

void Camera::UpdatePosition(const Vec2f& in_pos)
{
	pos = in_pos - center ;
	
	pos.x = std::max(pos.x, mapFrame.left);
	pos.y = std::max(pos.y, mapFrame.top);
	pos.x = std::min(pos.x, mapFrame.right - screen_width);
	pos.y = std::min(pos.y, mapFrame.bottom - screen_height);


	viewFrame.left = pos.x;
	viewFrame.top = pos.y;
	viewFrame.right = viewFrame.left + screen_width;
	viewFrame.bottom = viewFrame.top + screen_height;
}
