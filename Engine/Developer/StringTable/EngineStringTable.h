//
//	EngineStrigTable.h
//	Good Neighbours
//
//	Created by Diego Revilla on 25/10/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#ifndef _ENGINE_STRING_TABLE__H_
#define _ENGINE_STRING_TABLE__H_

namespace __Texts {
	namespace Engine {
		namespace AudioComponent {
			struct StaticText {
				constexpr static const char Loop[] = "Loop";
				constexpr static const char Volume[] = "Volume";
				constexpr static const char Pitch[] = "Pitch";
				constexpr static const char PlayOnSpawn[] = "Play On Spawn";
				constexpr static const char ContentAudioID[] = "CONTENT_AUDIO_FILE";
			};
		}

		namespace SoundEmitter3D {
			struct StaticText {
				constexpr static const char MinimunDistance[] = "Minimum Distance";
				constexpr static const char MaximumDistance[] = "Maximum Distance";
				constexpr static const char EnableDopplerEffect[] = "Enable Doppler Effect";
			};
		}

		namespace GameObject {
			struct StaticText {
				constexpr static const char GameObject[] = "Object";
				constexpr static const char Components[] = "Components";
				constexpr static const char Name[] = "Name";
				constexpr static const char Transform[] = "Transform";
				constexpr static const char Childs[] = "Childs";
				constexpr static const char Tags[] = "Tags";
				constexpr static const char PrefabExtension[] = ".prefab";
				constexpr static const char Type[] = "Type";
			};
		}
		namespace RenderComp {
			struct StaticText {
				constexpr static const char DefaultTexture[] = "EngineContent/Texture/DefaultTex.png";
			};
		}
	}
}
#endif