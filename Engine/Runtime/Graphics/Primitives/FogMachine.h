#ifndef _FOG_MACHINE__H_
#define _FOG_MACHINE__H_

namespace Engine {
	namespace Graphics {
		class FogComponent;

		class FogMachine {
		public:
			void ComputeFog();
			Array<FogComponent*> mAllFogs;
		};
	}
}

#endif