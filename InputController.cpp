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
	// ���콺 Ŀ���� ��ġ ������ ���� ȭ�� ũ�⸦ ����
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Direct Input �������̽��� �ʱ�ȭ
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

	// Ű������ Direct Input �������̽��� ����
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

	// ������ ������ ����, Ű�����̹Ƿ� ���� ���� �� ������ ������ ���
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

	// �ٸ� ���α׷��� �������� �ʵ��� Ű������ ���� ������ ����
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

	// Ű���带 �Ҵ�
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

	// ���콺 Direct Input �������̽��� ����
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

	// �̸� ���� �� ���콺 ������ ������ ����Ͽ� ���콺�� ������ ������ ����
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

	// �ٸ� ���α׷��� ���� �� �� �ֵ��� ���콺�� ���� ������ ����
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

	// ���콺�� �Ҵ�޴´�.
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
	// Ű������ ���� ���¸� �д´�.
	if (!ReadKeyboard())
	{
		return false;
	}

	// ���콺�� ���� ���¸� �д´�.
	if (!ReadMouse())
	{
		return false;
	}

	ProcessInput();

	return true;
}

bool InputController::IsEscapePressed()
{
	// escape Ű�� ���� �������� �ִ��� bit���� ����Ͽ� Ȯ��
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
	// Ű���� ����̽��� ��´�.
	HRESULT result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);

	if (FAILED(result))
	{
		// Ű���尡 ��Ŀ���� �Ҿ��ų� ȹ����� ���� ��� ��Ʈ���� �ٽ� ������.
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
	// ���콺 ����̽��� ��´�.
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
	// ������ ���� ���콺 ��ġ�� ������ ������� ���콺 Ŀ���� ��ġ�� ������Ʈ
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// ���콺 ��ġ�� ȭ�� �ʺ� �Ǵ� ���̸� �ʰ����� �ʴ��� Ȯ��
	if (m_mouseX < 0) {
		m_mouseX = 0;
	}

	if (m_mouseY < 0) {
		m_mouseY = 0;
	}

	if (m_mouseX < m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY < m_screenHeight) { m_mouseY = m_screenHeight; }

}