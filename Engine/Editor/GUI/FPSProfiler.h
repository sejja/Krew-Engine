//
//  FPSProfiler.h
//  Good Neighbours
//
//  Created by Diego Revilla 01/10/21.
//  Copyright � 2021 Diego Revilla. All rights reserved.
//

#ifndef _FPS_PROFILER__H_
#define _FPS_PROFILER__H_

#ifdef _EDITOR_
namespace Editor {
	class FPSProfiler : public Editor::IWindow {
#pragma region // Functions
		void Present() noexcept override;
#pragma endregion
	};
}
#endif
#endif