//
//	Serialization.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 24/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

namespace Engine {
	// ------------------------------------------------------------------------
	/*! From JSON
	*
	*   Empty Function
	*/ //--------------------------------------------------------------------
	void Serializable::FromJson(const nlohmann::json&) {}

	// ------------------------------------------------------------------------
	/*! To JSON
	*
	*   Empty Function
	*/ //--------------------------------------------------------------------
	void Serializable::ToJson(nlohmann::json&) const {}

	// ------------------------------------------------------------------------
	/*! Operator <<
	*
	*   Wrapper operator
	*/ //--------------------------------------------------------------------
	nlohmann::json& Serializable::operator<<(nlohmann::json& val) const {
		ToJson(val);
		return val;
	}

	// ------------------------------------------------------------------------
	/*! Operator >>
	*
	*   Wrapper operator
	*/ //--------------------------------------------------------------------
	const nlohmann::json& Serializable::operator>>(const nlohmann::json& val) {
		FromJson(val);
		return val;
	}

}