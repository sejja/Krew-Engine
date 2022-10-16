//
//  MasterAudio.cpp
//  Good Neighbours
//
//  Created by Diego Revilla on 02/11/21.
//  Copyright � 2021 Diego Revilla. All rights reserved.
//

#include <Shared.h>

namespace Engine {
	namespace Audio {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Default Constructor for the Sound Class
		*/ // --------------------------------------------------------------------
		Sound::Sound() noexcept
			: mSound(NULL) {}

		// ------------------------------------------------------------------------
		/*! Constructor
		*
		*   The constructor for the Voice
		*/ // ---------------------------------------------------------------------
		Voice::Voice() noexcept
			: mChannel(nullptr), mGroup(nullptr), mClock() {}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Destroys a Voice
		*/ // --------------------------------------------------------------------
		Voice::~Voice() {
			//If we are in a group
			if (mGroup) mGroup->RemoveVoice(*this);
		}

		// ------------------------------------------------------------------------
		/*! Set Volume
		*
		*   Sets the volume of a channel
		*/ // --------------------------------------------------------------------
		void Voice::SetVolume(const float vol) noexcept {
			//if the Channel exists
			if (mChannel) mChannel->setVolume(Math::Clamp(vol, 0.f, 1.f));
		}

		// ------------------------------------------------------------------------
		/*! Set Pause
		*
		*   Sets a channel to pause
		*/ // --------------------------------------------------------------------
		void Voice::SetPause(const bool paused) const noexcept {
			//if we have a valid channel
			if (mChannel) mChannel->setPaused(paused);
		}

		// ------------------------------------------------------------------------
		/*! Is Paused
		*
		*   Returns whether a voice is paused or not
		*/ // --------------------------------------------------------------------
		bool Voice::IsPaused() const noexcept{
			//if we have a valid channel
			if (mChannel) {
				bool _paused;

				mChannel->getPaused(&_paused);

				return _paused;
			}

			return true;
		}

		// ------------------------------------------------------------------------
		/*! Set Loop
		*
		*   Sets a channel to loop or stops looping
		*/ // --------------------------------------------------------------------
		void  Voice::SetLoop(const bool isLoop) const noexcept {
			//if we have a valid channel
			if (mChannel) mChannel->setLoopCount(isLoop ? -1 : 0);
		}

		// ------------------------------------------------------------------------
		/*! Is Looping
		*
		*   Returns whether a voice is looping or not
		*/ // --------------------------------------------------------------------
		bool Voice::IsLooping() const noexcept {
			//if we have a valid channel
			if (mChannel) {
				int loop;

				mChannel->getLoopCount(&loop);

				return loop;
			}

			return false;
		}

		// ------------------------------------------------------------------------
		/*! Set Pitch
		*
		*   Sets the pitch of a channel
		*/ // --------------------------------------------------------------------
		void Voice::SetPitch(const float pitch) const noexcept {
			//if we have a valid channel
			if (mChannel) mChannel->setPitch(1.0f);
		}

		// ------------------------------------------------------------------------
		/*! Get Pitch
		*
		*   Returns whether a channel has a certain pitch or not
		*/ // --------------------------------------------------------------------
		float Voice::GetPitch() const noexcept {
			//if we have a valid channel
			if (mChannel) {
				float _pitch;

				mChannel->getPitch(&_pitch);

				return _pitch;
			}

			return 0;
		}

		// ------------------------------------------------------------------------
		/*! GetElapsedTime
		*
		*   Reutrns the time the voice has been played
		*
		*/ // --------------------------------------------------------------------
		std::chrono::steady_clock::time_point Voice::GetClock() const noexcept
		{
			return mClock;
		}

		// ------------------------------------------------------------------------
		/*! GetElapsedTime
		*
		*   Increment elapsed time by a constant value
		*
		*/ // --------------------------------------------------------------------
		void Voice::SetClock(std::chrono::steady_clock::time_point dt) noexcept
		{
			mClock = dt;
		}

		// ------------------------------------------------------------------------
		/*! Is Valid
		*
		*   Returns whether a voice is valid or not (Valid = Playing and existing,
		*		Not Valid = Not existing or not playing)
		*/ // --------------------------------------------------------------------
		bool Voice::IsValid() noexcept {
			
			if (mChannel)
			{
				bool pl;
				mChannel->isPlaying(&pl);

				//later on
				FMOD::Sound* pSound;
				mChannel->getCurrentSound(&pSound);

				//Compute elapsed time
				float time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - mClock).count(); //elapsed time

				//Check if time greater than sound length
				//If true, release the sound for memory friendly usage, and return false
				unsigned soundLength;
				pSound->getLength(&soundLength, FMOD_TIMEUNIT_MS);
			
				if (time >= soundLength && !IsLooping())
					return false;
				else if (time >= soundLength && IsLooping())
				{
					mClock = std::chrono::high_resolution_clock::now();
					return true;
				}

				return pl;
			}

			return false;
		}

		// ------------------------------------------------------------------------
		/*! Get Volume
		*
		*   Gets the volume of a channel
		*/ // --------------------------------------------------------------------
		float Voice::GetVolume() const noexcept {
			//if we have a valid channel
			if (mChannel) {
				float _pitch;

				mChannel->getVolume(&_pitch);

				return _pitch;
			}

			return 0;
		}

		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Sets the SoundGroup to default settings
		*/ // ---------------------------------------------------------------------
		SoundGroup::SoundGroup() noexcept :
			mGroupVolume(1.f), mGroupPitch(1.f) {}

		// ------------------------------------------------------------------------
		/*! Set Group Volume
		*
		*   Sets the Volume of a group
		*/ // ---------------------------------------------------------------------
		void SoundGroup::SetGroupVolume(const float volume) noexcept{
			mGroupVolume = Math::Clamp(volume, 0.f, 1.f);
			FForEach(mVoices.begin(), mVoices.end(), [this](Voice*& v) {v->SetVolume(v->GetVolume() * mGroupVolume); });
		}

		// ------------------------------------------------------------------------
		/*! Set Group Pitch
		*
		*   Sets the Pitch of a group
		*/ // --------------------------------------------------------------------
		void SoundGroup::SetGroupPitch(const float pitch) noexcept {
			mGroupPitch = 1.0f;
			FForEach(mVoices.begin(), mVoices.end(), [this](Voice*& v) {v->SetPitch(v->GetPitch() * mGroupVolume); });
		}

		// ------------------------------------------------------------------------
		/*! Add Voice
		*
		*   Adds a voice to the soundgroup
		*/ // --------------------------------------------------------------------
		void SoundGroup::AddVoice(Voice& mVoice) {
			mVoices.push_back(&mVoice);
			mVoice.mGroup = this;
		}

		// ------------------------------------------------------------------------
		/*! Remove Voice
		*
		*   Removes a voice from this group
		*/ // --------------------------------------------------------------------
		void SoundGroup::RemoveVoice(Voice& mVoice) {
			mVoices.remove(&mVoice);
		}
	}
}