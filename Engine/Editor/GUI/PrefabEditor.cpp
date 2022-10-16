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

	PrefabWindow::PrefabWindow() {
	}

	PrefabWindow::~PrefabWindow() {
	}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Draws the Properties Window
	*/ //----------------------------------------------------------------------
	void PrefabWindow::Present() noexcept {
		using __Texts::Editor::PropertiesGUI::StaticText;

		UIPropertyDraw _UIDraw;
		auto& _editor = *GEditor;
		auto tempobj = new Engine::Object;
		tempobj->FromJson(mCurrentEdition);

		Engine::Object* _base = tempobj;

		//Add the properties menu
		if (ImGui::Begin("Prefab Editor")) {
			//If we have a target object
			if (_base) {
				std::string _name(_base->GetName());
				if (_UIDraw << std::pair{ _base->GetName(), &_name })
					_base->SetName(_name.c_str());

				if (ImGui::Button("Apply Changes & Reload All")) {
					Engine::Scene* const _scene = GWorld->GetScene();

					// Scene to not use GetScene() a bunch of times
					tempobj->ToJson(mCurrentEdition);
					std::ofstream outputfile(mPath);
					outputfile << std::setw(4);
					outputfile << mCurrentEdition;
					outputfile.close();

					for (decltype(_scene->GetObjectCount()) i = 0, size = _scene->GetObjectCount(); i < size; i++) {
						Engine::Object* _y = _scene->GetObjectByID(i);
						if (!strcmp(_y->GetType(), mPath.c_str())) {
							Engine::Math::Transform prevtransform = _y->GetTransform();
							std::string prevname = _y->GetName();
							_y->LoadFromPrefab(_y->GetType());
							_y->SetPosition(prevtransform.mPos);
							_y->SetRotation(prevtransform.mRot);
							_y->SetScale(prevtransform.mScale);
							_y->SetName(prevname.c_str());
						}
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
						_temp_v3 = _base->GetScale(); if (_fentrytext(StaticText::Scale)) { _base->SetScale(_temp_v3); }
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
						ImGui::PushID(y);

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

					if (ImGui::Button("Close", { ImGui::GetWindowWidth(), 60 })) {
						tempobj->ToJson(mCurrentEdition);
						std::ofstream outputfile(mPath);
						outputfile << std::setw(4);;
						outputfile << mCurrentEdition;
						outputfile.close();
						GetOwner()->RemoveWindow(this);
						delete tempobj;
					}
					else {
						tempobj->ToJson(mCurrentEdition);
						tempobj->Destroy();
						delete tempobj;
					}

					ImGui::EndChild();
				}
			}
			else
				ImGui::Text(StaticText::CallBack);
			ImGui::End();
		}
	}
	void PrefabWindow::SetPath(const std::string& s) {
		std::fstream infile(s);
		infile >> mCurrentEdition;
		mPath = s;
		infile.close();
	}
}
#endif