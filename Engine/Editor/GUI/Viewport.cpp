//
//	Viewport.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 20/10/20
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

#ifdef _EDITOR_
namespace Editor {
	// ------------------------------------------------------------------------
	/*! Default Constructor
	*
	*   Initializes the viewport and creates its own framebuffer
	*/ // ---------------------------------------------------------------------
	Viewport::Viewport() :
		mDimensions({ 1920, 1080 }), mViewportBuffer(static_cast<unsigned>(mDimensions.x),
			static_cast<unsigned>(mDimensions.y)), mGizmo(Engine::Allocator<Gizmo>::New()),
		mPreviewFlags{ 0, 0 } {
		using __Texts::Editor::Viewport::StaticText;
		auto& lcontent = *GContent;

		mUITextures[eViewportTextures::ePlayTex] = lcontent.GetResource<Engine::Graphics::Texture>(StaticText::PlayTexture).get();
		mUITextures[eViewportTextures::ePauseTex] = lcontent.GetResource<Engine::Graphics::Texture>(StaticText::PauseTexture).get();
		mUITextures[eViewportTextures::eSkipTex] = lcontent.GetResource<Engine::Graphics::Texture>(StaticText::SkipTexture).get();
		mPreviewText = StaticText::Viewport;
	}

	// ------------------------------------------------------------------------
	/*! Render
	*
	*   Renders the Viewport
	*/ // --------------------------------------------------------------------
	void Viewport::Present() {
		using __Texts::Editor::Viewport::StaticText;

		auto& leditor = *GEditor;
		auto& lworld = *GWorld;

		//If we can display the toolset menu
		if (ImGui::Begin(StaticText::Toolset)) {
			ImGui::Spacing();
			ImGui::Indent(ImGui::GetWindowWidth() / 2);

			const auto lfbutton = [this, &leditor](const bool playing, const bool paused, const eViewportTextures tex, const ImVec4& color) {
				bool lreturn = ImGui::ImageButton(reinterpret_cast<void*>(static_cast<long long>(
					mUITextures[tex]->Get()->GetGLHandle())), { 16, 16 }, { 0,0 }, { 1, 1 }, 8, { 0, 0, 0, 0 }, color);
				if (lreturn) {
					leditor.SetIsPlaying(playing);
					leditor.SetPaused(paused);
					leditor.GetSelectedObject().clear();
				}

				return lreturn;
			};

			//If we are currently playing
			if (leditor.IsPlaying()) {
				//IF we have to stop the gameplay
				if (lfbutton(false, false, eViewportTextures::ePlayTex, { 0, 1, 0, 1 })) {
					ResetSceneToPreviousScene();
					mPreviewFlags.first = mPreviewFlags.second;
					GEditor->ClearSelectedObjects();
				}

				ImGui::SameLine();

				//If we are paused
				if (leditor.IsPaused()) {
					lfbutton(true, false, eViewportTextures::ePauseTex, { 1, 1, 0, 1 });
					ImGui::SameLine();

					//If we pressed the button to skip a frame
					if (lfbutton(true, true, eViewportTextures::eSkipTex, { 1, 1, 1, 1 })) {
						Engine::gCollisionSystem->Update();
						lworld.UpdateScene();
						GStateManager->Update();
						GEditor->ClearSelectedObjects();
					}
				}
				else
					//If we want to pause
					if (lfbutton(true, true, eViewportTextures::ePauseTex, { 1, 1, 1, 1 })) {
						GEditor->ClearSelectedObjects();
						GAudio->StopAll();
					}
			}
			else
				//If pressed play
				if (lfbutton(true, false, eViewportTextures::ePlayTex, { 1, 1, 1, 1 })) {
					RegisterSceneState();
					mPreviewFlags.second = mPreviewFlags.first;
					GObjectMgr->BeginPlayAll();
					GEditor->ClearSelectedObjects();
				}

			ImGui::Indent(0); ImGui::SameLine(); ImGui::SetNextItemWidth(100);

			constexpr const char* _items[] = { StaticText::Viewport, StaticText::Window };

			//If the combo is expanded
			if (ImGui::BeginCombo(StaticText::Combo, mPreviewText)) {
				//Show every opetion
				for (int n = 0; n < sizeof(_items) / sizeof(const char*); n++) {
					bool _is_selected = mPreviewText == _items[n];

					//If option selected
					if (ImGui::Selectable(_items[n], _is_selected)) {
						mPreviewText = _items[n];
						mPreviewFlags.first = static_cast<BYTE>(n);
					}

					//Reset
					if (_is_selected) ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			ImGui::End();
		}

		ImVec2 mposition;

		//Scene Preview
		if (ImGui::Begin(StaticText::Viewport)) {
			mposition = ImGui::GetWindowPos();
			ImGui::Image(reinterpret_cast<void*>(static_cast<long long>(mViewportBuffer.GetTextureID())),
				{ ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 20 }, { 0, 0 }, { 1, -1 });

			//If we have a drop target
			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(StaticText::ViewportPayloadID))
				{
					Engine::IBase* p = reinterpret_cast<Engine::Object*>(reinterpret_cast<Editor::__private::GuiCreator*>(payload->Data)->Create());
					if (GRTTI->IsA<Engine::Object>(p)) {
						leditor.GetSelectedObject().clear();
						lworld.GetScene()->SpawnObject(reinterpret_cast<Engine::Object*>(p));
						leditor.SetSelectedObject(reinterpret_cast<Engine::Object*>(p));
					}
				}

				//For loading prefabs
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(StaticText::ViewportPrefabID))
				{
					if (payload->DataSize == sizeof(std::string))
					{
						std::string Path = *(std::string*)payload->Data;

						//Spawn an object based on a prefab
						Engine::Object* p = lworld.GetScene()->SpawnObject();
						p->LoadFromPrefab(Path);
						leditor.GetSelectedObject().clear();
						leditor.SetSelectedObject(reinterpret_cast<Engine::Object*>(p));
					}
				}

				ImGui::EndDragDropTarget();
			}

			bool transformed = mGizmo->EditTransform(leditor.GetSelectedObject());
			ImGui::SetNextWindowPos({ ImGui::GetWindowPos().x + 305, ImGui::GetWindowPos().y + 35 }, ImGuiCond_Always, { 0.f, 0.f });
			ImGui::SetNextWindowBgAlpha(0.35f);

			//draw the bounding volume of the selected go
			//if (!leditor.GetSelectedObject().empty()) {
			//	for (auto& x : leditor.GetSelectedObject())
			//		GDebug->DebugDrawAABB(x->GetWorldPosition(), x->GetWorldScale(), glm::vec4(0.f, 1.f, 0.f, 1.f));
			//}

			//PICKING
			if (!GEditor->IsPlaying() && GInput->IsKeyPressed(VK_LBUTTON) &&
				!transformed) {
				Engine::Object* go = picking();
				if (go) {

					if (!GRTTI->IsA<Engine::Skybox>(go)) {
						if (!GInput->IsKeyDown(VK_SHIFT))
							leditor.GetSelectedObject().clear();

						if (std::find(leditor.GetSelectedObject().begin(), leditor.GetSelectedObject().end(),
							go) == leditor.GetSelectedObject().end())
							leditor.SetSelectedObject(go);
					}
				}
			}

			//Show the Layer Window
			if (ImGui::Begin(StaticText::Layers, nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
				ImGuiWindowFlags_NoMove)) {

				//Add the drop down menu
				if (ImGui::BeginMenu(StaticText::LayerView)) {

					//If we go for a lit scene
					if (ImGui::MenuItem(StaticText::LitScene)) leditor.mRenderOption = 0;

					//If we go for shadowmaps
					if (ImGui::BeginMenu(StaticText::ShadowMap)) {
						int i = 0;

						for (auto& x : GGfxPipeline->lightSys->lights)
							if (ImGui::MenuItem((std::string(StaticText::ShadowMap) + std::to_string(++i)).c_str()))
								leditor.mRenderOption = i;
						ImGui::EndMenu();
					}

					bool renderinvisible = leditor.RenderInvisible(), wireframe = leditor.RenderWireframe(), boundingbox = leditor.RenderBoundingBoxes();
					ImGui::Checkbox(StaticText::RenderInvisibleObjects, &renderinvisible); leditor.ShouldRenderInvisible(renderinvisible);
					ImGui::Checkbox(StaticText::Wireframe, &wireframe); leditor.ShouldRenderWirefrane(wireframe);
					ImGui::Checkbox(StaticText::BoundingBoxes, &boundingbox); leditor.ShouldRenderBoundingBoxes(boundingbox);
				}

				ImGui::End();
			}

			ImGui::End();
		}

