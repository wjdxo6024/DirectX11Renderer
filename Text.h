#ifndef _TEXT_H_
#define _TEXT_H_

#include "Direct2DEngine.h"
#include "DirectXEngine.h"
#include <memory>
#include <wrl/client.h>

class Text
{
private:
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_textFormat;
	Microsoft::WRL::ComPtr<IDWriteTextLayout> m_textLayout;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_TextBrush;

	Direct2DEngine* m_pD2DEngine;

public:
	bool Init(Direct2DEngine* D2DEngine, WCHAR* String);
	bool Draw(float x, float y);
	bool DrawDirect(float x, float y, WCHAR* String);
	bool Update();
	void Quit();

	void SetFont(DWRITE_FONT_STYLE font);
	void SetFontEffect(const char* fonteffect);
	void SetFontSize(float fontSize, int range_length);
	void SetFontColor(D2D1::ColorF color);
	void SetTextAlign(DWRITE_TEXT_ALIGNMENT textAlign);
	void SetPosition(float, float);

};

#endif