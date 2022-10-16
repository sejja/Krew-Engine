//
//	MemoryTrackerWindow.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 19/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

#ifdef _EDITOR_
namespace Editor {
	// ------------------------------------------------------------------------
   /*! Present
   *
   *   Presents the Memory Tracker Diagnostics
   */ // --------------------------------------------------------------------
	void MemoryTrackerWindow::Present() {
		//If we could show the Streamed Resources window
		if (ImGui::Begin("Memory Tracker")) {
#ifdef _DEBUG
#if __cpp_consteval
			auto& _memdbg = *GMemoryDbg;
			auto& _map = _memdbg.mBlock;
			const auto _fheadertext = [](const char* str) {
				ImGui::Text(str); ImGui::NextColumn();
			};

			const auto _frowtext = [&_fheadertext](size_t str) {
				_fheadertext(std::to_string(str).c_str());
			};

			ImGui::Columns(6, "mycolumns"); ImGui::Separator();
			_fheadertext("Function"); _fheadertext("Allocations"); _fheadertext("Bytes");
			_fheadertext("Reallocations"); _fheadertext("Deallocations"); _fheadertext("Last Allocation Frame");
			ImGui::Separator();

			//For every asset
			for (auto& x : _map) {
				std::string_view a(x.first);

				a.remove_prefix(x.first.find_last_of("/\\") + 1);
				_fheadertext(a.data()); _frowtext(x.second.mAllocationCount);
				_fheadertext(Engine::Math::Conversions::ConvertSize(static_cast<unsigned>(
					x.second.mAllocationBytes)).c_str()); _frowtext(x.second.mReallocationCount);
				_frowtext(x.second.mDeallocationCount); _frowtext(x.second.mLastAllocationFrame);
			}

			ImGui::Columns(1); ImGui::Separator(); ImGui::Separator();

			ImGui::Text("Current individual allocations: %i", _memdbg.mCurrentAllocations.size());
			size_t _allmem = 0;

			for (auto& x : _memdbg.mCurrentAllocations) _allmem += x.second;
			
			ImGui::Text("Current allocated Memory: %s", Engine::Math::Conversions::ConvertSize(
				static_cast<unsigned>(_allmem)).c_str());
			ImGui::Separator();

			if (float* _data = (float*)_malloca(_memdbg.mBlock.size() * sizeof(float))) {
				_allmem = 0;
				
				for (auto& x : _map) _data[_allmem++] = static_cast<float>(x.second.mAllocationBytes);
				
				ImGui::PlotHistogram("", _data, _allmem, 0, "Bytes", 0, std::numeric_limits<float>::max(), { ImGui::GetWindowWidth() - 20, 200 });
				_freea(_data);
			} else
				ImGui::TextColored({ 1, 0, 0, 1 }, "Failed to represent memory usage - NO STACK MEMORY");
			ImGui::End();
#else
			ImGui::Text("You need a newer version of MSCV for Memory Tracking");
#endif
#else
			ImGui::Text("Memory Tracking is disabled in release builds");
#endif
		}
	}
}
#endif