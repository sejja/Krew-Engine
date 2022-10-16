#ifndef _CREATOR__H_
#define _CREATOR__H_

namespace Engine {
	class IBase;
}

namespace Editor {
	namespace __private {
		struct GuiCreator {
			virtual Engine::IBase* Create() const = 0;
		};
	}
}

#endif