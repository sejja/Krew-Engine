#include <Shared.h>

namespace Engine {
	namespace Graphics {
		FogComponent::FogComponent() {
			GGfxPipeline->mFogMachine.mAllFogs.push_back(this);
		}

		FogComponent::~FogComponent() {
			GGfxPipeline->mFogMachine.mAllFogs.remove(this);
		}

#ifdef _EDITOR_
		void FogComponent::OnEditor() {
			Editor::UIPropertyDraw uidraw;

			uidraw << std::make_pair(TEXT("Fog Color"), &mFogColor);
			uidraw << std::make_pair(TEXT("Intensity"), &mIntensity);

			const char* customNames[3] = { "Linear", "Exponential", "Exponential Squared" };
			// Choosing blending type
			int t = (int)mType;
			ImGui::Combo("Blending Type", &t, customNames, 3);
			mType = (eFogType)t;
			
			switch (mType) {
			case Engine::Graphics::FogComponent::eFogType::e_Linear:
				uidraw << std::make_pair(TEXT("Near"), &mNear);
				uidraw << std::make_pair(TEXT("Far"), &mFar);
				uidraw << std::make_pair(TEXT("Steepness"), &steepness);
				uidraw << std::make_pair(TEXT("Offset"), &offset);
				break;
			case Engine::Graphics::FogComponent::eFogType::e_Exponential:
			case Engine::Graphics::FogComponent::eFogType::e_ExponentialSquared:
				uidraw << std::make_pair(TEXT("Density"), &density);
				break;
			default:
				break;
			}

		}
#endif

		void FogComponent::FromJson(const nlohmann::json& j) {

			fetch(mNear, j, "mNear");
			fetch(mFar, j, "mFar");
			fetch(steepness, j, "steepness");
			fetch(offset, j, "offset");
			fetch(mIntensity, j, "mIntensity");
			fetch(density, j, "density");
			fetch(mType, j, "mType");
			fetch(mFogColor, j, "Color");
		}

		void FogComponent::ToJson(nlohmann::json& j) const {
			j["steepness"] = steepness;
			j["offset"] = offset;
			j["mNear"] = mNear;
			j["mFar"] = mFar;
			j["mIntensity"] = mIntensity;
			j["density"] = density;
			j["mType"] = (int)mType;
			j["Color"] = Serializers::ColorSerializer::ColorToJson(mFogColor);
		}
	}
}