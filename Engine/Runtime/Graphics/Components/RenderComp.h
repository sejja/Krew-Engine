//
//	RenderComp.h
//	Good Neighbours
//
//	Created by Fernando Uribe on 09/17/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#ifndef _GFX_RENDER_COMP__H_
#define _GFX_RENDER_COMP__H_

namespace Engine
{
	enum class BlendingType
	{
		eOpaque,
		eAdditive,
		eModular
	};

	enum class SamplingType
	{
		eRepeat,
		eMirroredRepeat,
		eClampToEdge,
		eClampToBorder
	};

	struct ENGINE_API RenderComp : public Component
	{
		virtual void Render() = 0;

		AABB GetBV()const;
		virtual void FromJson(const nlohmann::json& val);
		virtual void ToJson(nlohmann::json& val) const;

		BlendingType GetBlendingType() { return mBlendingType; }
		void SetBlendingType(BlendingType const& blend) { mBlendingType = blend; }

		bool operator<(const RenderComp& that) const;

		AABB bv;
		bool drawBV = false;
		bool isEmitter = false;

		bool isVisible = true;
		bool hasShadow = false;
		bool hasBloom = false;
		float bloomIntensity = 0;
		BlendingType mBlendingType;
		SamplingType mSamplingType = SamplingType::eRepeat;
	};
}

#endif
