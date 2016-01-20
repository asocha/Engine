//=====================================================
// InputSystem.cpp
// by Andrew Socha
//=====================================================

#include "InputSystem.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Engine/Math/Vec2.hpp"

InputSystem* s_theInputSystem = nullptr;

///=====================================================
/// 
///=====================================================
LRESULT __stdcall WindowMessageProcessingFunction(HWND windowHandle, UINT messageID, WPARAM wParam, LPARAM lParam){
	if (s_theInputSystem){
		switch(messageID){
		case WM_CHAR:
			s_theInputSystem->SetCurrentCharacterDown((int)wParam);
			return 0;
		case WM_KEYDOWN:
			s_theInputSystem->SetKeyState((int)wParam, true);
			return 0;
		case WM_KEYUP:
			s_theInputSystem->SetKeyState((int)wParam, false);
			return 0;
		case WM_CLOSE:
			s_theInputSystem->SetSystemQuit();
			break;
		case WM_MOUSEWHEEL:
			s_theInputSystem->SetMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
			return 0;
		case WM_LBUTTONDOWN:
			if (wParam == MK_LBUTTON){
				s_theInputSystem->SetLeftMouseButton(true);
				return 0;
			}
			break;
		case WM_RBUTTONDOWN:
			if (wParam == MK_RBUTTON){
				s_theInputSystem->SetRightMouseButton(true);
				return 0;
			}
			break;
		default:
			break;
		}
	}

	return DefWindowProc(windowHandle, messageID, wParam, lParam);
}

///=====================================================
/// 
///=====================================================
InputSystem::InputSystem()
:m_platformHandle(0),
m_previousAddress(0L),
m_mouseWheelWentUp(false),
m_mouseWheelWentDown(false),
m_leftMouseButton(false),
m_rightMouseButton(false),
m_currentCharacterDown(-1),
m_anyKeyWentUp(false),
m_anyKeyWentDown(false){
	RECOVERABLE_ASSERT(s_theInputSystem == nullptr);
	s_theInputSystem = this;
}

///=====================================================
/// 
///=====================================================
void InputSystem::Startup(void* platformHandle){
	m_isReadyToQuit = false;
	m_platformHandle = platformHandle;
	m_previousAddress = (long)SetWindowLongPtr((HWND)platformHandle, GWLP_WNDPROC, (long)&WindowMessageProcessingFunction);

	for (int i = 0; i < NUMBER_OF_VIRTUAL_KEYS; ++i){
		m_keyStates[i].m_isKeyDown = false;
		m_keyStates[i].m_stateJustChanged = false;
	}
}

///=====================================================
/// 
///=====================================================
void InputSystem::Shutdown(){
	if (m_previousAddress != 0L)
		SetWindowLongPtr((HWND)m_platformHandle, GWLP_WNDPROC, m_previousAddress);
}

///=====================================================
/// 
///=====================================================
void InputSystem::Update(){
	for (int i = 0; i < NUMBER_OF_VIRTUAL_KEYS; ++i){
		m_keyStates[i].m_stateJustChanged = false;
	}
	m_currentCharacterDown = -1;

	m_mouseWheelWentUp = false;
	m_mouseWheelWentDown = false;

	m_leftMouseButton = false;
	m_rightMouseButton = false;

	m_anyKeyWentDown = false;
	m_anyKeyWentUp = false;

	HWND myPlatformHandle = (HWND)m_platformHandle;
	MSG messageHandler;

	BOOL messageWasFound = PeekMessage(&messageHandler, myPlatformHandle, 0, 0, PM_REMOVE);
	while (messageWasFound){
		TranslateMessage(&messageHandler);
		DispatchMessage(&messageHandler);

		messageWasFound = PeekMessage(&messageHandler, myPlatformHandle, 0, 0, PM_REMOVE);
	}
}

///=====================================================
/// 
///=====================================================
const Vec2 InputSystem::GetMousePosition() const{
	POINT point;
	GetCursorPos(&point);
	return Vec2((float)point.x, (float)point.y);
}

///=====================================================
/// 
///=====================================================
void InputSystem::SetMousePosition(const Vec2& position) const{
	SetCursorPos((int)position.x, (int)position.y);
}

///=====================================================
/// 
///=====================================================
void InputSystem::ResetKeys(){
	for (int i = 0; i < NUMBER_OF_VIRTUAL_KEYS; ++i){
		m_keyStates[i].m_stateJustChanged = false;
		m_keyStates[i].m_isKeyDown = false;
	}

	m_currentCharacterDown = -1;
	m_anyKeyWentDown = false;
	m_anyKeyWentUp = false;
}

///=====================================================
/// 
///=====================================================
void InputSystem::ShowMouse(bool isVisible) const{
	if (isVisible)
		ShowCursor(TRUE);
	else
		ShowCursor(FALSE);
}
