//
//	Viewport.h
//	Good Neighbours
//
//	Created by Diego Revilla on 10/10/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifdef _EDITOR_
#ifndef _VIEWPORT__H_
#define _VIEWPORT__H_

namespace Editor {
	class ENGINE_API Viewport : public IWindow {
#pragma region // Declarations
		enum eViewportTextures {
			ePlayTex,
			ePauseTex,
			eSkipTex,
			eCount
		};
#pragma endregion

#pragma region //Constructors & Destructors
	public:
		Viewport();
#pragma endregion

#pragma region // Functions
		void Present() override;
		DONTDISCARD unsigned inline GetBuffer() const noexcept;
		DONTDISCARD bool inline IsViewport() const noexcept;
		DONTDISCARD Engine::Math::Vector2D inline GetViewportDimensions() const noexcept;
	private:
		void ResetSceneToPreviousScene() const;
		void RegisterSceneState();
		Engine::GameObject* picking();
#pragma endregion

#pragma region // Members
	private:
		Engine::Math::Vector2D mDimensions;
		nlohmann::json mTemporalScene;
		Engine::Assets::TResource<Engine::Graphics::Texture>* mUITextures[eViewportTextures::eCount];
		Engine::Graphics::Framebuffer mViewportBuffer;
		Engine::UniquePointer<Editor::Gizmo> mGizmo;
		std::pair <BYTE, BYTE > mPreviewFlags;
		const char* mPreviewText;
#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Get Buffer
	*
	*   Gets the Viewport FrameBuffer ID
	*/ // ---------------------------------------------------------------------
	unsigned inline Viewport::GetBuffer() const noexcept {
		return mViewportBuffer.GetFrameBufferID();
	}

	// ------------------------------------------------------------------------
   /*! Is Viewport
   *
   *   Gets wether we are rendering onto viewport or onto window
   */ // ---------------------------------------------------------------------
	bool inline Viewport::IsViewport() const noexcept {
		return !mPreviewFlags.first;
	}

	// ------------------------------------------------------------------------
   /*! Get Viewport Dimensions
   *
   *   Gets the Display dimensions the viewport is currently rendering onto
   */ // ---------------------------------------------------------------------
	Engine::Math::Vector2D inline Viewport::GetViewportDimensions() const noexcept {
		return mDimensions;
	}
}

#endif
#endif