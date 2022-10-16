//
//	AudioComponent.h
//	Good Neighbours
//
//	Created by Diego Revilla on 19/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _AUDIO_COMPONENT__H_
#define _AUDIO_COMPONENT__H_

namespace Engine {
	namespace Assets {
		template<>
		class ENGINE_API TResource<Audio::Sound> : public IResource {
#pragma region //Definitions
			using asset_ptr = Audio::Sound*;
			friend class Importers::AudioImporter;
#pragma endregion

#pragma region //Constructors & Destructors
		public:
			// ------------------------------------------------------------------------
			/*! Destructor
			*
			*   Frees a Sound using the FMOD API
			*/ // ---------------------------------------------------------------------
			~TResource() {
				GAudio->FreeSound(rawData);
			}
#pragma endregion

#pragma region // Functions
			// ------------------------------------------------------------------------
			/*! Get
			*
			*   Gets the Resource as something
			*/ // ---------------------------------------------------------------------
			DONTDISCARD asset_ptr inline Get() const noexcept {
				return reinterpret_cast<asset_ptr>(rawData);
			}
#pragma endregion

#pragma region //Members
		private:
			asset_ptr rawData = nullptr;
#pragma endregion
		};
	}

	namespace Audio {
		class ENGINE_API AudioEmitter : public Component {
#pragma region //Declarations
			struct AudioProperties {
				bool mLoop;
				bool mPlayOnSpawn;
				float mVolume;
				float mPitch;
				std::string mAudioFile;
			};
#pragma region

#pragma region //Constructors & Destructors
		public:
			AudioEmitter() noexcept;
			~AudioEmitter();
#pragma endregion

#pragma region // Functions
			void BeginPlay() override;
#ifdef _EDITOR_
			void OnEditor() override;
			void EditorTick() override;
#endif
			void FromJson(const json& val) override;
			void ToJson(json& val) const override;
			void SetLoop(const bool b) noexcept;
			void SetVolume(const float f) noexcept;
			void SetPitch(const float p) noexcept;
			void SetPlayOnSpawn(const bool p) noexcept;
			void SetAudioFile(const char* filePath) noexcept;
			void inline Play() noexcept;
			DONTDISCARD bool inline GetLoop() const noexcept;
			DONTDISCARD float inline GetVolume() const noexcept;
			DONTDISCARD float inline GetPitch() const noexcept;
		protected:
			DONTDISCARD AssetReference<Sound> inline GetSound() const noexcept;
			DONTDISCARD Voice inline& GetVoice() noexcept;
#pragma endregion

#pragma region //Members
		private:
			Voice mVoice;
			Asset<Sound> mSound;
			AudioProperties mProperties;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Play
		*
		*   Plays an already loaded audio file
		*/ // ---------------------------------------------------------------------
		void inline AudioEmitter::Play() noexcept {
			GAudio->Play(mSound->Get(), false, mVoice);
		}

		// ------------------------------------------------------------------------
		/*! Get Loop
		*
		*   Gets wether we are currently looping the audio track
		*/ // ---------------------------------------------------------------------
		bool inline AudioEmitter::GetLoop() const noexcept {
			return mProperties.mLoop;
		}

		// ------------------------------------------------------------------------
		/*! Get Volume
		*
		*   Gets the volume of the audio track
		*/ // ---------------------------------------------------------------------
		float inline AudioEmitter::GetVolume() const noexcept {
			return mProperties.mVolume;
		}

		// ------------------------------------------------------------------------
		/*! Get Pitch
		*
		*   Gets the pitch of the audio track
		*/ // ---------------------------------------------------------------------
		float inline AudioEmitter::GetPitch() const noexcept {
			return mProperties.mPitch;
		}

		// ------------------------------------------------------------------------
		/*! Get Sound
		*
		*   Gets the sound of the audio track
		*/ // ---------------------------------------------------------------------
		AssetReference<Sound> inline AudioEmitter::GetSound() const noexcept {
			return mSound;
		}

		// ------------------------------------------------------------------------
		/*! Get Voice
		*
		*   Gets the voice of the audio track
		*/ // ---------------------------------------------------------------------
		Voice inline& AudioEmitter::GetVoice() noexcept {
			return mVoice;
		}
	}
}

#endif