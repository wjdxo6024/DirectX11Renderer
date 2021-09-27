#include "InputController.h"

InputController::InputController()
{

}

InputController::InputController(const InputController& other)
{

}

InputController::~InputController()
{

}

bool InputController::Initialize(HINSTANCE hInstance, HWND hWnd, int screenWidth, int screenHeight)
{
	bool IsInit = true;
	// 마우스 커서의 위치 지정에 사용될 화면 크기를 설정
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Direct Input 인터페이스를 초기화
	HRESULT result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Init Input Device.", L"Error", MB_OK);
		IsInit = false;
#else
		return false;
#endif
	}

	// 키보드의 Direct Input 인터페이스를 생성
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create Input Device.", L"Error", MB_OK);
		IsInit = false;
#else
		return false;
#endif
	}

	// 데이터 형식을 설정, 키보드이므로 사전 정의 된 데이터 형식을 사용
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Set Data Format. Keyboard.", L"Error", MB_OK);
		IsInit = false;
#else
		return false;
#endif
	}

	// 다른 프로그램과 공유하지 않도록 키보드의 협조 수준을 설정
	result = m_keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Set Cooperative Level.", L"Error", MB_OK);
		IsInit = false;
#else
		return false;
#endif
	}

	// 키보드를 할당
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
#ifdef _DEBUG
		MessageBoxW(hWnd, L"DirectInputDevice8 :: Acquire failed", L"Error", MB_ICONERROR | MB_OK);
		IsInit = false;
#else
		return false;
#endif

	}

	// 마우스 Direct Input 인터페이스를 생성
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Create Mouse InterFace Device.", L"Error", MB_OK);
		IsInit = false;
#else
		return false;
#endif
	}

	// 미리 정의 된 마우스 데이터 형식을 사용하여 마우스의 데이터 형식을 설정
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Set Data Format.", L"Error", MB_OK);
		IsInit = false;
#else
		return false;
#endif
	}

	// 다른 프로그램과 공유 할 수 있도록 마우스의 협력 수준을 설정
	result = m_mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Set Cooperative Level. Mouse.", L"Error", MB_OK);
		IsInit = false;
#else
		return false;
#endif
	}

	// 마우스를 할당받는다.
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
#ifdef _DEBUG
		MessageBox(NULL, L"Cannot Acquire Mouse.", L"Error", MB_OK);
		IsInit = false;
#else
		return false;
#endif
	}
	return IsInit;
}

void InputController::Quit()
{
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}
}

bool InputController::Frame()
{
	// 키보드의 현재 상태를 읽는다.
	if (!ReadKeyboard())
	{
		return false;
	}

	// 마우스의 현재 상태를 읽는다.
	if (!ReadMouse())
	{
		return false;
	}

	ProcessInput();

	return true;
}

bool InputController::IsEscapePressed()
{
	// escape 키가 현재 눌려지고 있는지 bit값을 계산하여 확인
	if (m_keyboardState[DIK_ESCAPE] & 80)
	{
		return true;
	}

	return false;
}

void InputController::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
}

char InputController::GetKeyBoardBuffer()
{
	for (int i = 0; i < 256; i++)
	{
		if (m_keyboardState[i] & 80)
		{
			return m_keyboardState[i];
		}
	}

	return 0;
}

bool InputController::ReadKeyboard()
{
	// 키보드 디바이스를 얻는다.
	HRESULT result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);

	if (FAILED(result))
	{
		// 키보드가 포커스를 잃었거나 획득되지 않은 경우 컨트롤을 다시 가져옴.
		if ((result == DIERR_INPUTLOST || (result == DIERR_NOTACQUIRED)))
		{
			m_keyboard->Acquire();
		}
		else
			return false;
	}

	return true;
}

bool InputController::ReadMouse()
{
	// 마우스 디바이스를 얻는다.
	HRESULT result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
			return false;
	}

	return true;
}

void InputController::ProcessInput()
{
	// 프레임 동안 마우스 위치의 변경을 기반으로 마우스 커서의 위치를 업데이트
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// 마우스 위치가 화면 너비 또는 높이를 초과하지 않는지 확인
	if (m_mouseX < 0) {
		m_mouseX = 0;
	}

	if (m_mouseY < 0) {
		m_mouseY = 0;
	}

	if (m_mouseX < m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY < m_screenHeight) { m_mouseY = m_screenHeight; }

}