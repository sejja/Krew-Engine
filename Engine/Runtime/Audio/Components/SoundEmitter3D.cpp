//
//	SoundEmitter3D.cpp
//	Good Neighbours
//
//	Created by Biaggio Fillice on 08/10/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include "Shared.h"
namespace Engine {
	namespace Audio {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Initializes inherited Audio Emitter and inner 3D Parameters
		*/ // ---------------------------------------------------------------------
		SoundEmitter3D::SoundEmitter3D() noexcept :
			AudioEmitter(), mDoppler(false), mDists{ 1.0f, 10000.0f }, mLastPos() {}

		// ------------------------------------------------------------------------
		/*! Begin Play
		*
		*   Retrieves actor position and Asks for the Audio Emitter to Initialize the Audio Track
		*/ // ---------------------------------------------------------------------
		void SoundEmitter3D::BeginPlay() {
			AudioEmitter::BeginPlay();
			mLastPos = GetOwner()->GetPosition();
		}

		// ------------------------------------------------------------------------
		/*! Tick
		*
		*   Updates all the 3D Parameters and tweaks the output sound
		*/ // ---------------------------------------------------------------------
		void SoundEmitter3D::Tick() noexcept {
			const Asset<Sound> sound = GetSound().lock();
			Sound const* soundSoure = sound->Get();

			if(sound && **soundSoure) 
				(**soundSoure)->set3DMinMaxDistance(mDists.first, mDists.second);

			Math::Vector3D posE = GetOwner()->GetPosition();
			const FMOD_VECTOR ownPos{ posE.x, posE.y , posE.z };
			auto channel = GetVoice().GetChannel();

			//Avoid unnecesary recomputation
			//if (mDoppler && mLastPos != posE) {
			//	mLastPos *= (1000.0f * GFrames->GetFrameTime());
			//	posE -= mLastPos;
			//
			//	// calculate velocity for doppler effect
			//	FMOD_VECTOR velocity{ posE.x, posE.y, posE.z };
			//	channel->set3DAttributes(&ownPos, &velocity);
			//} else
				channel->set3DAttributes(&ownPos, nullptr);

			mLastPos += posE;
		}
	}
}