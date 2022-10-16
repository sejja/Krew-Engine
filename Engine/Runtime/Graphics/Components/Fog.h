#ifndef _FOG__H_
#define _FOG__H_

namespace Engine {
	namespace Graphics {
		class FogComponent : public Component {
			friend class FogMachine;

			enum class eFogType : unsigned short {
				e_Linear,
				e_Exponential,
				e_ExponentialSquared,
			};
		
		public:
			FogComponent();
			~FogComponent();
#ifdef _EDITOR_
			virtual void OnEditor() override;
#endif
			void FromJson(const nlohmann::json& j) override;
			void ToJson(nlohmann::json& j) const override;
		private:
			Color mFogColor;
			float mNear = 0.01f;
			float mFar = 1000.f;
			float steepness = 0.5;
			float offset = 5.0;
			float mIntensity = 0.1f;
			float density = 1.f;
			eFogType mType = eFogType::e_Linear;
		};
	}
}

#endif