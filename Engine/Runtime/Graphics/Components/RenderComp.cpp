#include "Shared.h"

using namespace glm;

namespace Engine
{
	AABB RenderComp::GetBV()const
	{
		if (isEmitter)
			return bv;

		vec3 pos = GetOwner()->GetWorldPosition();
		vec3 rot = GetOwner()->GetWorldRotation();
		vec3 sca = GetOwner()->GetWorldScale();
		mat4 mtx = GetOwner()->GetWorldMatrix();
		AABB bv0 = { sca * bv.min, sca * bv.max };
		if (rot != vec3(0))
		{
			vec3 localAxis[3] = {
				glm::normalize(vec3(mtx * vec4(1,0,0,0))),
				glm::normalize(vec3(mtx * vec4(0,1,0,0))),
				glm::normalize(vec3(mtx * vec4(0,0,1,0))),
			};
			vec3 localHalfSize = (bv0.max - bv0.min) * 0.5f;
			vec3 mid = bv0.min + localHalfSize;
			vec3 aabbHalfSize =
				glm::abs(localAxis[0] * localHalfSize[0]) +
				glm::abs(localAxis[1] * localHalfSize[1]) +
				glm::abs(localAxis[2] * localHalfSize[2]);
			bv0.min = mid - aabbHalfSize;
			bv0.max = mid + aabbHalfSize;
		}
		bv0.min += pos;
		bv0.max += pos;

		AABB result(
			glm::min(bv0.min, bv0.max),
			glm::max(bv0.min, bv0.max)
		);
		return result;
	}

	void RenderComp::FromJson(const nlohmann::json& val)
	{
		fetch(hasShadow, val, "hasShadow");
		fetch(hasBloom, val, "hasBloom");
		fetch(bloomIntensity, val, "bloomIntensity");
	}

	void RenderComp::ToJson(nlohmann::json& val) const
	{
		val["hasShadow"] = hasShadow;
		val["hasBloom"] = hasBloom;
		val["bloomIntensity"] = bloomIntensity;
	}

	bool RenderComp::operator<(const RenderComp& that) const
	{
		auto thisBV = GetBV();
		auto thatBV = that.GetBV();
		vec3 thisClosest = glm::clamp(GGfxPipeline->camPos, thisBV.min, thisBV.max);
		vec3 thatClosest = glm::clamp(GGfxPipeline->camPos, thatBV.min, thatBV.max);

		// Sort in distance to camera order : far components drawn first.
		return glm::distance2(thisClosest, GGfxPipeline->camPos) > glm::distance2(thatClosest, GGfxPipeline->camPos);
	}
}