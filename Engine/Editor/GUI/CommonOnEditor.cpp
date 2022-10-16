//
//	CommonOnEditor.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 23/10/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

#ifdef _EDITOR_
namespace Engine {
	namespace Audio {
		// ------------------------------------------------------------------------
		/*! On Editor
		*
		*   Draws the Sound Emitter 3D Properties on the Editor
		*/ // ---------------------------------------------------------------------
		void SoundEmitter3D::OnEditor() {
			using __Texts::Engine::SoundEmitter3D::StaticText;
			Editor::UIPropertyDraw ui;

			AudioEmitter::OnEditor();
			ImGui::Separator();
			ui << std::pair{ StaticText::MinimunDistance, &mDists.first };
			ui << std::pair{ StaticText::MaximumDistance, &mDists.second };
			ui << std::pair{ StaticText::EnableDopplerEffect, &mDoppler };
		}

		// ------------------------------------------------------------------------
		/*! On Editor
		*
		*   Represents the Audio Emitter Information through the Editor
		*/ // ---------------------------------------------------------------------
		void AudioEmitter::OnEditor() {
			using __Texts::Engine::AudioComponent::StaticText;

			Editor::UIPropertyDraw ui;

			ui <<= {__Texts::Editor::MainState::StaticText::File, StaticText::ContentAudioID, &mProperties.mAudioFile};
			ui << std::pair{StaticText::PlayOnSpawn, & mProperties.mPlayOnSpawn};
			ImGui::SameLine(); ui << std::pair{ StaticText::Loop, &mProperties.mLoop };
			if (ui << std::pair{ StaticText::Volume, &mProperties.mVolume }) SetVolume(mProperties.mVolume);
			if (ui << std::pair{ StaticText::Pitch, &mProperties.mPitch }) SetPitch(mProperties.mPitch);
		}

		// ------------------------------------------------------------------------
		/*! On Editor
		*
		*   Toogles the doppler effect on the Listener
		*/ // ---------------------------------------------------------------------
		void SoundListener3D::OnEditor() {
			Editor::UIPropertyDraw ui;

			ui << std::pair{ __Texts::Engine::SoundEmitter3D::StaticText::EnableDopplerEffect, &mDoppler };
		}
	}
}
#endif