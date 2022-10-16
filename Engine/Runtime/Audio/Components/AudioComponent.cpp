//
//	AudioComponent.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 19/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

namespace Engine {
	namespace Audio {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Constructs an Audio Emitter properties
		*/ // ---------------------------------------------------------------------
		AudioEmitter::AudioEmitter() noexcept :
			mProperties{ false, true, 0.8f, 1.f } {}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Destroys an Audio Emitter and frees the voice
		*/ // ---------------------------------------------------------------------
		AudioEmitter::~AudioEmitter() {
			mVoice.SetPause(true);
			GAudio->FreeVoice(&mVoice);
		}

		// ------------------------------------------------------------------------
		/*! Begin Play
		*
		*   Loads the actual audio file
		*/ // ---------------------------------------------------------------------
		void AudioEmitter::BeginPlay() {
			//If we have a file to load
			if (!mProperties.mAudioFile.empty()) {
				mSound = GContent->GetResource<Sound>(mProperties.mAudioFile.c_str(), Assets::IResourceImporter::eImportMode::eInmediate);
				if (mProperties.mPlayOnSpawn && mSound && mSound.get()->Get()) Play();
				mVoice.SetLoop(mProperties.mLoop);
				mVoice.SetVolume(mProperties.mVolume);
				mVoice.SetPitch(mProperties.mPitch);
			}
		}

#ifdef _EDITOR_
		// ------------------------------------------------------------------------
		/*! Editor Tick
		*
		*   Ticks on Editor Mode
		*/ // ---------------------------------------------------------------------
		void AudioEmitter::EditorTick() {
			auto& editor = *GEditor;

			//If we are not playing and we want to render invisible objects
			if (!editor.IsPlaying() && editor.RenderInvisible())
				GDebug->DebugDrawAABB(GetOwner()->GetWorldPosition(), { 2, 2, 2 }, { 1, 0, 0, 1 });
		}
#endif

		// ------------------------------------------------------------------------
		/*! Set Loop
		*
		*   Makes the Audio Component to Loop the track once it's finished
		*/ // ---------------------------------------------------------------------
		void AudioEmitter::SetLoop(const bool b) noexcept {
			mProperties.mLoop = b;
			mVoice.SetLoop(mProperties.mLoop);
		}

		// ------------------------------------------------------------------------
		/*! Set Volume
		*
		*   Sets the Track volume
		*/ // ---------------------------------------------------------------------
		void AudioEmitter::SetVolume(const float f) noexcept {
			mProperties.mVolume = f;
			mVoice.SetVolume(mProperties.mVolume);
		}

		// ------------------------------------------------------------------------
		/*! Set Pitch
		*
		*   Sets the Track pitch
		*/ // ---------------------------------------------------------------------
		void AudioEmitter::SetPitch(const float p) noexcept {
			mProperties.mPitch = p;
			mVoice.SetPitch(p);
		}

		// ------------------------------------------------------------------------
		/*! Set Audio File
		*
		*   Sets the track to play, given a filename
		*/ // ---------------------------------------------------------------------
		void AudioEmitter::SetAudioFile(const char* filePath) noexcept
		{
			if (filePath != nullptr)
				mProperties.mAudioFile = filePath;
		}

		void AudioEmitter::SetPlayOnSpawn(const bool p) noexcept
		{
			mProperties.mPlayOnSpawn = p;
		}
	}
}