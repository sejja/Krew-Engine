//
//	InputManager.cpp
//	Good Neighbours
//
//	Created by Jon Ander Eceiza 17/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>
constexpr int c_sensibility = 5000;

namespace Engine {
	// ------------------------------------------------------------------------
	/*! Instance
	*
	*   Returns an instance of the Input Manager
	*/ // ---------------------------------------------------------------------
	InputManager& InputManager::Instance() noexcept 
	{
		static InputManager inptmgr;

		return inptmgr;
	}

	// ------------------------------------------------------------------------
	/*! Is Key Pressed
	*
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	bool InputManager::IsKeyPressed(const KeyCode key) noexcept 
	{
		const bool previousState = mKeyboardState[key];

		return ((mKeyboardState[key] = IsKeyDown(key)) && !previousState);
	}

	// ------------------------------------------------------------------------
	/*! Is Key Released
	*
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	bool InputManager::IsKeyReleased(const KeyCode key) noexcept 
	{
		const bool previousState = mKeyboardState[key];

		return (!(mKeyboardState[key] = IsKeyDown(key)) && previousState);
	}

	// ------------------------------------------------------------------------
	/*! Get Mouse Movement
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	Math::Vector2D InputManager::GetMouseMovement() const noexcept 
	{
		Math::Vector2D pos = GetWindowCoordinatesMousePosition();
		pos -= mMousePositon;
		return pos;
	}

	// ------------------------------------------------------------------------
	/*! Set Mouse Position
	*   Name says everything
	*/ // ---------------------------------------------------------------------
	void InputManager::SetMousePosition(const unsigned w, const unsigned h) const noexcept 
	{
		POINT p{static_cast<LONG>(w), static_cast<LONG>(h)};
		ClientToScreen(GetActiveWindow(), &p);
		SetCursorPos(p.x, p.y);
	}

	// ------------------------------------------------------------------------
	/*! Process Input
	*
	*   Updates the Input Information for the application
	*/ // ---------------------------------------------------------------------
	void InputManager::ProcessInput() noexcept 
	{
		for (KeyCode i = 0; i < 101; i++) mKeyboardState[i] = IsKeyDown(i);
		mMousePositon = GetWindowCoordinatesMousePosition();
	}

	// ------------------------------------------------------------------------
	/*! Get Window-Coordinates Mouse Position
	*
	*   Win32 Wrapper
	*/ // ---------------------------------------------------------------------
	Math::Vector2D InputManager::GetWindowCoordinatesMousePosition() const noexcept 
	{
		POINT p;
		GetCursorPos(&p);
		ScreenToClient(GetActiveWindow(), &p);

		return  { static_cast<float>(p.x), static_cast<float>(p.y) };
	}

	// ------------------------------------------------------------------------
	/*! Get Window-Coordinates Mouse Position
	*
	*   Win32 Wrapper
	*/ // ---------------------------------------------------------------------
	ControllerClass::ControllerClass(const unsigned short controller) noexcept :
		mControllerNumber(controller) 
	{
		ZeroMemory(&mControllerState, sizeof(XINPUT_STATE));
	}

	void ControllerClass::Update()
	{
		XInputGetState(mControllerNumber, &mPrevControllerState);
	}

	// ------------------------------------------------------------------------
	/*! Is Button Pressed
	*
	*   Checks whether a Button is pressed on a gamepad
	*/ // ---------------------------------------------------------------------
	bool ControllerClass::IsButtonPressed(const int to_check) noexcept 
	{
		XInputGetState(mControllerNumber, &mControllerState);

		return mControllerState.Gamepad.wButtons & to_check;
	}

	// ------------------------------------------------------------------------
	/*! Is Button Triggered
	*
	*   Checks whether a Button has been triggered on a gamepad
	*/ // ---------------------------------------------------------------------
	bool ControllerClass::IsButtonTriggered(const int to_check) noexcept
	{
		return (!(mPrevControllerState.Gamepad.wButtons & to_check) && IsButtonPressed(to_check));
	}

	// ------------------------------------------------------------------------
	/*! Is Button Released 
	*
	*   Checks whether a Button is released on a gamepad
	*/ // ---------------------------------------------------------------------
	bool ControllerClass::IsButtonReleased(const int to_check) noexcept 
	{
		return ((mPrevControllerState.Gamepad.wButtons & to_check) && !IsButtonPressed(to_check));
	}

	// ------------------------------------------------------------------------
	/*! Is Button Released
	*
	*   Checks whether a Button is up on a gamepad
	*/ // ---------------------------------------------------------------------
	bool ControllerClass::IsButtonUp(const int to_check) noexcept
	{
		return (!IsButtonPressed(to_check));
	}

	// ------------------------------------------------------------------------
	/*! Is Thumb Pressed
	*
	*   Checks whether a Left thumb is pressed on a gamepad
	*/ // ---------------------------------------------------------------------
	bool ControllerClass::IsLeftThumbPressed() noexcept
	{
		XInputGetState(mControllerNumber, &mControllerState);

		return mControllerState.Gamepad.bLeftTrigger & XINPUT_GAMEPAD_LEFT_THUMB;
	}

