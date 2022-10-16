//
//	ResourceWindow.h
//	Good Neighbours
//
//	Created by Diego Revilla on 19/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifdef _EDITOR_
#ifndef _RESOURCE_WINDOW__H_
#define _RESOURCE_WINDOW__H_

namespace Editor {
	class ResourceWindow : public IWindow {
#pragma region // Functions
	public:
		void Present() noexcept override final;
#pragma endregion
	};
}

#endif
#endif