//
//	PropertiesGUI.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 18/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

#ifdef _EDITOR_
namespace Editor {
	// ------------------------------------------------------------------------
	/*! Begin Drop Target
	*
	*   Drags object around the scene
	*/ //----------------------------------------------------------------------
	template<typename FN>
	void BeginDropTarget(const char* id, FN f) {
		//If there is a drag drop going
		if (ImGui::BeginDragDropTarget()) {
			//If the packet is for us
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(id)) {
				f(payload);
			}
			ImGui::EndDragDropTarget();
		}
	}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Draws the Properties Window
	*/ //----------------------------------------------------------------------
	void PropertiesWindow::Present() noexcept {
		using __Texts::Editor::PropertiesGUI::StaticText;

		UIPropertyDraw _UIDraw;
		auto& _editor = *GEditor;
		Engine::Object* _base = !_editor.GetSelectedObject().empty() ? _editor.GetSelectedObject().front() :
			nullptr;

		//Add the properties menu
		if (ImGui::Begin(StaticText::Properties)) {
			//If we have a target object
			if (_base && _editor.GetSelectedObject().size() == 1) {
				std::string _name(_base->GetName());
				if (_UIDraw << std::pair{ _base->GetName(), &_name })
					_base->SetName(_name.c_str());

				if (strcmp(_base->GetType(), "Object")) {
					if (ImGui::Button("Reload Prefab")) {
						Engine::Math::Transform prevtransform = _base->GetTransform();
						std::string prevname = _base->GetName();
						_base->LoadFromPrefab(_base->GetType());
						_base->SetPosition(prevtransform.mPos);
						_base->SetRotation(prevtransform.mRot);
						_base->SetScale(prevtransform.mScale);
						_base->SetName(prevname.c_str());
					}
				}

				ImGui::Separator();

				//If we want to show options
				if (ImGui::BeginMenu(StaticText::Options)) {
					//Remove object?
					if (ImGui::MenuItem(StaticText::RemoveObject))
					{
						GWorld->GetScene()->RemoveObject(_base);
						_editor.GetSelectedObject().clear();
					}
					//Create a prefab
					if (ImGui::MenuItem(StaticText::MakePrefab))
					{
						GetOwner()->AddWindow(Engine::Allocator<PrefabWindowModal>::New());

					}

					ImGui::EndMenu();
				}

				//Create a child, called transform
				if (ImGui::BeginChild(__Texts::Engine::GameObject::StaticText::Transform)) {
					//If we want to display the transform
					if (ImGui::CollapsingHeader(__Texts::Engine::GameObject::StaticText::Transform)) {
						Engine::Math::Vector3D _temp_v3 = _base->GetPosition();

						const auto _fentrytext = [&_UIDraw, &_temp_v3](const char* text) noexcept {
							return _UIDraw << std::pair{ text, &_temp_v3 };
						};

						if (_fentrytext(StaticText::Position)) _base->SetPosition(_temp_v3);
						_temp_v3 = _base->GetScale(); if (_fentrytext(StaticText::Scale)) _base->SetScale(_temp_v3);
						_temp_v3 = _base->GetRotation(); if (_fentrytext(StaticText::Rotation)) _base->SetRotation(_temp_v3);
					}

					//If we want to display the tags
					if (ImGui::CollapsingHeader(__Texts::Engine::GameObject::StaticText::Tags)) {
						//Text input for tags
						std::string _tag;
						_UIDraw << std::pair{ "", &_tag };

						//Button to add inputed tag to object tags
						ImGui::SameLine();
						if (ImGui::Button(StaticText::Add)) _base->AddTag(_tag.c_str());

						//Button to remove inputed tag from object tags
						ImGui::SameLine();
						if (ImGui::Button(StaticText::Remove))
							if (_base->HasTag(_tag.c_str()))
								_base->RemoveTag(_tag.c_str());

						//Display all the object tags
						ImGui::NewLine();
						const Engine::Array<std::string>& tags = _base->GetTagsRef();
						std::string c(StaticText::CurrentTags);

						//Add all the tags to a common string
						for (auto& it : tags) {
							c += it;
							if (it != tags.back())
								c += ", ";
						}
						//Display the string
						//NOTE: Need to check tomorrow how to limit width
						ImGui::TextWrapped(c.c_str());
					}
					const size_t _ccount = _base->GetComponentCount();

					//For every Child in the object
					for (std::remove_const<decltype(_ccount)>::type y = 0; y < _ccount; y++) {
						Engine::Component* const _comp = _base->GetComponent(y);

						ImGui::Separator();
						ImGui::PushID(reinterpret_cast<void*>(reinterpret_cast<intptr_t>(_comp)));

						//If we expand the property
						if (ImGui::CollapsingHeader(_comp->GetName())) {
							//If we display the options palette
							if (ImGui::BeginMenu(StaticText::Options)) {
								//Remove object?
								if (ImGui::MenuItem(StaticText::RemoveComponent)) {
									_base->RemoveComponent(_comp);
									ImGui::EndMenu();
									ImGui::PopID();
									ImGui::EndChild();
									ImGui::End();

									return;
								}

								ImGui::EndMenu();
							}

							_comp->OnEditor();
						}

						ImGui::PopID();
					}

					ImGui::Button(StaticText::add, { ImGui::GetWindowWidth(), 60 });

					const auto _fcreatecomp = [&_base](const ImGuiPayload* payload) {
						std::string payload_n = *(std::string*)payload->Data;
						Engine::IBase* p = GFactory->CreateTypeByName(payload_n.c_str());
						p->SetName(payload_n.c_str());

						if (GRTTI->IsA<Engine::Component>(p)) _base->AddComponent((Engine::Component*)p);
					};

					BeginDropTarget(__Texts::Editor::PaletteWindow::StaticText::ComponentSpawnerID, _fcreatecomp);

					ImGui::EndChild();
				}
			}
			else
				ImGui::Text(StaticText::CallBack);
			ImGui::End();
		}
	}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Presents the Palette Window
	*/ //----------------------------------------------------------------------
	void SceneWindow::Present() noexcept {
		using __Texts::Editor::SceneWindow::StaticText;

		if (ImGui::Begin(__Texts::Editor::PostProcessorGUI::StaticText::PostProcessorGUI))
		{
			Engine::PostProcessor* _postprocessor = GGfxPipeline->GetPostProcessor();

			ImGui::Checkbox("Bloom", &_postprocessor->Bloom_ON);
			if (_postprocessor->Bloom_ON)
			{
				ImGui::DragFloat("Threshold", &_postprocessor->threshold, 0.01f, 0.0f, 10.0f);
				ImGui::SliderInt("Samples", &_postprocessor->samples, 1, 10);
			}
			ImGui::Separator();
			ImGui::Checkbox("HDR", &_postprocessor->HDR_ON);
			if (_postprocessor->HDR_ON)
			{
				ImGui::Checkbox("Aces", &_postprocessor->ACES_ON);
				ImGui::DragFloat("Exposure", &_postprocessor->exposure, 0.01f, 0.0f, 10.0f);
				ImGui::DragFloat("Gamma", &_postprocessor->gamma, 0.01f, 0.1f, 5.0f);
			}
		}

		//If we can present the Scene Inspector
		if (ImGui::Begin(StaticText::Scene)) {
			//For each space
			ImGui::Columns(3);
			ImGui::Separator();

			const auto _fcolumnheader = [](const char* text) {
				ImGui::Text(text); ImGui::NextColumn();
			};

			_fcolumnheader(StaticText::ID);
			_fcolumnheader(__Texts::Engine::GameObject::StaticText::Name);
			_fcolumnheader(__Texts::Engine::GameObject::StaticText::Type);
			ImGui::Separator();

			// Scene to not use GetScene() a bunch of times
			Engine::Scene* const _scene = GWorld->GetScene();

			for (decltype(_scene->GetObjectCount()) i = 0, size = _scene->GetObjectCount(); i < size; i++) {
				Engine::Object* _y = _scene->GetObjectByID(i);
				Engine::Object** _move = nullptr;

				_Move(i, _move, _y);

				// If we dragged an object into another
				if (_move)
				{
					Engine::Object* parent = (*_move)->GetParent(), ** new_parent = &_y;

					//If the game object does not have a parent
					if (!parent) {
						_scene->RemoveObjectOwnership(*_move);
						size = _scene->GetObjectCount();
					}
					else
						parent->RemoveChild(*_move);

					//If the target game object is a 0-level object
					if (*_move == (*new_parent)->GetParent()) {
						//If we didn't have a parent
						if (!parent)
							_scene->SpawnObject(*_move);
						else
							UNUSED(parent->AddChild(*new_parent));
					}
					(*new_parent)->AddChild(*_move);
					ImGui::SetDragDropPayload(StaticText::ChildPayload, *new_parent, sizeof(Engine::Object)); // Update payload immediately so on the next frame if we move the mouse to an earlier item our index payload will be correct. This is odd and showcase how the DnD api isn't best presented in this example.
				}

				ImGui::NextColumn();
				ImGui::Text(_y->GetName());
				ImGui::NextColumn();
				std::string name = _y->GetType();
				name = name.substr(name.find_last_of("/\\") + 1);
				std::string::size_type const p(name.find_last_of('.'));
				name = name.substr(0, p);
				ImGui::Text(name.c_str());
				ImGui::NextColumn();

				//Recurse each child
				if (_y->GetChildCount()) {
					_present_children(_y);
					ImGui::Columns(3);
				}
			}

			ImGui::Columns(1);
			ImGui::Separator();
		}

		ImGui::End();
	}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Presents the Palette Window
	*/ //----------------------------------------------------------------------
	void SceneWindow::_present_children(Engine::Object* const parent) noexcept {
		using __Texts::Editor::SceneWindow::StaticText;

		std::string _parent_name = std::string(StaticText::ChildrenOf) + std::string(parent->GetName());
		ImGui::Columns(1);

		//If we open the tree node
		if (ImGui::TreeNode(_parent_name.c_str())) {
			ImGui::Columns(3);

			//Present each child
			for (size_t i = 0, size = parent->GetChildCount(); i < size; i++) {
				Engine::Object* _y = parent->GetChild(i);
				Engine::Object** move = nullptr;

				_Move(i, move, _y);
				// If we dragged an object into another
				if (move) {
					auto& _world = *GWorld;

					Engine::Object* _parent = (*move)->GetParent(), ** new_parent = &_y;

					//If it doesn't have a parent
					if (!_parent)
						_world.GetScene()->RemoveObjectOwnership(*move);
					else
						_parent->RemoveChildParentship(*move);

					//If the parent is the target
					if (*move == (*new_parent)->GetParent()) {
						//If we had a parent
						if (!_parent) {
							_world.GetScene()->SpawnObject(_y);
							_y->GetParent()->RemoveChildParentship(_y);
						}
						else
							_parent->AddChild(*new_parent);
						(*move)->RemoveChildParentship(*new_parent);
					}
					(*new_parent)->AddChild(*move);
					ImGui::SetDragDropPayload(StaticText::ChildPayload, *new_parent, sizeof(Engine::Object)); // Update payload immediately so on the next frame if we move the mouse to an earlier item our index payload will be correct. This is odd and showcase how the DnD api isn't best presented in this example.
				}

				ImGui::NextColumn();
				ImGui::Text(_y->GetName());
				ImGui::NextColumn();
				std::string name = _y->GetType();
				name = name.substr(name.find_last_of("/\\") + 1);
				std::string::size_type const p(name.find_last_of('.'));
				name = name.substr(0, p);
				ImGui::Text(name.c_str());
				ImGui::NextColumn();

				if (_y->GetChildCount()) _present_children(_y);
			}

			ImGui::Columns(1);
			ImGui::Separator();
			ImGui::TreePop();
		}
	}

	// ------------------------------------------------------------------------
	/*! Move
	*
	*   Moves an object into another object through drag payloads
	*/ //----------------------------------------------------------------------
	void SceneWindow::_Move(const size_t i, DiffPtr<Engine::Object*>& move, DiffPtr<Engine::Object> y) {
		using __Texts::Editor::SceneWindow::StaticText;

		auto& _editor = *GEditor;

		static char _label[5];
		sprintf(_label, StaticText::IDFormating, i);

		auto it = std::find(_editor.GetSelectedObject().begin(), _editor.GetSelectedObject().end(), y);

		bool b = it != _editor.GetSelectedObject().end();

		//If we select the Object
		if (ImGui::Selectable(_label, b, ImGuiSelectableFlags_SpanAllColumns) && !ImGui::BeginDragDropSource()) {
			if (!ImGui::IsKeyDown(VK_SHIFT))
				_editor.GetSelectedObject().clear();
			_editor.SetSelectedObject(y);
		}

		// If we grab the object
		if (ImGui::BeginDragDropSource()) {
			ImGui::Text(StaticText::PayloadText, y->GetName());
			ImGui::SetDragDropPayload(StaticText::ChildPayload, &y, sizeof(Engine::Object*));
			ImGui::EndDragDropSource();
		}

		const auto _fmove = [&move](const ImGuiPayload* payload) {
			move = (Engine::Object**)payload->Data;
		};

		BeginDropTarget(StaticText::ChildPayload, _fmove);
	}
	void PostProcessorGUI::Present() noexcept
	{
		using __Texts::Editor::PostProcessorGUI::StaticText;

		//If we can present the postprocessor GUI
		if (ImGui::Begin(StaticText::PostProcessorGUI))
		{
			Engine::PostProcessor* _postprocessor = GGfxPipeline->GetPostProcessor();

			ImGui::Checkbox("Bloom", &_postprocessor->Bloom_ON);
			if (_postprocessor->Bloom_ON)
			{
				ImGui::DragFloat("Threshold", &_postprocessor->threshold, 0.01f, 0.0f, 10.0f);
				ImGui::SliderInt("Samples", &_postprocessor->samples, 1, 10);
			}
			ImGui::Separator();
			ImGui::Checkbox("HDR", &_postprocessor->HDR_ON);
			if (_postprocessor->HDR_ON)
			{
				ImGui::Checkbox("Aces", &_postprocessor->ACES_ON);
				ImGui::DragFloat("Exposure", &_postprocessor->exposure, 0.01f, 0.0f, 10.0f);
				ImGui::DragFloat("Gamma", &_postprocessor->gamma, 0.01f, 0.1f, 5.0f);
			}
		}

		ImGui::End();
	}
}
#endif