#pragma once
#include "Shape.h"
class CEllipse :
	public Shape
{
protected:
	D2D1_ELLIPSE m_Ellipse;
	D2D1::ColorF m_FillColor = NULL;
	Microsoft::WRL::ComPtr<ID2D1StrokeStyle> m_StrokeStyle; // ¹Ì±¸Çö
	float m_LineStroke;
	bool IsFill;

public:
	CEllipse();
	~CEllipse();

	bool Init(Direct2DEngine* pD2DEngine, int x, int y, int height, int width);
	bool Draw();
	bool Update();
	void Quit();

	void SetFill(bool isFill);
	void SetStroke(float stroke);
	void SetFillColor(D2D1::ColorF color);
};

