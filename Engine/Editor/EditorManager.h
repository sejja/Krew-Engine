//
//	Editor.h
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#ifdef _EDITOR_
#ifndef _EDITOR_MANAGER__H_
#define _EDITOR_MANAGER__H_

namespace Editor {
	class Viewport;

	class ENGINE_API ImGuiManager : public Singleton<ImGuiManager> {
#pragma region // Declarations
		template<typename T> friend class Singleton;
		friend class Viewport;

		struct DebugText {
			std::string mText;
			Engine::Graphics::Color mColor;
			std::pair<float, float> mDurations;
		};
#pragma endregion

#pragma region //Constructors & Destructors
		ImGuiManager();
#pragma endregion

#pragma region //Methods
	public:
		void Initialize();
		void Shutdown();
		void ShowGUI();
		DONTDISCARD bool inline IsPlaying() const noexcept;
		void SetIsPlaying(const bool b) noexcept;
		DONTDISCARD bool inline IsPaused() const noexcept;
		void SetPaused(const bool b) noexcept;
		std::list<Engine::GameObject*> inline& GetSelectedObject() noexcept;
		void inline SetSelectedObject(Engine::GameObject* const obj) noexcept;
		DONTDISCARD GLuint GetBuffer() noexcept;
		DONTDISCARD Engine::Math::Vector2D GetViewportDimensions() noexcept;
		DONTDISCARD bool IsViewport() noexcept;
		void inline PrintDebugMessage(const std::string& text, const Engine::Graphics::Color& col, const float dur = 1.f);
		DONTDISCARD bool inline RenderInvisible() const noexcept;
		DONTDISCARD bool inline RenderWireframe() const noexcept;
		DONTDISCARD bool inline RenderBoundingBoxes() const noexcept;
		DONTDISCARD unsigned char inline GetRenderOption() const noexcept;
		void inline ShouldRenderInvisible(const bool b) noexcept;
		void inline ShouldRenderWirefrane(const bool b) noexcept;
		void inline ShouldRenderBoundingBoxes(const bool b) noexcept;
		void inline ClearSelectedObjects() noexcept;
#pragma endregion

#pragma region // Members
	private:
		void RenderDebugText();
		void Dockspace() const;
		Engine::Array<DebugText> mMessages;
		Engine::Array<Engine::UniquePointer<IEditorState>> mStates;
		std::list<Engine::GameObject*> mSelectedObject;
		std::pair<bool, bool> mPlayFlags;
		unsigned char mRenderOption;
		std::tuple<bool, bool, bool> mRenderFlags;
#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Is Playing
	*
	*   Retrieves is the Editor is currently running or not
	*/ // ---------------------------------------------------------------------
	bool inline Editor::ImGuiManager::IsPlaying() const noexcept {
		return mPlayFlags.first;
	}

	// ------------------------------------------------------------------------
	/*! Set Is Playing
	*
	*   Returns a the current Editor Selected Object
	*/ // ---------------------------------------------------------------------
	void inline ImGuiManager::SetIsPlaying(const bool b) noexcept {
		mPlayFlags.first = b;
	}

	// ------------------------------------------------------------------------
	/*! Is Paused
	*
	*   Retrieves is the Editor is currently on pause mode
	*/ // ---------------------------------------------------------------------
	bool inline Editor::ImGuiManager::IsPaused() const noexcept {
		return mPlayFlags.second;
	}

	// ------------------------------------------------------------------------
	/*! Set Is Paused
	*
	*   Set the editor on pause state
	*/ // ---------------------------------------------------------------------
	void inline ImGuiManager::SetPaused(const bool b) noexcept {
		mPlayFlags.second = b;
	}

	// ------------------------------------------------------------------------
	/*! Get Selected Object
	*
	*   Returns a the current Editor Selected Object
	*/ // ---------------------------------------------------------------------
	std::list<Engine::GameObject*> inline& ImGuiManager::GetSelectedObject() noexcept {
		return mSelectedObject;
	}

	// ------------------------------------------------------------------------
	/*! Set Selected Object
	*
	*   Selects a new Object
	*/ // ---------------------------------------------------------------------
	void inline ImGuiManager::SetSelectedObject(Engine::GameObject* const obj) noexcept {
		mSelectedObject.push_back(obj);
	}

	void inline ImGuiManager::ClearSelectedObjects() noexcept {
		mSelectedObject.clear();
	}

	// ------------------------------------------------------------------------
	/*! Print Debug Message
	*
	*   Prints a debug message onto screen for an specified amount of seconds
	*/ // ---------------------------------------------------------------------
	void inline ImGuiManager::PrintDebugMessage(const std::string& text, const Engine::Graphics::Color& col, const float dur) {
		mMessages.push_back({ text, col, {dur, 0} });
	}

	// ------------------------------------------------------------------------
	/*! Render Invisible
	*
	*   Prints a debug message onto screen for an specified amount of seconds
	*/ // ---------------------------------------------------------------------
	bool inline Editor::ImGuiManager::RenderInvisible() const noexcept {
		return std::get<0>(mRenderFlags);
	}

	// ------------------------------------------------------------------------
	/*! Print Debug Message
	*
	*   Prints a debug message onto screen for an specified amount of seconds
	*/ // ---------------------------------------------------------------------
	bool inline ImGuiManager::RenderWireframe() const noexcept {
		return  std::get<1>(mRenderFlags);
	}

	// ------------------------------------------------------------------------
	/*! Print Debug Message
	*
	*   Prints a debug message onto screen for an specified amount of seconds
	*/ // ---------------------------------------------------------------------
	bool inline ImGuiManager::RenderBoundingBoxes() const noexcept {
		return  std::get<2>(mRenderFlags);
	}

	// ------------------------------------------------------------------------
	/*! Should Render Invisible
	*
	*  Marks Invisible Objects to be rendered on Edit Mode
	*/ // ---------------------------------------------------------------------
	void inline ImGuiManager::ShouldRenderInvisible(const bool b) noexcept {
		std::get<0>(mRenderFlags) = b;
	}

	// ------------------------------------------------------------------------
	/*! Should Render Wireframe
	*
	*  Marks the Scene to be rendered on wireframe mode
	*/ // ---------------------------------------------------------------------
	void inline ImGuiManager::ShouldRenderWirefrane(const bool b) noexcept {
		std::get<1>(mRenderFlags) = b;
	}

	// ------------------------------------------------------------------------
	/*! Should Render Wireframe
	*
	*  Marks the Scene to be rendered on wireframe mode
	*/ // ---------------------------------------------------------------------
	void inline ImGuiManager::ShouldRenderBoundingBoxes(const bool b) noexcept {
		std::get<2>(mRenderFlags) = b;
	}

	// ------------------------------------------------------------------------
	/*! Get Render Option
	*
	*  Gets the Render Mode we are aplying
	*/ // ---------------------------------------------------------------------
	unsigned char inline ImGuiManager::GetRenderOption() const noexcept {
		return mRenderOption;
	}
}

ENGINE_API Editor::ImGuiManager& get_gui();

#define GEditor (&get_gui())

#endif
#endif