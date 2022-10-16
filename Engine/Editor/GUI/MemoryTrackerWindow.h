//
//	MemoryTrackerWindow.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 19/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifdef _EDITOR_
#ifndef _MEMORY_TRACKER_WINDOW__H_
#define _MEMORY_TRACKER_WINDOW__H_

namespace Editor {
	class MemoryTrackerWindow : public IWindow {
	public:
		void Present() override;
	};
}

#endif
#endif