		mposition.y += 20;
		ImGui::SetNextWindowPos(mposition);
		leditor.RenderDebugText();

		//If we are on window mode and we press scape
		if (GInput->IsKeyPressed(VK_ESCAPE) && mPreviewFlags.first && leditor.IsPlaying())
			mPreviewFlags.first = 0;
	}

	// ------------------------------------------------------------------------
	/*! Register Scene State
	*
	*   Register previews scene state
	*/ // --------------------------------------------------------------------
	void Viewport::RegisterSceneState() {
		mTemporalScene.clear();
		GWorld->GetScene()->ToJson(mTemporalScene);
	}

	// ------------------------------------------------------------------------
	/*! Reset Scene to previous scene
	*
	*   Resets the scene to its previous state
	*/ // --------------------------------------------------------------------
	void Viewport::ResetSceneToPreviousScene() const {
		GWorld->GetScene()->FromJson(mTemporalScene);
	}

	Engine::Object* Viewport::picking() {
		//get mouse pos
		ImVec2 mouse_pos = ImGui::GetIO().MousePos;

		//check if the mouse click is in the window
		if (mouse_pos.x < ImGui::GetWindowPos().x || mouse_pos.x >(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x) ||
			mouse_pos.y < ImGui::GetWindowPos().y || mouse_pos.y >(ImGui::GetWindowPos().y + ImGui::GetWindowSize().y))
			return nullptr;

		//offset from window to viewport;
		float offset_X = ImGui::GetWindowPos().x - ImGui::GetWindowViewport()->Pos.x;
		float offset_Y = ImGui::GetWindowPos().y - ImGui::GetWindowViewport()->Pos.y;

		//map from viewport to window
		mouse_pos.x -= offset_X;
		mouse_pos.y -= offset_Y;

		//NDC coordinates
		float x = (2 * mouse_pos.x) / ImGui::GetWindowSize().x - 1.f;
		float y = -1.f + (2 * mouse_pos.y) / ImGui::GetWindowSize().y;	//-1 top left

		const auto& view = GGfxPipeline->editCam.GetCam()->GetViewMtx();
		const auto& proj = GGfxPipeline->editCam.GetCam()->GetProjMtx();

		glm::mat4 invVP = glm::inverse(proj * view);
		glm::vec4 screenPos = glm::vec4(x, -y, 1.0f, 1.0f);
		glm::vec4 worldPos = invVP * screenPos;

		glm::vec3 dir = glm::normalize(glm::vec3(worldPos));

		Engine::Ray r;
		r.start = GGfxPipeline->editCam.GetCam()->GetPos();
		r.dir = dir;

		//get all objects
		const Engine::Array<Engine::Object*>& obj_list = GWorld->GetScene()->get_all_objects();
		Engine::Object* to_return = nullptr;

		float t_min = 10000000.f; //very big number
		//iterate through all the game objects
		for (auto& it : obj_list) {
			float t = intersection_time_ray_aabb(r, Engine::AABB(it));

			if (t > 0.f && t < t_min) {
				t_min = t;
				to_return = it;
			}
		}

		return to_return;
	}
}

#endif