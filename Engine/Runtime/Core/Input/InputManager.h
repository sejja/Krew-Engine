//
//	InputManager.hpp
//	Good Neighbours
//
//	Created by Jon Ander Eceiza 17/09/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#ifndef _INPUT_MANAGER__H_
#define _INPUT_MANAGER__H_

namespace Engine {
    class ENGINE_API InputManager {
    public:
        using KeyCode = decltype(VK_ACCEPT);

        DONTDISCARD bool IsKeyPressed(const KeyCode key) noexcept;
        DONTDISCARD bool IsKeyReleased(const KeyCode key) noexcept;
        DONTDISCARD bool inline IsKeyDown(const KeyCode key) const noexcept;
        DONTDISCARD bool inline IsKeyUp(const KeyCode key) const noexcept;
        DONTDISCARD Engine::Math::Vector2D GetMouseMovement() const noexcept;
        DONTDISCARD Engine::Math::Vector2D inline GetMousePosition() const noexcept;
        void inline SetMousePosition(const Math::Vector2D& pos) const noexcept;
        void SetMousePosition(const unsigned w, const unsigned h) const noexcept;
        void ProcessInput() noexcept;
        DONTDISCARD static InputManager& Instance() noexcept;
    private:
        Math::Vector2D GetWindowCoordinatesMousePosition() const noexcept;
        bool mKeyboardState[101];
        Math::Vector2D mMousePositon;
    };

    class ENGINE_API ControllerClass {
    public:
        ControllerClass(const unsigned short controller) noexcept;

        DONTDISCARD bool IsButtonPressed(const int to_check) noexcept;
        DONTDISCARD bool IsButtonTriggered(const int to_check) noexcept;
        DONTDISCARD bool IsButtonReleased(const int to_check) noexcept;
        DONTDISCARD bool IsButtonUp(const int to_check) noexcept;

        DONTDISCARD bool IsLeftThumbPressed() noexcept;
        DONTDISCARD bool IsLeftThumbTriggered() noexcept;
        DONTDISCARD bool IsLeftThumbReleased() noexcept;
        DONTDISCARD bool IsLeftThumbUp() noexcept;

        DONTDISCARD bool IsRightThumbPressed() noexcept;
        DONTDISCARD bool IsRightThumbTriggered() noexcept;
        DONTDISCARD bool IsRightThumbReleased() noexcept;
        DONTDISCARD bool IsRightThumbUp() noexcept;

        DONTDISCARD bool IsConnected() noexcept;
        void Update();
        void Vibrate(const unsigned short left, const unsigned short right) noexcept;
        void GetLeftStickValues(SHORT& leftX, SHORT& leftY) noexcept;
        void GetRightStickValues(SHORT& rightX, SHORT& rightY) noexcept;
    private:
        int mControllerNumber;
        XINPUT_STATE mPrevControllerState;
        XINPUT_STATE mControllerState;
    };

    // ------------------------------------------------------------------------
    /*! Is Key Down
    *
    *   Name says everything
    */ // ---------------------------------------------------------------------
    bool inline InputManager::IsKeyDown(const KeyCode key) const noexcept {
        return (GetAsyncKeyState(key) & (1 << 16));
    }

    // ------------------------------------------------------------------------
    /*! Is Key Up
    *
    *   Name says everything
    */ // ---------------------------------------------------------------------
    bool inline InputManager::IsKeyUp(const KeyCode key) const noexcept {
        return !IsKeyDown(key);
    }

    // ------------------------------------------------------------------------
    /*! Set Mouse Position
    *   Name says everything
    */ // ---------------------------------------------------------------------
    void inline InputManager::SetMousePosition(const Math::Vector2D& pos) const noexcept {
        SetMousePosition(pos.x, pos.y);
    }

    // ------------------------------------------------------------------------
    /*! Get Mouse Position
    *   Name says everything
    */ // ---------------------------------------------------------------------
    Math::Vector2D inline InputManager::GetMousePosition() const noexcept {
        return mMousePositon;
    }
}

#define GInput (&Engine::InputManager::Instance())

#endif