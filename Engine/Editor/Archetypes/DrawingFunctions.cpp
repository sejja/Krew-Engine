//
//	DrawingFunctions.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 25/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

#ifdef _EDITOR_
namespace Editor {
	// ------------------------------------------------------------------------
	/*! Operator <<
	*
	*   Draws A Beautiful Vector 3 Editor
	*/ //----------------------------------------------------------------------
	bool UIPropertyDraw::operator<<(const std::pair<text_t, DiffPtr<Engine::Math::Vector3D>>& info) noexcept {
		bool _returned = false;
		std::string _namestr;
		float& _x = info.second->x;

		const auto _fsetc = [&_returned](DiffPtr<float> var, text_t name, const ImVec4& col1, const ImVec4& col2, const ImVec4& col3)
			noexcept {
			const ImVec2 _buttonSize = { 19.f, 16.f };

			ImGui::PushStyleColor(ImGuiCol_Button, col1);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, col2);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, col3);

			//If pressed on the Axis Icon, reset the value
			if (ImGui::Button(name, _buttonSize)) {
				*var = 0.f;
				_returned = true;
			}
		};

		const auto _fnewdrag = [&_returned, &_namestr](DiffPtr<float> var) noexcept {
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			if (ImGui::DragFloat(_namestr.c_str(), var, 0.1f, 0.0f, 0.0f, "%.2f")) _returned = true;
			ImGui::PopItemWidth();
		};

		const auto _fcoordedit = [_fsetc, _fnewdrag, &_namestr](DiffPtr<float> val, text_t name, text_t id, const ImVec4& col1,
			const ImVec4& col2, const ImVec4& col3) noexcept {
				ImGui::SameLine();
				_fsetc(val, name, col1, col2, col3);
				memcpy(_namestr.data(), id, sizeof(char) * 3);
				_fnewdrag(val);
		};

		_namestr.assign("##X");
		_namestr.reserve(3 + strlen(info.first));
		_namestr.append(info.first);

		ImGui::PushID(info.first);
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100.f);
		ImGui::Text(info.first);
		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });

		_fsetc(&_x, "X", { 0.8f, 0.1f, 0.15f, 1.0f }, { 0.9f, 0.2f, 0.2f, 1.0f }, { 0.8f, 0.1f, 0.15f, 1.0f });
		_fnewdrag(&_x);
		_fcoordedit(&info.second->y, "Y", "##Y", { 0.2f, 0.7f, 0.2f, 1.0f }, { 0.3f, 0.8f, 0.3f, 1.0f }, { 0.2f, 0.7f, 0.2f, 1.0f });
		_fcoordedit(&info.second->z, "Z", "##Z", { 0.1f, 0.25f, 0.8f, 1.0f }, { 0.2f, 0.35f, 0.9f, 1.0f }, { 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PopStyleVar();
		ImGui::Columns(1);
		ImGui::PopID();

		return _returned;
	}

	// ------------------------------------------------------------------------
	/*! Operator <<
	*
	*   Draws an Int Modifier
	*/ //----------------------------------------------------------------------
	bool UIPropertyDraw::operator<<(const std::pair<text_t, DiffPtr<int>>& info) noexcept {
		bool _returned = false;

		ImGui::TreeNodeEx(info.first, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
		ImGui::NextColumn();
		ImGui::SetNextItemWidth(-1);
		if (ImGui::InputInt((std::string("##") + info.first).c_str(), info.second)) _returned = true;
		ImGui::NextColumn();

		return _returned;
	}

	// ------------------------------------------------------------------------
	/*! Operator <<
	*
	*   Draws a Float Modifier
	*/ //----------------------------------------------------------------------
	bool UIPropertyDraw::operator<<(const std::pair<text_t, DiffPtr<float>>& info) noexcept {
		bool _returned = false;

		ImGui::TreeNodeEx(info.first, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
		ImGui::NextColumn();
		ImGui::SetNextItemWidth(-1);
		if (ImGui::InputFloat((std::string("##") + info.first).c_str(), info.second)) _returned = true;
		ImGui::NextColumn();

		return _returned;
	}

	// ------------------------------------------------------------------------
	/*! Operator <<
	*
	*   Draws an Unsigned modifier
	*/ //----------------------------------------------------------------------
	bool UIPropertyDraw::operator<<(const std::pair<text_t, DiffPtr<unsigned>>& info) noexcept {
		bool _returned = false;
		int _temp = static_cast<int>(*info.second);

		ImGui::TreeNodeEx(info.first, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet);
		ImGui::NextColumn();
		ImGui::SetNextItemWidth(-1);

		//Slide the Int on ImGui
		if (ImGui::SliderInt((std::string("##") + info.first).c_str(), &_temp, 0, 5000)) {
			_returned = true;
			*info.second = static_cast<unsigned>(_temp);
		}

		ImGui::NextColumn();

		return _returned;
	}

	// ------------------------------------------------------------------------
	/*! Operator <<
	*
	*   Draws a Color modifier
	*/ //----------------------------------------------------------------------
	bool UIPropertyDraw::operator<<(const std::pair<text_t, DiffPtr<Engine::Graphics::Color>>& info) noexcept {
		return operator<<(std::make_tuple(info.first, info.second, 4));
	}

	// ------------------------------------------------------------------------
	/*! Operator <<
	*
	*   Draws a selector of indexed strings
	*/ //----------------------------------------------------------------------
	bool UIPropertyDraw::operator<<(const std::tuple<text_t, DiffPtr<int>, text_t>& info) noexcept {
		bool _returned = false;
		ImGui::TreeNodeEx(std::get<0>(info), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen |
			ImGuiTreeNodeFlags_Bullet);
		ImGui::NextColumn();
		ImGui::SetNextItemWidth(-1);
		if (ImGui::Combo((std::string("##") + std::get<0>(info)).c_str(), std::get<1>(info), std::get<2>(info))) _returned = true;
		ImGui::NextColumn();

		return _returned;
	}

	// ------------------------------------------------------------------------
	/*! Operator <<
	*
	*   Draws a Color Modifier
	*/ //----------------------------------------------------------------------
	bool UIPropertyDraw::operator<<(const std::tuple<text_t, DiffPtr<Engine::Graphics::Color>, unsigned char>& info) noexcept {
		bool _returned = false;
		ImGui::TreeNodeEx(std::get<text_t>(info), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen |
			ImGuiTreeNodeFlags_Bullet);
		ImGui::NextColumn();
		ImGui::SetNextItemWidth(-1);

		//Switch by the number of channels
		switch (std::get<2>(info)) {
		case 3:
			if (ImGui::ColorEdit3((std::string("##") + std::get<0>(info)).c_str(), std::get<1>(info)->v)) _returned = true;
			break;
		default:
			if (ImGui::ColorEdit4((std::string("##") + std::get<0>(info)).c_str(), std::get<1>(info)->v)) _returned = true;
		}

		ImGui::NextColumn();

		return _returned;
	}

	// ------------------------------------------------------------------------
	/*! Operator <<=
	*
	*   Draws an Asset Importer with Drag & Drops
	*/ //----------------------------------------------------------------------
	bool UIPropertyDraw::operator<<=(const std::tuple<text_t, text_t, DiffPtr<std::string>>& info) const noexcept {
		bool returned = operator<<({ std::get<0>(info), std::get<2>(info) });

		//Start the Drop Target
		if (ImGui::BeginDragDropTarget()) {
			//If we can get the drag drop payload
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(std::get<1>(info))) {
				assertThis(static_cast<bool>(payload->DataSize == sizeof(std::string)), "Wrong Payload")
				std::string payload_n = *(std::string*)payload->Data;
				*std::get<2>(info) = payload_n;
				returned = true;
			}
			ImGui::EndDragDropTarget();
		}

		return returned;
	}

	// ------------------------------------------------------------------------
	/*! Operator <<
	*
	*   Draws an Unsigned modifier
	*/ //----------------------------------------------------------------------
	bool UIPropertyDraw::operator<<(const std::pair<text_t, DiffPtr<bool>>& info) const noexcept {
		return ImGui::Checkbox(info.first, info.second);
	}

	// ------------------------------------------------------------------------
	/*! Operator <<
	*
	*   Draws a string modifier
	*/ //----------------------------------------------------------------------
	bool UIPropertyDraw::operator<<(const std::pair<text_t, DiffPtr<std::string>>& info) const noexcept {
		return ImGui::InputText(info.first, info.second, ImGuiInputTextFlags_EnterReturnsTrue);
	}

	//bool UIPropertyDraw::operator<<(const std::pair<text_t, DiffPtr<Behavior*>>& info) noexcept
	//{
	//	auto traverse = [](Behavior* behavior) {
	//		Composite* comp = dynamic_cast<Composite*>(behavior);
	//		if (comp)
	//		{
	//			Array<Behavior*> comp_children = comp->GetChildren();
	//			for (auto child : comp_children)
	//			{
	//				traverse(child);
	//			}
	//		}
	//	};
	//}
}

#endif