//
//	SoundEmitter3D.h
//	Good Neighbours
//
//	Created by Biaggio Fillice on 08/10/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _SOUND_EMITTER_3D__H_
#define _SOUND_EMITTER_3D__H_

namespace Engine {
	namespace Audio {
		class SoundListener3D;

		class ENGINE_API SoundEmitter3D : public AudioEmitter {
#pragma region //Constructors & Destructors
		public:
			SoundEmitter3D() noexcept;
#pragma endregion

#pragma region //Functions
			virtual void BeginPlay() final override;
#ifdef _EDITOR_
			virtual void OnEditor() final override;
#endif
			virtual void Tick() noexcept final override;
			void FromJson(const json& val) final override;
			void ToJson(json& val) const final override;
#pragma endregion

#pragma region //Members
		private:
			bool mDoppler;
			std::pair<float, float> mDists;
			Math::Vector3D mLastPos;
#pragma endregion
		};
	}
}
#endif