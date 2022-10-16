//
//  Audio.h
//  Good Neighbours
//
//  Created by Diego Revilla on 28/3/20.
//  Copyright � 2020 Diego Revilla. All rights reserved.
//

#ifndef _AUDIO__H_
#define _AUDIO__H_

namespace Engine {
	namespace Audio {
		class ENGINE_API AudioManager {
#pragma region //Declarations
			struct fmod_info {
				FMOD::System* mFMOD;
				Voice* mVoices;
			};
#pragma endregion

#pragma region //Constructors & Destructors
		public:
			AudioManager();
			~AudioManager();
			AudioManager& operator=(const AudioManager&)  = delete;
#pragma endregion

#pragma region //Methods
			DONTDISCARD Sound* CreateSound(const char* filename);
			void FreeSound(Sound* mSound);
			void StopAll();
			void FreeVoice(Voice* pVoice);
			void Play(const Sound* mSound, bool paused, Voice& outvoice);
			void Loop(Voice* mSound);
			DONTDISCARD size_t inline GetFreeVoiceCount() const noexcept;
			DONTDISCARD size_t inline GetUsedVoiceCount() const noexcept;
			DONTDISCARD size_t inline GetTotalVoiceCount() const noexcept;
			void Update();
			void inline SetMasterVolume(const float f) noexcept;
			DONTDISCARD float inline GetMasterVolume() const noexcept;
			DONTDISCARD SoundGroup inline& GetSoundGroup(const char* name);
			void Set3DListener(const int listener, const Math::Vector3D& pos, const Math::Vector3D& vel,
				const Math::Vector3D& forward, const Math::Vector3D& up) const;
			void RequestMoreVoices(std::size_t mAdvise);
			static AudioManager& Instance();
		private:
			void AllocateVoices();
			void FreeVoices();
			DONTDISCARD Voice* GetFreeVoice();
#pragma endregion

#pragma region //Members
			fmod_info mData;
			std::list<Voice*> mFreeVoices;
			std::list<Voice*> mUsedVoices;
			std::unordered_map<std::string, SoundGroup> mSoundGroups;
			float mMasterVol = 1;
			Engine::PageAllocator<Sound> mSoundAlloc;
			Engine::Allocator<Voice> mVoiceAlloc;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Get Free Voice Count
		*
		*   Returns how many free voices we do have
		*/ // --------------------------------------------------------------------
		size_t inline AudioManager::GetFreeVoiceCount() const noexcept {
			return mFreeVoices.size();
		}

		// ------------------------------------------------------------------------
		/*! Get Used Voice Count
		*
		*   Returns how many voices have we used
		*/ // --------------------------------------------------------------------
		size_t inline AudioManager::GetUsedVoiceCount() const noexcept {
			return mUsedVoices.size();
		}

		// ------------------------------------------------------------------------
		/*! Get Total Voice Count
		*
		*  Get the total voice count
		*/ // --------------------------------------------------------------------
		size_t inline AudioManager::GetTotalVoiceCount() const noexcept {
			return mFreeVoices.size() + mUsedVoices.size();
		}

		// ------------------------------------------------------------------------
		/*! Get Sound Group
		*
		*   Returns a SoundGroup
		*/ // ---------------------------------------------------------------------
		SoundGroup inline& AudioManager::GetSoundGroup(const char* name) {
			return mSoundGroups[name];
		}

		// ------------------------------------------------------------------------
		/*! Set Master Volume
		*
		*   Sets the maximun value
		*/ // --------------------------------------------------------------------
		void inline AudioManager::SetMasterVolume(const float f) noexcept {
			mMasterVol = Math::Clamp(f, 0.f, 1.f);
		}

		// ------------------------------------------------------------------------
		/*! Get Master Volume
		*
		*   Gets the Master volume audio
		*/ // ----------------------------------------------------------------------
		float inline AudioManager::GetMasterVolume() const noexcept {
			return mMasterVol;
		}
	}
}

#define GAudio (&Engine::Audio::AudioManager::Instance())

#endif