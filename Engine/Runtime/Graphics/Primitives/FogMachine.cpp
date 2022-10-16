#include <Shared.h>

namespace Engine {
	namespace Graphics {
		void FogMachine::ComputeFog() {
			Color finalcol;
			float intensity = std::numeric_limits<float>::min();
			auto shader = GGfxPipeline->shaders.simpleShader;
			shader->Bind();
			int size = mAllFogs.size();

			if (!mAllFogs.empty()) {
				float nears = std::numeric_limits<float>::max();
				float fars = std::numeric_limits<float>::min();
				float steepness = std::numeric_limits<float>::min();
				float offset = std::numeric_limits<float>::min();
				float density = std::numeric_limits<float>::min();
				int type = std::numeric_limits<int>::min();
				for (auto& x : mAllFogs) {
					finalcol.r += (1.f / size) * x->mFogColor.r;
					finalcol.g += (1.f / size) * x->mFogColor.g;
					finalcol.b += (1.f / size) * x->mFogColor.b;
					finalcol.a += (1.f / size) * x->mFogColor.a;
					nears = std::min(nears, x->mNear);
					fars = std::max(fars, x->mFar);
					steepness = std::max(steepness, x->steepness);
					intensity = std::max(intensity, x->mIntensity);
					offset = std::max(offset, x->offset);
					density = std::max(density, x->density);
					type = std::max(type, (int)x->mType);
				}

				shader->SetShaderUniform("fogColor", &finalcol);
				shader->SetShaderUniform("near", &nears);
				shader->SetShaderUniform("far", &fars);
				shader->SetShaderUniform("fogsteepness", &steepness);
				shader->SetShaderUniform("fogoffset", &offset);
				shader->SetShaderUniform("fogintensity", &intensity);
				shader->SetShaderUniform("fogdensity", &density);
				shader->SetShaderUniform("fogpreset", &type);
			}
			shader->SetShaderUniform("fogenabled", &size);
			shader->Unbind();
			shader = GGfxPipeline->shaders.SkyboxShader;
			shader->Bind();

			if (!mAllFogs.empty()) {
				shader->SetShaderUniform("fogcolor", &finalcol);
				shader->SetShaderUniform("fogintensity", &intensity);
			}
			else {
				Color tempcol(1, 1, 1, 1);
				float tempint = 1.f;
				shader->SetShaderUniform("fogcolor", &tempcol);
				shader->SetShaderUniform("fogintensity", &tempint);
			}

			shader->Unbind();
		}
	}
}