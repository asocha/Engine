//=====================================================
// InputSystem.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_InputSystem__
#define __included_InputSystem__

#define NUMBER_OF_VIRTUAL_KEYS 256

#include "Engine/Core/Assert.hpp"
struct Vec2;

struct KeyState{
public:
	bool m_isKeyDown;
	bool m_stateJustChanged;
};

class InputSystem{
private:
	void* m_platformHandle;
	long m_previousAddress;
	KeyState m_keyStates[NUMBER_OF_VIRTUAL_KEYS];
	bool m_isReadyToQuit;
	bool m_mouseWheelWentUp;
	bool m_mouseWheelWentDown;
	bool m_leftMouseButton;
	bool m_rightMouseButton;
	int m_currentCharacterDown;
	bool m_anyKeyWentDown;
	bool m_anyKeyWentUp;

public:
	InputSystem();
	inline ~InputSystem(){};

	void Startup(void* platformHandle);
	void Shutdown();
	void Update();

	void ResetKeys();

	void SetKeyState(int key, bool isPressedDown);
	bool IsKeyDown(int key) const;
	bool DidStateJustChange(int key) const;

	bool GetKeyWentDown(int key) const;
	bool GetKeyWentUp(int key) const;

	inline bool GetAnyKeyWentDown() const { return m_anyKeyWentDown; }
	inline bool GetAnyKeyWentUp() const { return m_anyKeyWentUp; }

	inline void SetSystemQuit(){m_isReadyToQuit = true;}
	inline bool IsReadyToQuit() const{return m_isReadyToQuit;}

	void ShowMouse(bool isVisible) const;
	const Vec2 GetMousePosition() const;
	void SetMousePosition(const Vec2& position) const;

	void SetMouseWheel(short z);
	inline bool MouseWheelWentUp() const{return m_mouseWheelWentUp;}
	inline bool MouseWheelWentDown() const{return m_mouseWheelWentDown;}

	inline void SetLeftMouseButton(bool isDown){m_leftMouseButton = isDown;}
	inline void SetRightMouseButton(bool isDown){m_rightMouseButton = isDown;}
	inline bool GetLeftMouseButtonDown() const{return m_leftMouseButton;}
	inline bool GetRightMouseButtonDown() const{return m_rightMouseButton;}

	inline int GetCurrentCharacterDown() const{return m_currentCharacterDown;}
	inline void SetCurrentCharacterDown(int key){ m_currentCharacterDown = key; }
};
extern InputSystem* s_theInputSystem;

///=====================================================
/// 
///=====================================================
inline bool InputSystem::GetKeyWentDown(int key) const{
	return DidStateJustChange(key) && IsKeyDown(key);
}

///=====================================================
/// 
///=====================================================
inline bool InputSystem::GetKeyWentUp(int key) const{
	return DidStateJustChange(key) && !IsKeyDown(key);
}

///=====================================================
/// 
///=====================================================
inline bool InputSystem::IsKeyDown(int key) const{
	FATAL_ASSERT(key >= 0 && key < NUMBER_OF_VIRTUAL_KEYS);
	return m_keyStates[key].m_isKeyDown;
}

///=====================================================
/// 
///=====================================================
inline bool InputSystem::DidStateJustChange(int key) const{
	FATAL_ASSERT(key >= 0 && key < NUMBER_OF_VIRTUAL_KEYS);
	return m_keyStates[key].m_stateJustChanged;
}

///=====================================================
/// 
///=====================================================
inline void InputSystem::SetKeyState(int key, bool isPressedDown){
	FATAL_ASSERT(key >= 0 && key < NUMBER_OF_VIRTUAL_KEYS);
	if (m_keyStates[key].m_isKeyDown != isPressedDown){
		m_keyStates[key].m_stateJustChanged = true;
		m_keyStates[key].m_isKeyDown = isPressedDown;

		if (isPressedDown)
			m_anyKeyWentDown = true;
		else
			m_anyKeyWentUp = true;
	}
}

///=====================================================
/// 
///=====================================================
inline void InputSystem::SetMouseWheel(short z){
	if (z > 0)
		m_mouseWheelWentUp = true;
	else
		m_mouseWheelWentDown = true;
}

#endif