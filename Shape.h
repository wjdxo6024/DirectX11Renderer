#ifndef _SHAPE_H_
#define _SHAPE_H_
#include "Direct2DEngine.h"
#include <memory>
#include <wrl/client.h>

// 모든 도형의 기본 클래스
class Shape
{
protected:
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_shapeBrush;
	D2D1::ColorF m_ShapeLineColor = D2D1::ColorF::White;
	int shape_X, shape_Y;
	int m_width, m_height;

	D2D1_POINT_2F m_CenterPos;
	D2D1_SIZE_F m_RenderSize;
	float m_RotationDegree;
	Direct2DEngine* m_pD2DEngine = NULL;


public:
	Shape();
	Shape(const Shape& other);
	~Shape();

	virtual bool Init(Direct2DEngine* pD2DEngine, int x, int y, int height, int width);
	virtual bool Draw();
	virtual bool Update();
	virtual void Quit();

	virtual void SetColor(D2D1::ColorF color);
	virtual void SetPosition(int x, int y); // TODO : 회전할 경우 다른 요소들이 깨진다.
	virtual void SetRotationDegree(float Degree);
	
};

#endif

