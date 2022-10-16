//
//	Serialization.h
//	Good Neighbours
//
//	Created by Diego Revilla on 24/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _SERIALIZATION__H_
#define _SERIALIZATION__H_

namespace Engine {
	struct ENGINE_API Serializable {
#pragma region //Declarations
		using json = nlohmann::json;
#pragma endregion

#pragma region // Functions
		virtual void FromJson(const nlohmann::json& val);
		virtual void ToJson(nlohmann::json& val) const;
		DONTDISCARD nlohmann::json& operator<<(nlohmann::json& val) const;
		const nlohmann::json& operator>>(const nlohmann::json& val);
#pragma endregion
	};

	template<typename T, typename D>
	void fetch(T& t, const nlohmann::json& val, const D& key)
	{
		auto it = val.find(key);
		if (it != val.end())
			t = (*it).get<T>();
	}
}

#endif