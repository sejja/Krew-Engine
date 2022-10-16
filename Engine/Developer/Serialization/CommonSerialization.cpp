//
//	CommonSerialization.cpp
//	Good Neighbours
//
//	Created by Diego Revilla 26/10/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

namespace Engine {
	namespace Audio {
		// ------------------------------------------------------------------------
		/*! From Json
		*
		*   Converts a JSON onto an Audio Emitter
		*/ // ---------------------------------------------------------------------
		void AudioEmitter::FromJson(const json& val) {
			using __Texts::Engine::AudioComponent::StaticText;

			fetch(mProperties.mAudioFile, val, __Texts::Editor::MainState::StaticText::File);
			fetch(mProperties.mLoop, val, StaticText::Loop);
			fetch(mProperties.mVolume, val, StaticText::Volume);
			fetch(mProperties.mPitch, val, StaticText::Pitch);
			fetch(mProperties.mPlayOnSpawn, val, StaticText::PlayOnSpawn);
		}

		// ------------------------------------------------------------------------
		/*! From Json
		*
		*   Converts a JSON onto an Audio Emitter
		*/ // ---------------------------------------------------------------------
		void AudioEmitter::ToJson(json& val) const {
			using __Texts::Engine::AudioComponent::StaticText;

			val[__Texts::Editor::MainState::StaticText::File] = mProperties.mAudioFile;
			val[StaticText::Loop] = mProperties.mLoop;
			val[StaticText::Volume] = mProperties.mVolume;
			val[StaticText::Pitch] = mProperties.mPitch;
			val[StaticText::PlayOnSpawn] = mProperties.mPlayOnSpawn;
		}

		// ------------------------------------------------------------------------
		/*! From Json
		*
		*   Converts a JSON onto an SoundEmitter3D
		*/ // ---------------------------------------------------------------------
		void SoundEmitter3D::FromJson(const json& val) {
			using __Texts::Engine::SoundEmitter3D::StaticText;

			AudioEmitter::FromJson(val);

			fetch(mDists.first, val, StaticText::MinimunDistance);
			fetch(mDists.second, val, StaticText::MaximumDistance);
			fetch(mDoppler, val, StaticText::EnableDopplerEffect);
		}

		// ------------------------------------------------------------------------
		/*! From Json
		*
		*   Converts a JSON onto an SoundEmitter3D
		*/ // ---------------------------------------------------------------------
		void SoundEmitter3D::ToJson(json& val) const {
			using __Texts::Engine::SoundEmitter3D::StaticText;

			AudioEmitter::ToJson(val);
			val[StaticText::MinimunDistance] = mDists.first;
			val[StaticText::MaximumDistance] = mDists.second;
			val[StaticText::EnableDopplerEffect] = mDoppler;
		}

		// ------------------------------------------------------------------------
		/*! From Json
		*
		*   Converts a JSON onto an SoundListener3D
		*/ // ---------------------------------------------------------------------
		void SoundListener3D::FromJson(const json& val) {
			fetch(mDoppler, val, __Texts::Engine::SoundEmitter3D::StaticText::EnableDopplerEffect);
		}

		// ------------------------------------------------------------------------
		/*! From Json
		*
		*   Converts a JSON onto an SoundListener3D
		*/ // ---------------------------------------------------------------------
		void SoundListener3D::ToJson(json& val) const {
			val[__Texts::Engine::SoundEmitter3D::StaticText::EnableDopplerEffect] = mDoppler;
		}
	}
}