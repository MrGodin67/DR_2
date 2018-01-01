#include "stdafx.h"
#include "Camera.h"


Camera::Camera(const float& width, const float& height)
	:
	pos({ 1.0f,1.0f }),
	center({ width / 2.0f,height / 2.0f }),
	view_width(width),
	view_height(height)
{
	viewFrame.left = pos.x;
	viewFrame.top = pos.y;
	viewFrame.right = viewFrame.left + view_width;
	viewFrame.bottom = viewFrame.top + view_height;
	Resize(width, height);
	
}

//void Camera::Scroll(const Vec2f& dir)
//{
//	
//	scroll_pos += dir;
//	if (scroll_pos.x < center.x)scroll_pos.x = center.x;
//	if (scroll_pos.x > mapFrame.right -center.x)scroll_pos.x = mapFrame.right - center.x;
//	if (scroll_pos.y < center.y)scroll_pos.y = center.y;
//	if (scroll_pos.y > mapFrame.bottom - center.y)scroll_pos.y = mapFrame.bottom - center.y;
//
//	
//
//}

void Camera::Update(const float & dt)
{
	
	Vec2f p = pos;
	pos += (focalPoint - pos) * scrollSpeed * dt;
	ScrollDiff = (pos - p);
	pos.x = std::max(pos.x, mapFrame.left);
	pos.y = std::max(pos.y, mapFrame.top);
	pos.x = std::min(pos.x, mapFrame.right - view_width);
	pos.y = std::min(pos.y, mapFrame.bottom - view_height);


	viewFrame.left = pos.x;
	viewFrame.top = pos.y;
	viewFrame.right = viewFrame.left + view_width;
	viewFrame.bottom = viewFrame.top + view_height;

	// Draw a portion of the mediumground based on the scroll amount
	

}

void Camera::SetFocusPoint(const Vec2f & focus_pt)
{
	focalPoint = focus_pt -center;
	
}

void Camera::SetPanSpeed(const float & pan_speed)
{
	scrollSpeed = pan_speed;
}


Vec2f Camera::GetPosition() const
{ 
	return pos; 
}
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
	view_width = w;
	view_height = h;
	center = Vec2f(view_width * 0.5f, view_height * 0.5f);
	viewFrame.left = pos.x;
	viewFrame.top = pos.y;
	viewFrame.right = viewFrame.left + view_width;
	viewFrame.bottom = viewFrame.top + view_height;

};


