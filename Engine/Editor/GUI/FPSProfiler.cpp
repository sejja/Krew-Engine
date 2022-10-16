//
//  FPSProfiler.h
//  Good Neighbours
//
//  Created by Diego Revilla 01/10/21.
//  Copyright � 2021 Diego Revilla. All rights reserved.
//

#include <Shared.h>

#ifdef _EDITOR_
namespace Editor {
	// ------------------------------------------------------------------------
	/*! Plot Variable
	*
	*   Plots the evolution of one variable
	*/ // ---------------------------------------------------------------------
	void PlotVar(const char* label, const float value, const float scale_min, const float scale_max,
		int buffer_size) noexcept {
		struct PlotVarData {
			ImGuiID        ID;
			Engine::Array<float>  Data;
			int            DataInsertIdx;
			int            LastFrame;

			PlotVarData() : ID(0), DataInsertIdx(0), LastFrame(-1) {}
		};

		static std::map<ImGuiID, PlotVarData> s_PlotVarsMap;

		if (!buffer_size) buffer_size = 120;

		ImGui::PushID(label);
		ImGuiID id = ImGui::GetID("");
		PlotVarData& pvd = s_PlotVarsMap[id];

		pvd.Data.reserve(buffer_size);

		if (pvd.DataInsertIdx == buffer_size)
			pvd.DataInsertIdx = 0;

		const int display_idx = pvd.DataInsertIdx;

		pvd.Data[pvd.DataInsertIdx++] = value;

		const int current_frame = ImGui::GetFrameCount();

		//If we have plotting space
		if (pvd.LastFrame != current_frame) {
			ImGui::PlotLines("##plot", pvd.Data.data(), buffer_size, pvd.DataInsertIdx, NULL,
				scale_min, scale_max, ImVec2(0, 40));
			ImGui::SameLine();
			ImGui::Text("%s\n%-3.4f", label, pvd.Data[display_idx]);
			pvd.LastFrame = current_frame;
		}

		ImGui::PopID();
	}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Profiles the FPS on a window
	*/ // ---------------------------------------------------------------------
	void FPSProfiler::Present() noexcept {
		//If we could create the window
		if (ImGui::Begin("Performance")) {
			PlotVar("FPS", GFrames->GetFrameRate(), 0, 60, 0);
			PlotVar("Physics", GFrames->PhysicsTime(), 0, 60, 0);
			PlotVar("Logic", GFrames->LogicTime(), 0, 60, 0);
			PlotVar("Graphics", GFrames->GraphicsTime(), 0, 60, 0);
			ImGui::End();
		}
	}
}

#endif