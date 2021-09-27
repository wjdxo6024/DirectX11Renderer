#pragma once
#include "Shape.h"
class CRectangle :
	public Shape
{
protected:
	D2D1_RECT_F m_Rect;
	D2D1::ColorF m_FillColor = NULL;
	Microsoft::WRL::ComPtr<ID2D1StrokeStyle> m_StrokeStyle; // ¹Ì±¸Çö
	float m_LineStroke;
	bool IsFill;
	bool IsRound;
public:
	CRectangle();
	~CRectangle();

	bool Init(Direct2DEngine* pD2DEngine, int x, int y, int height, int width);
	bool Draw();
	bool DrawDirect(float x, float y);
	bool Update();
	void Quit();

	void SetFill(bool isFill);
	void SetRound(bool isRound);
	void SetStroke(float stroke);
	void SetFillColor(D2D1::ColorF color);
};

