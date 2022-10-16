//
//	Gizmo.cpp
//	Good Neighbours
//
//	Created by Jon Ibarra on 17/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

#ifdef _EDITOR_
#define GLM_ENABLE_EXPERIMENTAL

namespace Editor {
	// ------------------------------------------------------------------------
	/*! Default Constructors
	*
	*   Initializes ImGuizmo API
	*/ // ---------------------------------------------------------------------
	Gizmo::Gizmo() noexcept :
		mOperation(ImGuizmo::OPERATION::TRANSLATE), mMode(ImGuizmo::MODE::LOCAL), mShowGrid(false), mGridSize(100) {
		ImGuizmo::Enable(true), ImGuizmo::SetOrthographic(false);
	}

	// ------------------------------------------------------------------------
	/*! Edit Transform
	*
	*   Edits the Transform of an object using Guizmos
	*/ // ---------------------------------------------------------------------
	bool Gizmo::EditTransform(const std::list<Engine::GameObject*>& object) {
		ImGui::SetNextWindowPos({ ImGui::GetWindowPos().x + 15, ImGui::GetWindowPos().y + 35 }, ImGuiCond_Always, { 0.f, 0.f });
		bool _p_open = true;
		ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
		auto& _gfx = *GGfxPipeline;

		if (ImGui::Begin("Viewport Options", &_p_open, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav |
			ImGuiWindowFlags_NoMove)) {
			//If Selected Translation Option
			if (ImGui::Button("T")) mOperation = ImGuizmo::OPERATION::TRANSLATE;
			ImGui::SameLine();

			//If we want to rotate
			if (ImGui::Button("R")) mOperation = ImGuizmo::OPERATION::ROTATE;
			ImGui::SameLine();

			//If we want to scale
			if (ImGui::Button("S")) mOperation = ImGuizmo::OPERATION::SCALE;

			ImGui::Separator();

			int _tempfloat = _gfx.editCam.GetCam()->GetSensitivity(); //Change when there is an editro camera

			ImGui::SliderInt("Camera Speed", &_tempfloat, 1, 9);
			_gfx.editCam.GetCam()->SetSensitivity(static_cast<int>(_tempfloat));
			ImGui::Checkbox("Show Grid", &mShowGrid);

			if (mShowGrid) ImGui::InputInt("Grid Size", &mGridSize);

			ImGui::End();
		}

		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(),
			ImGui::GetWindowHeight());

		ImGuizmo::ViewManipulate(glm::value_ptr(_gfx.mtxCam), 100, ImVec2(ImGui::GetWindowPos().x + (float)ImGui::GetWindowWidth() - 128, ImGui::GetWindowPos().y), ImVec2(128, 128), 0x10101010);

		glm::mat4 id(1);

		if (mShowGrid || GInput->IsKeyDown('G'))
			ImGuizmo::DrawGrid(glm::value_ptr(_gfx.mtxCam), glm::value_ptr(_gfx.mtxProj), glm::value_ptr(id),
				static_cast<float>(mGridSize));

		//If we have an object to edit
		if (!object.empty()) {
			if (ImGui::IsKeyPressed('R')) mOperation = ImGuizmo::OPERATION::TRANSLATE;
			if (ImGui::IsKeyPressed('T')) mOperation = ImGuizmo::OPERATION::ROTATE;
			if (ImGui::IsKeyPressed('Y')) mOperation = ImGuizmo::OPERATION::SCALE;

			const float _windowWidth = ImGui::GetWindowWidth() - 15;
			const float _windowHeight = ImGui::GetWindowHeight() - 35;;
			auto& _geditor = *GEditor;

			//If we have a current base
			if (!_geditor.IsPlaying()) {

				Engine::Math::Transform tr;

				for (auto& x : object) {
					tr.mPos += x->GetWorldPosition();
				}

				tr.mPos /= object.size();

				glm::mat4 _transform = tr.GetModelMtx();

				ImGuizmo::Manipulate(glm::value_ptr(_gfx.mtxCam), glm::value_ptr(_gfx.mtxProj), mOperation, ImGuizmo::MODE::WORLD,
					glm::value_ptr(_transform), nullptr, nullptr);

				//If we are editing with the gizmo
				if (ImGuizmo::IsOver()) {
					glm::vec3 translate, scale, rotate;
					Engine::Math::fast_decompose_transfom(_transform, translate, rotate, scale);

					translate.x -= tr.mPos.x; translate.y -= tr.mPos.y; translate.z -= tr.mPos.z;
					rotate.x -= tr.mRot.x; rotate.y -= tr.mRot.y; rotate.z -= tr.mRot.z;
					scale.x -= tr.mScale.x; scale.y -= tr.mScale.y; scale.z -= tr.mScale.z;

					switch (mOperation) {
					case ImGuizmo::OPERATION::TRANSLATE:
						// Setting translation
						for (auto& x : object) {
							x->SetPosition(x->GetPosition() + translate);
						}
						break;

					case ImGuizmo::OPERATION::ROTATE:
						// Setting the rotation
						for (auto& x : object) {
							x->SetRotation(x->GetRotation() + rotate);
						}
						break;

					case ImGuizmo::OPERATION::SCALE:
						// Setting the scale
						for (auto& x : object) {
							x->SetScale(x->GetScale() + scale);
						}
					}

					return true;
				}
			}
		}

		return false;
	}
}
#endif