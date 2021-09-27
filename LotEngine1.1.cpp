// LotEngine1.1.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "LotEngine1.1.h"
#include "WindowOption.h"
#include "D3DMainFrame.h"
#include "GameScene.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND g_hWnd;
WindowOption windowOption;
LPCWSTR lpszClass = TEXT("LotEngine");

// DirectX
D3DMainFrame g_D3DFrame;
GameScene g_GameScene;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LOTENGINE11, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LOTENGINE11));

    MSG msg;

    // 기본 메시지 루프입니다:
	while (true)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_D3DFrame.Update();
			g_GameScene.Update();

			// 렌더링
			g_GameScene.PreRender();

			g_D3DFrame.Begin();
			g_GameScene.Render();
			g_D3DFrame.End();

			g_GameScene.AfterRender();
		}
	}

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LOTENGINE11));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	int posX, posY;

	//풀 스크린
	if (windowOption.getFullScreen())
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)windowOption.getWidth();
		dmScreenSettings.dmPelsHeight = (unsigned long)windowOption.getHeight();
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
	}
	else
	{
		windowOption.setWidth(1024);
		windowOption.setHeight(768);
		posX = (GetSystemMetrics(SM_CXSCREEN) - windowOption.getWidth()) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - windowOption.getHeight()) / 2;
	}

   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_BORDER, posX, posY,
	   windowOption.getWidth(), windowOption.getHeight(), nullptr, nullptr, hInst, nullptr);
   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

   SetForegroundWindow(g_hWnd);
   SetFocus(g_hWnd);

   g_D3DFrame.Init(windowOption.getWidth(), windowOption.getHeight(), g_hWnd, windowOption.getFullScreen());
   g_GameScene.Init(g_D3DFrame.GetDirectXEngine(), g_D3DFrame.GetDirect2DEngine(), g_hWnd,
	   windowOption.getWidth(), windowOption.getHeight());
   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		// 키보드 입력
		case WM_KEYDOWN:
		{
			g_GameScene.KeyboardEvent(message, wParam, lParam);
		}
		break;
		// 마우스 입력
		case WM_MOUSEMOVE:
		{
			g_GameScene.MouseEvent(message, wParam, lParam);
		}
		break;
		case WM_COMMAND:
        {
        }
        break;
		case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			g_GameScene.Release();
			g_D3DFrame.Quit();
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
