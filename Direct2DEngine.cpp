#include "Direct2DEngine.h"

// TODO: Microsoft의 문서를 참조할 것. 1번 방식으로 사용하며, 기존 DirectX 11의 스왑체인을 가져와서 그것에 출력을 지정하도록함.
/*
1.Create the Direct2D and the DirectWrite factories.
2.Create the Direct2D device and its context.
3.Set up Direct2D to render to the same buffer as Direct3D.
4.Resize the Direct2D render targets when the game window is resized.
5.Set up brushes and text formats.
6.Set up text layouts.
7.Print!
*/
bool Direct2DEngine::Init(DirectXEngine* DXEngine, HWND hWnd, int screenWidth, int screenHeight, bool fullscreen)
{
	m_screenHeight = screenHeight;
	m_screenWidth = screenWidth;

	// 여기 조심 할것. ComPtr을 사용하지 않았음.
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&m_writeFactory))))
	{
		MessageBox(NULL, L"Cannot Create WriteFactory.", L"Error", MB_OK);
		return false;
	}

	// create Factory
	ID2D1Factory1* factory = nullptr;

	D2D1_FACTORY_OPTIONS options;
	// 만약 디버깅 할경우 D2D1_DEBUG_LEVEL_INFORMATION을 사용
#ifdef _DEBUG
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#else
	options.debugLevel = D2D1_DEBUG_LEVEL_NONE;
#endif

	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, __uuidof(ID2D1Factory1), &options, (void**)&factory)))
	{
		MessageBox(NULL, L"Cannot Create 2D Factory.", L"Error", MB_OK);
		return false;
	}

	// dxgi 가져오기
	if (FAILED(DXEngine->GetDevice()->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&m_dxgiDevice))))
	{
		MessageBox(NULL, L"Cannot Get Device.", L"Error", MB_OK);
		return false;
	}

	// Direct2D device 만들기
	if (FAILED(factory->CreateDevice(m_dxgiDevice, &m_d2dDevice)))
	{
		MessageBox(NULL, L"Cannot Create D2D Device.", L"Error", MB_OK);
		return false;
	}

	if (FAILED(m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS, &m_d2dDeviceContext)))
	{
		MessageBox(NULL, L"Cannot Create D2D DeviceContext.", L"Error", MB_OK);
		return false;
	}

	// 비트맵 설정
	FLOAT dpiX;
	FLOAT dpiY;

	UINT nDpiX;
	UINT nDpiY;

	// 이 소스코드는 C4996에러가 발생하여 사용 안함
	//factory->GetDesktopDpi(&dpiX, &dpiY);

	HWND hDesktopWnd = GetDesktopWindow();
	HMONITOR hMonitor = MonitorFromWindow(hDesktopWnd, MONITOR_DEFAULTTOPRIMARY);
	GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &nDpiX, &nDpiY);

	D2D1_BITMAP_PROPERTIES1 bitmapProperties;
	bitmapProperties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	bitmapProperties.dpiX = (FLOAT)nDpiX;
	bitmapProperties.dpiY = (FLOAT)nDpiY;
	bitmapProperties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bitmapProperties.colorContext = nullptr;

	// Direc2D는 백버퍼의 DXGI 버전이 필요함
	IDXGISurface* dxgiBuffer = nullptr;
	if (FAILED(DXEngine->GetSwapChain()->GetBuffer(0, __uuidof(IDXGISurface), (void**)&dxgiBuffer)))
	{
		MessageBox(NULL, L"Cannot Create DXGI Surface.", L"Error", MB_OK);
		return false;
	}

	// 비트맵 생성
	ID2D1Bitmap1* targetBitmap = nullptr;
	HRESULT result = m_d2dDeviceContext->CreateBitmapFromDxgiSurface(dxgiBuffer, &bitmapProperties, &targetBitmap);
	if (FAILED(result))
	{
		MessageBox(NULL, L"Cannot Create Bitmap DXGI Surface.", L"Error", MB_OK);
		return false;
	}

	// 비트맵 렌더타겟으로 지정
	m_d2dDeviceContext->SetTarget(targetBitmap);

	if (!setTextRender())
	{
		MessageBox(NULL, L"Cannot Set Text Option.", L"Error", MB_OK);
		return false;
	}

	// Release
	if (factory)
	{
		factory->Release();
		// delete factory;
	}


	if (dxgiBuffer)
	{
		dxgiBuffer->Release();
		// delete dxgiBuffer;
	}

	if (targetBitmap)
	{
		targetBitmap->Release();
		// delete targetBitmap;
	}

	return true;
}

