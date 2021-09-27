#include "CEllipse.h"
CEllipse::CEllipse()
{
	
}

CEllipse::~CEllipse()
{

}

bool CEllipse::Init(Direct2DEngine* pD2DEngine, int x, int y, int height, int width)
{
	Shape::Init(pD2DEngine, x, y, height, width);
	m_Ellipse.point.x = shape_X;
	m_Ellipse.point.y = shape_Y;
	m_Ellipse.radiusX = m_width;
	m_Ellipse.radiusY = m_height;

	// Default
	m_LineStroke = 10.0f;
	IsFill = false;
	return true;
}

bool CEllipse::Draw()
{
	D2D1_MATRIX_3X2_F rotation;
	if (m_RotationDegree != 0.0f)
	{
		D2D1MakeRotateMatrix(m_RotationDegree, m_CenterPos, &rotation);
		m_pD2DEngine->GetDeviceContext()->SetTransform(rotation);
	}

	m_pD2DEngine->GetDeviceContext()->DrawEllipse(m_Ellipse, *m_shapeBrush.GetAddressOf(), m_LineStroke);
	
	if (IsFill)
	{
		m_pD2DEngine->GetDeviceContext()->FillEllipse(m_Ellipse, *m_shapeBrush.GetAddressOf());
	}

	return true;
}

bool CEllipse::Update()
{
	return true;
}

void CEllipse::Quit()
{

}

void CEllipse::SetFill(bool isFill)
{
	IsFill = isFill;
}

void CEllipse::SetStroke(float stroke)
{
	m_LineStroke = stroke;
}

void CEllipse::SetFillColor(D2D1::ColorF color)
{
	m_FillColor = color;
}