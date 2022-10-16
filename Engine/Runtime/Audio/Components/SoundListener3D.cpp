//
//	SoundListener3D.cpp
//	Good Neighbours
//
//	Created by Biaggio Fillice on 08/10/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

namespace Engine {
	namespace Audio {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Constructs a 3D Sound Listener
		*/ // ---------------------------------------------------------------------
		SoundListener3D::SoundListener3D() noexcept : 
			mListenerID(0), mDoppler(true), mLastPos() {}

		// ------------------------------------------------------------------------
		/*! Tick
		*
		*   Calculates the 3D Context for the listener
		*/ // ---------------------------------------------------------------------
		void SoundListener3D::Tick() noexcept {
			const GameObject* const owner = GetOwner();
			
			// use rotation to create rotation matrix to get forward and up vectors
			glm::vec3 forward(0.f, 0.f, 1.f);
			glm::vec3 up(0.f, 1.f, 0.f);
			glm::vec3 rot = owner->GetRotation();
			glm::mat4 identity(1.f);
			glm::mat4 rotMtx = glm::rotate(identity, glm::radians(rot.x), { 1.f, 0.f, 0.f }) *
				glm::rotate(identity, glm::radians(rot.y), up) * glm::rotate(identity, glm::radians(rot.z), forward);
			forward = rotMtx * glm::vec4(forward,0.0f);
			up = rotMtx * glm::vec4(up, 0.0f);

			Math::Vector3D pos = owner->GetPosition();
			auto& audio = *GAudio;
			
			//If we changed our position this frame
			//if (mDoppler && mLastPos != pos) {
			//	glm::vec3 velocity = (pos - mLastPos) * (1000.0f * GFrames->GetFrameTime());
			//	audio.Set3DListener(mListenerID, pos, velocity, forward, up);
			//} else
				audio.Set3DListener(mListenerID, pos, glm::vec3(0.0f), forward, up);
			mLastPos = pos;
		}
	}
}