bool Direct2DEngine::Render()
{
	BeginDraw();
	m_d2dDeviceContext->DrawTextLayout(D2D1::Point2F(2.0f, 5.0f), m_textLayoutFPS, yellowBrush);
	EndDraw();
	return true;
}

bool Direct2DEngine::BeginDraw()
{
	m_d2dDeviceContext->BeginDraw();
	return true;
}

bool Direct2DEngine::EndDraw()
{
	m_d2dDeviceContext->EndDraw();
	return true;
}

bool Direct2DEngine::Update()
{
	return true;
}

bool Direct2DEngine::Quit()
{
	TextQuit();

	if (m_d2dDeviceContext)
	{
		m_d2dDeviceContext->Release();
		m_d2dDeviceContext = 0;
	}

	if (m_d2dDevice)
	{
		m_d2dDevice->Release();
		m_d2dDevice = 0;
	}

	if (m_dxgiDevice)
	{
		m_dxgiDevice->Release();
		m_dxgiDevice = 0;
	}

	if (m_writeFactory)
	{
		m_writeFactory->Release();
		m_writeFactory = 0;
	}
	return true;
}

void Direct2DEngine::TextQuit()
{
	if (yellowBrush)
	{
		yellowBrush->Release();
		yellowBrush = 0;
	}

	if (blackBrush)
	{
		blackBrush->Release();
		blackBrush = 0;
	}

	if (whiteBrush)
	{
		whiteBrush->Release();
		whiteBrush = 0;
	}

	if (m_textFormatFPS)
	{
		m_textFormatFPS->Release();
		m_textFormatFPS = 0;
	}

	if (m_normalFormatText)
	{
		m_normalFormatText->Release();
		m_normalFormatText = 0;
	}

	if (m_highlightFormatText)
	{
		m_highlightFormatText->Release();
		m_highlightFormatText = 0;
	}

	if (m_textLayoutFPS)
	{
		m_textLayoutFPS->Release();
		m_textLayoutFPS = 0;
	}
}

void Direct2DEngine::onResize()
{
	m_d2dDeviceContext->SetTarget(nullptr);

	m_d2dDeviceContext->Clear();
}

bool Direct2DEngine::initTextRender()
{
	if (FAILED(m_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &yellowBrush)))
	{
		MessageBox(NULL, L"Cannot Create Yellow Brush.", L"Error", MB_OK);
		return false;
	}
	if (FAILED(m_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &blackBrush)))
	{
		MessageBox(NULL, L"Cannot Create Black Brush.", L"Error", MB_OK);
		return false;
	}
	if (FAILED(m_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &whiteBrush)))
	{
		MessageBox(NULL, L"Cannot Create White Brush.", L"Error", MB_OK);
		return false;
	}

	// TextFormat 설정
	if (FAILED(m_writeFactory->CreateTextFormat(L"Lucida Console", NULL, DWRITE_FONT_WEIGHT_LIGHT, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 20.0f, L"en-GB", &m_textFormatFPS)))
	{
		MessageBox(NULL, L"Cannot Create Text Format.", L"Error", MB_OK);
		return false;
	}

	if (FAILED(m_textFormatFPS->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING)))
	{
		MessageBox(NULL, L"Cannot Create Text Alignment.", L"Error", MB_OK);
		return false;
	}

	if (FAILED(m_textFormatFPS->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR)))
	{
		MessageBox(NULL, L"Cannot Create Text Paragraph.", L"Error", MB_OK);
		return false;
	}
	return true;
}

bool Direct2DEngine::setTextRender()
{
	if (!initTextRender())
	{
		MessageBox(NULL, L"Cannot Init Text Brush.", L"Error", MB_OK);
		return false;
	}

	WCHAR TestText[] = L"LotEngine";
	if (FAILED(m_writeFactory->CreateTextLayout(TestText, sizeof(TestText) / sizeof(WCHAR), m_textFormatFPS, (float)m_screenWidth, (float)m_screenHeight, &m_textLayoutFPS)))
	{
		MessageBox(NULL, L"Cannot Text Layout.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void Direct2DEngine::SetBrush(WCHAR* brush)
{

}

IDWriteFactory* Direct2DEngine::GetWriteFactory()
{
	return m_writeFactory;
}

ID2D1DeviceContext* Direct2DEngine::GetDeviceContext()
{
	return m_d2dDeviceContext;
}

ID2D1Device* Direct2DEngine::GetDevice()
{
	return m_d2dDevice;
}


int Direct2DEngine::GetScreenWidth()
{
	return m_screenWidth;
}

int Direct2DEngine::GetScreenHeight()
{
	return m_screenHeight;
}

