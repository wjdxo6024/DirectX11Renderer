#ifndef _CBITMAP_H_
#define _CBITMAP_H_
#include "Direct2DEngine.h"
#include <memory>
#include <wrl/client.h>
#include <wincodec.h>

class CBitmap
{
protected:
	Microsoft::WRL::ComPtr<ID2D1Bitmap> m_Bitmap;
	Microsoft::WRL::ComPtr<IWICImagingFactory> factory;

	float m_imageX, m_imageY;
	float m_Width, m_Height;

	D2D1_POINT_2F m_CenterPos;
	D2D1_SIZE_F m_RenderSize;
	float m_RotationDegree;
	Direct2DEngine* m_pD2DEngine = NULL;
public:
	virtual bool Init(Direct2DEngine* pD2DEngine, int x, int y, int height, int width, WCHAR* filePath);
	virtual bool Draw();
	virtual bool Update();
	virtual void Quit();
};

#endif