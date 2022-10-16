//
//	GraphicsManager.h
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _GFX_MGR__H_
#define _GFX_MGR__H_

namespace Engine {
	class GraphicsManager :public Singleton<GraphicsManager> {
#pragma region // Functions
	public:
		void Initialize(const HWND& hwnd) noexcept;
		void Load() noexcept;
		void Render() noexcept;
		void Unload() const noexcept;
		void Present() const noexcept;
		void inline SetClearColor(const Graphics::Color& col) noexcept;
#pragma endregion

#pragma region // Members
		HDC mDeviceContext = 0;
		HGLRC mRenderContext = 0;
#pragma endregion
	};

	// ------------------------------------------------------------------------
	/*! Set Clear Color
	*
	*   Sets the buffer clear color
	*/ // ---------------------------------------------------------------------
	void inline GraphicsManager::SetClearColor(const Graphics::Color& col) noexcept {
		glClearColor(col.r, col.g, col.b, col.a);
	}
}

#define GGfxMan (&Engine::GraphicsManager::Instance())

#endif