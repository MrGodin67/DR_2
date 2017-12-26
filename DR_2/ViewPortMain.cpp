#include "stdafx.h"
#include "ViewPortMain.h"




ViewPort::ViewPort(const RectF& position)
	:m_view(position)
{
	HRESULT hr = Locator::D2DRenderTarget()->CreateLayer({position.right - position.left,position.bottom - position.top}, &pLayer);
	Locator::D2DRenderTarget()->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f, 1.0f), &brush);
	
}

ViewPort::~ViewPort()
{
	SafeRelease(&pLayer);
	SafeRelease(&brush);
}



float ViewPort::Width()
{
	return m_view.right - m_view.left;
}

float ViewPort::Height()
{
	return m_view.bottom - m_view.top;
}

RectF ViewPort::GetViewRect()
{
	return m_view;
}

Vec2f ViewPort::GetViewTopLeft()
{
	return{ m_view.left,m_view.top };
}

void ViewPort::BeginScene()

{
	//
	Locator::D2DRenderTarget()->PushLayer(D2D1::LayerParameters(
		m_view.ToD2D(), NULL, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
		D2D1::Matrix3x2F::Identity(), m_transparency, brush, D2D1_LAYER_OPTIONS_NONE),
		pLayer);


}

void ViewPort::EndScene()
{
	Locator::D2DRenderTarget()->PopLayer();

}

void ViewPort::ResetTransparency()
{
	m_transparency = 0.0f;
}
