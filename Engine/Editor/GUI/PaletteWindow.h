//
//	PaletteWindow.h
//	Good Neighbours
//
//	Created by Diego Revilla on 24/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _PALETTE_WINDOW__H_
#define _PALETTE_WINDOW__H_

#ifdef _EDITOR_
namespace Editor {
	class PaletteWindow : public IWindow {
#pragma region // Functions
	public:
		void Present() override final;
#pragma endregion
	};
}

#endif
#endif