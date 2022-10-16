//
//	SoundListener3D.h
//	Good Neighbours
//
//	Created by Biaggio Fillice on 08/10/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#ifndef _SOUND_LISTENER_3D__H_
#define _SOUND_LISTENER_3D__H_

namespace Engine {
	namespace Audio {
		class ENGINE_API SoundListener3D : public Component, public events::Listener {
#pragma region // Construtors & Destructors
		public:
			SoundListener3D() noexcept;
#pragma endregion

#pragma region //Functions
			virtual void inline BeginPlay() noexcept final override;
#ifdef _EDITOR_
			virtual void OnEditor() final override;
#endif
			virtual void Tick() noexcept final override;
			void FromJson(const json& val) final override;
			void ToJson(json& val) const final override;
#pragma endregion

#pragma region //Members
		private:
			int mListenerID;
			bool mDoppler;
			Math::Vector3D mLastPos;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Begin Play
		*
		*   Sets the last position to the current position
		*/ // ---------------------------------------------------------------------
		void inline SoundListener3D::BeginPlay() noexcept {
			mLastPos = GetOwner()->GetPosition();
		}
	}
}

#endif