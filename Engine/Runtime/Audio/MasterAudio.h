//
//  MasterAudio.h
//  Good Neighbours
//
//  Created by Diego Revilla on 02/11/21.
//  Copyright � 2021 Diego Revilla. All rights reserved.
//

#ifndef _MASTER_AUDIO__H_
#define _MASTER_AUDIO__H_

namespace Engine {
	namespace Audio {
		struct ENGINE_API Sound {
			friend class AudioManager;
#pragma region //Constructors & Destructor
			Sound() noexcept;
#pragma endregion

#pragma region //Functions
			DONTDISCARD FMOD::Sound inline*& GetSound() noexcept;
			DONTDISCARD FMOD::Sound inline* operator*() const noexcept;
#pragma endregion

#pragma region //Members
		private:
			FMOD::Sound* mSound;
#pragma endregion
		};

		class ENGINE_API Voice {
#pragma region //Declarations
			// friends have access to privates
			friend struct SoundGroup;
#pragma endregion

#pragma region //Constructors & Destructor
		public:
			Voice() noexcept;
			~Voice();
#pragma endregion

#pragma region //Functions
			// Setters and Getters
			void SetVolume(const float volume) noexcept;
			void SetPause(const bool paused) const noexcept;
			void SetLoop(const bool isLoop) const noexcept;
			DONTDISCARD bool IsPaused() const noexcept;
			DONTDISCARD bool IsValid() noexcept;
			DONTDISCARD float GetVolume() const noexcept;
			void SetPitch(const float pitch) const noexcept;
			DONTDISCARD float GetPitch() const noexcept;
			DONTDISCARD std::chrono::steady_clock::time_point GetClock() const noexcept;
			void SetClock(std::chrono::steady_clock::time_point dt) noexcept;
			DONTDISCARD bool IsLooping() const noexcept;
			DONTDISCARD FMOD::Channel inline *& GetChannel() noexcept;
#pragma endregion

#pragma region //Members
		private:
			FMOD::Channel* mChannel;
			SoundGroup* mGroup;
			std::chrono::steady_clock::time_point mClock;
#pragma endregion
		};

		struct ENGINE_API SoundGroup {
#pragma region //Constructor & Destructors
			SoundGroup() noexcept;
			SoundGroup& operator=(const SoundGroup&) = delete;
#pragma endregion

#pragma region //Functions
			void SetGroupVolume(const float volume) noexcept;
			DONTDISCARD float inline GetGroupVolume() const noexcept;
			void SetGroupPitch(const float pitch) noexcept;
			float inline GetGroupPitch() const noexcept;
			void AddVoice(Voice& mVoice);
			void RemoveVoice(Voice& mVoice);
			void inline Clear();
#pragma endregion

#pragma region //Members
		private:
			Array<Voice*> mVoices;
			float mGroupVolume;
			float mGroupPitch;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Get Sound
		*
		*   Returns an access to the inner FMOD Sound
		*/ // ---------------------------------------------------------------------
		FMOD::Sound inline*& Sound::GetSound() noexcept {
			return mSound;
		}

		// ------------------------------------------------------------------------
		/*! Get Sound
		*
		*   Returns the inner FMOD Sound
		*/ // ---------------------------------------------------------------------
		FMOD::Sound inline* Sound::operator* () const noexcept {
			return mSound;
		}

		// ------------------------------------------------------------------------
		/*! Get Channel
		*
		*   Returns the voice's channel
		*/ // ---------------------------------------------------------------------
		FMOD::Channel inline *& Voice::GetChannel() noexcept { 
			return mChannel; 
		}

		// ------------------------------------------------------------------------
		/*! Get Group Volume
		*
		*   Gets the Volume of a group
		*/ // ---------------------------------------------------------------------
		float inline SoundGroup::GetGroupVolume() const noexcept {
			return mGroupVolume;
		}

		// ------------------------------------------------------------------------
		/*! Get Group Pitch
		*
		*   Gets the Pitch of a group
		*/ // ---------------------------------------------------------------------
		float inline SoundGroup::GetGroupPitch() const noexcept {
			return mGroupPitch;
		}

		// ------------------------------------------------------------------------
		/*! Clear
		*
		*   Clears all the pointees of the soundgroup
		*/ // ---------------------------------------------------------------------
		void inline SoundGroup::Clear() {
			mVoices.clear();
		}
	}
}

#endif
