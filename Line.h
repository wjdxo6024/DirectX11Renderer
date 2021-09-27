#ifndef _LINE_H_
#define _LINE_H_

#include "Direct2DEngine.h"
#include "DirectXEngine.h"
#include <memory>
#include <wrl/client.h>

class Line
{
private:
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_LineBrush;


	float sX, sY; // Start Point
	float fX, fY; // End Point

public:
	void Init();
	void Draw();
	void Update();
	void Quit();
};

#endif

