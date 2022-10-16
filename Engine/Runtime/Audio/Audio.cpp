//
//  Audio.cpp
//  Good Neighbours
//
//  Created by Diego Revilla on 28/3/20.
//  Copyright � 2020 Diego Revilla. All rights reserved.
//

#include <Shared.h>

constexpr const size_t MAX_VOICE_COUNT = 1000;

namespace Engine {
	namespace Audio {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Creates the FMOD Context
		*/ // ---------------------------------------------------------------------
		AudioManager::AudioManager() :
			mData{ nullptr, nullptr } {
			FMOD_RESULT result = FMOD::System_Create(&mData.mFMOD);

			//if we were not able to initialize FMOD
			assertThis(!result, "Could not create FMOD Context");

			// Initialize FMOD
			result = mData.mFMOD->init(MAX_VOICE_COUNT, FMOD_INIT_NORMAL, 0);

			assertThis(!result, "Could not initialize Sound System");

			AllocateVoices();
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Releases FMOD and all the voices
		*/ // ---------------------------------------------------------------------
		AudioManager::~AudioManager() {
			FreeVoices();
			mData.mFMOD->release();
		}

		// ------------------------------------------------------------------------
		/*! Update
		*
		*   Updates the Audio Manager
		*/ // --------------------------------------------------------------------
		void AudioManager::Update() {
			mData.mFMOD->update();

			// loop through the voices
			for (auto it = mUsedVoices.begin(), endx = mUsedVoices.end(); it != endx; ++it) 
			{
				auto& v = *it;
				
				v->SetVolume(v->GetVolume() * mMasterVol);
				// this voice is no longer playing
				if (!v->IsValid()) {
					mFreeVoices.push_back(*it);
					it = mUsedVoices.erase(it);
					endx = mUsedVoices.end();

					//if we are at the end of our voice list
					if (it == endx) break;
				}
			}
		}

		// ------------------------------------------------------------------------
		/*! Free Voice
		*
		*   Free�s a voice
		*/ // ---------------------------------------------------------------------
		void AudioManager::FreeVoice(Voice* pVoice) {
			assertThis(pVoice, "Can't free a null voice");
			pVoice->GetChannel()->stop();
			mUsedVoices.remove(pVoice);
			mFreeVoices.push_back(pVoice);
		}

		// ------------------------------------------------------------------------
		/*! Get Free Voice
		*
		*   Returns a free voice
		*/ // --------------------------------------------------------------------
		DONTDISCARD Voice* AudioManager::GetFreeVoice() {
			//if we do have an FMOD system and the free voices list is not empty
			if (!mFreeVoices.empty()) {
				Voice* const pv = mFreeVoices.back();
				mFreeVoices.pop_back();
				mUsedVoices.push_back(pv);

				return pv;
			}

			return nullptr;
		}

		// ------------------------------------------------------------------------
		/*! Allocate Voices
		*
		*   Allocates the Voices that we might need
		*/ // --------------------------------------------------------------------
		void AudioManager::AllocateVoices() {
			FreeVoices();
			mData.mVoices = mVoiceAlloc.New(MAX_VOICE_COUNT);
			FForEach(mData.mVoices, mData.mVoices + MAX_VOICE_COUNT, [this](Voice& v) {mFreeVoices.push_back(&v); });
		}

		// ------------------------------------------------------------------------
		/*! Free Voices
		*
		*   Free all the voices within the Audio Manager
		*/ // --------------------------------------------------------------------
		void AudioManager::FreeVoices() {
			//If we have available voices
			if (mData.mVoices) {
				mVoiceAlloc.deallocate(mData.mVoices);
				mFreeVoices.clear();
				mUsedVoices.clear();
				FForEach(mSoundGroups.begin(), mSoundGroups.end(), [](std::pair<const std::string, SoundGroup>& s) {s.second.Clear(); });
				mData.mVoices = nullptr;
			}
		}

		// ------------------------------------------------------------------------
		/*! Create Sound
		*
		*   Creates a sound from a file
		*/ // --------------------------------------------------------------------
		Sound* AudioManager::CreateSound(const char* filename) {
			Sound* const mSound = mSoundAlloc.allocate();
			mSoundAlloc.construct(mSound);
			mData.mFMOD->createSound(filename, FMOD_LOOP_NORMAL | FMOD_3D, 0, &mSound->mSound);

			// error check
			if (!**mSound) {
				mSoundAlloc.destroy(mSound);
				mSoundAlloc.deallocate(mSound);
				return nullptr;
			}

			return mSound;
		}

		// ------------------------------------------------------------------------
		/*! Free Sound
		*
		*   Frees a sound from our Audio Manager
		*/ // --------------------------------------------------------------------
		void AudioManager::FreeSound(Sound* mSound) {
			if (mSound == nullptr)
				return;

			(**mSound)->release();
			mSoundAlloc.destroy(mSound);
			mSoundAlloc.deallocate(mSound);
		}

		// ------------------------------------------------------------------------
		/*! Play
		*
		*   Plays a certain sound
		*/ // --------------------------------------------------------------------
		void AudioManager::Play(const Sound* mSound, const bool paused, Voice& outvoice) {
			assertThis(mSound, "Can't play null sound");

			std::size_t freevoice = GetFreeVoiceCount();

			if (!freevoice)
				RequestMoreVoices(1000);
			Voice* const pVoice = GetFreeVoice();

			pVoice->SetClock(std::chrono::steady_clock::now());
			mData.mFMOD->playSound(**mSound, 0, paused, &pVoice->GetChannel());
			pVoice->SetPause(paused);
			pVoice->SetLoop(false);
			outvoice = *pVoice;
		}

		AudioManager& AudioManager::Instance()
		{
			static AudioManager am;
			return am;
		}

		// ------------------------------------------------------------------------
		/*! Loop
		*
		*   Loops a Voice
		*/ // --------------------------------------------------------------------
		void AudioManager::Loop(Voice* pVoice) {
			assertThis(pVoice, "Can't loop null voice");
			pVoice->SetLoop(true);
		}

		// ------------------------------------------------------------------------
		/*! Stop All
		*
		*   Stops All Sound
		*/ // ---------------------------------------------------------------------
		void AudioManager::StopAll() {
			// loop through the voices
			while (!mUsedVoices.empty()) {
				mUsedVoices.back()->GetChannel()->stop();
				mFreeVoices.push_back(mUsedVoices.back());
				mUsedVoices.pop_back();
			}
		}

		// ------------------------------------------------------------------------
		/*! Set 3D Listener
		*
		*   Sets the position of the 3D listener
		*/ // ---------------------------------------------------------------------
		void AudioManager::Set3DListener(const int listener, const Math::Vector3D& pos, const Math::Vector3D& vel,
			const Math::Vector3D& forward, const Math::Vector3D& up) const {
			const FMOD_VECTOR posF{ pos.x, pos.y, pos.z }, velF{ vel.x, vel.y, vel.z }, fwd{ forward.x, forward.y, forward.z },
				upF{ up.x, up.y, up.z };
			mData.mFMOD->set3DListenerAttributes(listener, &posF, &velF, &fwd, &upF);
		}
		void AudioManager::RequestMoreVoices(std::size_t msize) {
			mData.mVoices = mVoiceAlloc.New(msize);
			FForEach(mData.mVoices, mData.mVoices + msize, [this](Voice& v) {mFreeVoices.push_back(&v); });
		}
	}
}