	// ------------------------------------------------------------------------
	/*! Is Thumb Triggered
	*
	*   Checks whether a Left thumb is triggered on a gamepad
	*/ // ---------------------------------------------------------------------
	bool ControllerClass::IsLeftThumbTriggered() noexcept
	{
		return (!(mPrevControllerState.Gamepad.bLeftTrigger & XINPUT_GAMEPAD_LEFT_THUMB) && IsLeftThumbPressed());
	}

	// ------------------------------------------------------------------------
	/*! Is Thumb Released
	*
	*   Checks whether a Left thumb has been released on a gamepad
	*/ // ---------------------------------------------------------------------
	bool ControllerClass::IsLeftThumbReleased() noexcept
	{
		return ((mPrevControllerState.Gamepad.bLeftTrigger & XINPUT_GAMEPAD_LEFT_THUMB)  && !IsLeftThumbPressed());
	}

	// ------------------------------------------------------------------------
	/*! Is Thumb Up
	*
	*   Checks whether a Left thumb is up on a gamepad
	*/ // ---------------------------------------------------------------------
	bool ControllerClass::IsLeftThumbUp() noexcept
	{
		return (!IsLeftThumbUp());
	}

	// ------------------------------------------------------------------------
	/*! Is Thumb Pressed
	*
	*   Checks whether a Right thumb is pressed on a gamepad
	*/ // ---------------------------------------------------------------------
	bool ControllerClass::IsRightThumbPressed() noexcept
	{
		XInputGetState(mControllerNumber, &mControllerState);

		return mControllerState.Gamepad.bRightTrigger & XINPUT_GAMEPAD_LEFT_THUMB;
	}

	// ------------------------------------------------------------------------
	/*! Is Thumb Triggered
	*
	*   Checks whether a Right thumb is triggered on a gamepad
	*/ // ---------------------------------------------------------------------
	bool ControllerClass::IsRightThumbTriggered() noexcept
	{
		return (!(mPrevControllerState.Gamepad.bRightTrigger & XINPUT_GAMEPAD_LEFT_THUMB) && IsRightThumbPressed());
	}

	// ------------------------------------------------------------------------
	/*! Is Thumb Released
	*
	*   Checks whether a Right thumb has been released on a gamepad
	*/ // ---------------------------------------------------------------------
	bool ControllerClass::IsRightThumbReleased() noexcept
	{
		return ((mPrevControllerState.Gamepad.bRightTrigger & XINPUT_GAMEPAD_LEFT_THUMB) && !IsRightThumbPressed());
	}

	// ------------------------------------------------------------------------
	/*! Is Thumb Up
	*
	*   Checks whether a Right thumb is up on a gamepad
	*/ // ---------------------------------------------------------------------
	bool ControllerClass::IsRightThumbUp() noexcept
	{
		return (!IsRightThumbUp());
	}

	// ------------------------------------------------------------------------
	/*! Get Left Stick Values
	*
	*   Gets the titlting values of the left stick
	*/ // ---------------------------------------------------------------------
	void ControllerClass::GetLeftStickValues(SHORT& leftX, SHORT& leftY) noexcept 
	{
		XInputGetState(mControllerNumber, &mControllerState);
		leftX = abs(mControllerState.Gamepad.sThumbLX) >= c_sensibility ? mControllerState.Gamepad.sThumbLX : 0;
		leftY = abs(mControllerState.Gamepad.sThumbLY) >= c_sensibility ? mControllerState.Gamepad.sThumbLY : 0;
	}

	// ------------------------------------------------------------------------
	/*! Get Right Stick Values
	*
	*   Gets the titlting values of the right stick
	*/ // ---------------------------------------------------------------------
	void ControllerClass::GetRightStickValues(SHORT& rightX, SHORT& rightY) noexcept 
	{
		XInputGetState(mControllerNumber, &mControllerState);
		rightX = abs(mControllerState.Gamepad.sThumbRX) >= c_sensibility ? mControllerState.Gamepad.sThumbRX : 0;
		rightY = abs(mControllerState.Gamepad.sThumbRY) >= c_sensibility ? mControllerState.Gamepad.sThumbRY : 0;
	}

	// ------------------------------------------------------------------------
	/*! Vibrate
	*
	*   Vibrates the controller given the magnitude of the vibration
	*/ // ---------------------------------------------------------------------
	void ControllerClass::Vibrate(const unsigned short left, const unsigned short right) noexcept
	{
		XINPUT_VIBRATION vibration;
		vibration.wLeftMotorSpeed = left;
		vibration.wRightMotorSpeed = right;
		XInputSetState(mControllerNumber, &vibration);
	}

	// ------------------------------------------------------------------------
	/*! Is Connected
	*
	*   Returns wether a certain controller is connected
	*/ // ---------------------------------------------------------------------
	bool ControllerClass::IsConnected() noexcept 
	{
		return XInputGetState(mControllerNumber, &mControllerState) == ERROR_SUCCESS;
	}
}