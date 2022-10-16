//
//  Common Defines
//  Good Neighbours
//
//  Created by Diego Revilla on 28/09/20.
//  Copyright � 2020 Diego Revilla. All rights reserved.
//

#ifndef _COMMON_DEFINES__H_
#define _COMMON_DEFINES__H_

#define ALIGN(BYTES)				__declspec(align(BYTES))
#define DONTDISCARD				[[nodiscard]]
#define UNLIKELY						[[unlikely]]
#define LIKELY							[[likely]]
#define NOSPACE						[[no_unique_address]]
#define CATRIBUTE						 __declspec(naked)
#define UNUSED(a)						(void)(a)
#define ENGINE_API					__declspec(dllexport)
#define CLASS(...)
#define PROPERTY(...)
#ifdef _EDITOR_
#define GENERATED_BODY(...)	void OnEditor(); void FromJson(const nlohmann::json& val); void ToJson(nlohmann::json& val) const;
#else
#define GENERATED_BODY(...)	void FromJson(const json& val); void ToJson(json& val) const;
#endif

template<typename T>
using DiffPtr = T* __restrict;

static const float EPSILON = 0.0001f;
static const float aDegree = glm::radians(1.0f);
static const float aLoop = glm::radians(360.0f);
static const float cUnit = 1 / 255.0f;

namespace Color
{
	static const glm::vec4 white = glm::vec4(1, 1, 1, 1);
	static const glm::vec4 black = glm::vec4(0, 0, 0, 1);
	static const glm::vec4 red = glm::vec4(1, 0, 0, 1);
	static const glm::vec4 green = glm::vec4(0, 1, 0, 1);
	static const glm::vec4 blue = glm::vec4(0, 0, 1, 1);
}

#endif