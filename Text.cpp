#include "Text.h"

bool Text::Init(Direct2DEngine* D2DEngine, WCHAR* String)
{
	m_pD2DEngine = D2DEngine;

	if (FAILED(D2DEngine->GetWriteFactory()->CreateTextFormat(L"Arial", NULL, DWRITE_FONT_WEIGHT_LIGHT, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20.0f, L"ko-KR", m_textFormat.GetAddressOf())))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create Solid Brush.", L"Text Error", MB_OK);
#endif
		return false;
	}

	// TODO : TextLayout이기 때문에 어느정도 문자가 들어가고 그 다음 줄이 바뀌는지 알아 볼 것.
	if (FAILED(D2DEngine->GetWriteFactory()->CreateTextLayout(String, wcslen(String) + 1, m_textFormat.Get(), (float)D2DEngine->GetScreenWidth(), (float)D2DEngine->GetScreenHeight(), m_textLayout.GetAddressOf())))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create Solid Brush.", L"Text Error", MB_OK);
#endif
		return false;
	}
	if (FAILED(D2DEngine->GetDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), m_TextBrush.GetAddressOf())))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create Solid Brush.", L"Text Error", MB_OK);
#endif
		return false;
	}

	return true;
}

bool Text::Draw(float x, float y)
{
	m_pD2DEngine->GetDeviceContext()->DrawTextLayout(D2D1::Point2F(x, y), m_textLayout.Get(), m_TextBrush.Get());
	return true;
}

bool Text::DrawDirect(float x, float y, WCHAR* String)
{
	m_pD2DEngine->GetDeviceContext()->SetTransform(D2D1::IdentityMatrix());
	// 사각형 영역 계산을 잘할 것.
	int stringlength = wcslen(String) + 1;
	D2D1_RECT_F r = { x , y , x + stringlength * 12 , y + 20 };
	m_pD2DEngine->GetDeviceContext()->DrawTextW(String, stringlength, m_textFormat.Get(), &r, m_TextBrush.Get());

	return true;
}

// 사용하지 않음
bool Text::Update()
{
	return true;
}

// 사용하지 않음
void Text::Quit()
{

}

void Text::SetPosition(float x, float y)
{

}

void Text::SetFont(DWRITE_FONT_STYLE fontStyle)
{
	DWRITE_TEXT_RANGE range;
	range.length = 0;
	range.startPosition = 0;
	m_textLayout->SetFontStyle(fontStyle, range);
}

void Text::SetFontEffect(const char* fonteffect)
{

}

void Text::SetFontSize(float fontSize, int range_length)
{
	DWRITE_TEXT_RANGE range;
	range.length = range_length;
	range.startPosition = 0;

	m_textLayout->SetFontSize(fontSize, range);
}

void Text::SetFontColor(D2D1::ColorF color)
{
	m_TextBrush->SetColor(color);
}

void Text::SetTextAlign(DWRITE_TEXT_ALIGNMENT textAlign)
{
	m_textLayout->SetTextAlignment(textAlign);
}