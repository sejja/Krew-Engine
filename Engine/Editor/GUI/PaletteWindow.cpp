//
//	PaletteWindow.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 24/09/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

#ifdef _EDITOR_
namespace Editor {
	static Engine::PageAllocator<Engine::GameObject> sGOAlloc;

	namespace {
		// ------------------------------------------------------------------------
		/*! New Obj
		*
		*   Creates a new Object given a name
		*/ //----------------------------------------------------------------------
		DONTDISCARD Engine::GameObject* _new_obj(const char* name) {
			using __Texts::Editor::PaletteWindow::StaticText;

			Engine::GameObject* const go = sGOAlloc.New();
			go->SetName(name);
			return go;
		};

		// ------------------------------------------------------------------------
		/*! New Obj with Mesh
		*
		*   Creates a new Object with a mesh renderer
		*/ //----------------------------------------------------------------------
		DONTDISCARD Engine::GameObject* _new_obj_with_mesh(const char* name, const int idx) {
			using __Texts::Editor::PaletteWindow::StaticText;

			Engine::GameObject* const go = _new_obj(name);
			Engine::MeshRenderer* const c = go->AddComponent<Engine::MeshRenderer>(StaticText::MeshRenderer);
			c->SetMeshByIdx(idx), c->SetShaderByIdx(0);
			return go;
		};

		// ------------------------------------------------------------------------
		/*! Button With Id
		*
		*   Creates a button with Drag and Drop Properties
		*/ //----------------------------------------------------------------------
		template<typename T>
		void _button_with_id(DiffPtr<const char> const text, const float bias, DiffPtr<T> const creator, 
			DiffPtr<const char> const id) noexcept {
			using __Texts::Editor::PaletteWindow::StaticText;

			ImGui::PushID(0);
			ImGui::Button(text, { ImGui::GetWindowWidth() - bias, 60 });

			// Add a drag source for our object
			if (ImGui::BeginDragDropSource()) {
				ImGui::SetDragDropPayload(id, creator, sizeof(T));
				ImGui::Text((std::string{ StaticText::CreateNew } + text).c_str());
				ImGui::EndDragDropSource();
			}

			ImGui::PopID();
		};

		// ------------------------------------------------------------------------
		/*! Thumbnail
		*
		*   Renders a Thumbnail button fot the palette
		*/ //----------------------------------------------------------------------
		void _thumbnail(DiffPtr<const char> name, DiffPtr<const char> tex, 
			DiffPtr<const __private::GuiCreator> creator) noexcept {
			using __Texts::Editor::PaletteWindow::StaticText;

			ImGui::Image(reinterpret_cast<ImTextureID>(static_cast<intptr_t>(
					GContent->GetResource<Engine::Graphics::Texture>(tex)->Get()->GetGLHandle())), { 60, 60 },
				{ 0, 1 }, { 1, 0 });
				ImGui::SameLine();
				_button_with_id(name, 60, creator, StaticText::GOSpawnerID);
		};

		template<const char* Text, const char* PNG, size_t idx>
		struct GeometryCreator : public __private::GuiCreator {
			// ------------------------------------------------------------------------
			/*! Default Constructor
			*
			*   Renders a Thumbnail, for sintactic sugar
			*/ //----------------------------------------------------------------------
			GeometryCreator() noexcept {
				_thumbnail(Text, PNG, this);
			}

			// ------------------------------------------------------------------------
			/*! Create
			*
			*   Creates a new Base, given the parameters
			*/ //----------------------------------------------------------------------
			DONTDISCARD Engine::IBase inline* Create() const override {
				return _new_obj_with_mesh(Text, idx);
			}
		};
	}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Presents the Palette Window
	*/ //----------------------------------------------------------------------
	void PaletteWindow::Present() {
		using __Texts::Editor::PaletteWindow::StaticText;

		//If we can show the Tools window
		if (ImGui::Begin(StaticText::Tools)) {
			//If we can show the Objects
			if (ImGui::BeginTabBar(StaticText::Objects)) {
				//And we are on the Basic Tab
				if (ImGui::BeginTabItem(StaticText::Basic)) {
					struct EmptyGameObjectCreator : public __private::GuiCreator {
						// ------------------------------------------------------------------------
						/*! Create
						*
						*   Creates an empty object
						*/ //----------------------------------------------------------------------
						Engine::IBase* Create() const override {
							return _new_obj(StaticText::NoName);
						}
					};

					static EmptyGameObjectCreator _goc;

					_button_with_id(StaticText::EmptyObject, 0, &_goc, StaticText::GOSpawnerID);

					//If we want to display the geometry options
					if (ImGui::CollapsingHeader(StaticText::Geometry)) {
						GeometryCreator<StaticText::Plane, StaticText::PlanePath, 0> _;
						GeometryCreator<StaticText::Cube, StaticText::CubePath, 1> _1;
						GeometryCreator<StaticText::Cone, StaticText::ConePath, 2> _2;
						GeometryCreator<StaticText::Cylinder, StaticText::CylinderPath, 3> _3;
						GeometryCreator<StaticText::Sphere, StaticText::SpherePath, 4> _4;
					}

					//If we want to display the emitter options
					if (ImGui::CollapsingHeader(StaticText::Emitters)) {
						struct SoundSourceCreator : public __private::GuiCreator {

							// ------------------------------------------------------------------------
							/*! Create
							*
							*   Creates an object with a Sound Emitter attached to it
							*/ //----------------------------------------------------------------------
							Engine::IBase* Create() const override {
								Engine::GameObject* const go = _new_obj(StaticText::SoundEmitter);
								UNUSED(go->AddComponent<Engine::Audio::AudioEmitter>(StaticText::SoundEmitter));
								return go;
							}
						};

						static SoundSourceCreator goc;
						_thumbnail(StaticText::SoundEmitter, StaticText::SoundEmittterPath, &goc);
					}

					ImGui::EndTabItem();
				}

				//Create a skybox object on screen
				ImGui::PushID(0);
				ImGui::Button("Skybox", ImVec2(ImGui::GetWindowWidth() - 60, 60));

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					if (GGfxPipeline->GetSkybox() == nullptr)
					{
						struct EmptySkyboxCreator : public __private::GuiCreator
						{
							Engine::IBase* Create() const override
							{
								auto Skybox = new Engine::Skybox;
								Skybox->SetName("Skybox");
								auto skycomp = Skybox->AddComponent<Engine::Skybox::SkyboxComp>("Skybox settings");
								GGfxPipeline->SetSkybox(skycomp);
								return Skybox;
							}
						};

						static EmptySkyboxCreator goc;

						// Set payload to carry the index of our item (could be anything)
						ImGui::SetDragDropPayload("VIEWPORT_GO_CLASS", &goc, sizeof(EmptySkyboxCreator));

						ImGui::Text("Create a skybox");
						ImGui::EndDragDropSource();
					}			

				}
				ImGui::PopID();


				//Begin the Tab for the C++ Classes
				if (ImGui::BeginTabItem(__Texts::Engine::GameObject::StaticText::Components)) {
					auto& _factory = *GFactory;
					const auto _fcollapsedheader = [](const char* const name,
						const decltype(_factory.EngineCreators)& creators) {
							if (ImGui::CollapsingHeader(name))
								for (auto& x : creators)
									_button_with_id(x.first.c_str(), 0, &x.first, StaticText::ComponentSpawnerID);
					};

					_fcollapsedheader(StaticText::EngineClasses, _factory.EngineCreators);
					_fcollapsedheader(StaticText::CClasses, _factory.GetCreators());
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

			ImGui::End();
		}
	}
}
#endif