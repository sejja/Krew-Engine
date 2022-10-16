//
//	ResourceWindow.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 19/09/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

#ifdef _EDITOR_
namespace Editor {
	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Presents a Resource Window
	*/ // --------------------------------------------------------------------
	void ResourceWindow::Present() noexcept {
		using __Texts::Editor::ResourceWindow::StaticText;

		//If we could show the Streamed Resources window
		if (ImGui::Begin(StaticText::Title)) {
			auto& lmap = GContent->resources;

			const auto ltextandcolumn = [](const char* text) noexcept {
				ImGui::Text(text); ImGui::NextColumn();
			};

			const auto lcolumnandseparator = [](const int i) noexcept {
				ImGui::Columns(4);
				ImGui::Separator();
			};

			const auto lfullrow = [&ltextandcolumn](DiffPtr<const char> text, DiffPtr<const char> text1, 
				DiffPtr<const char> text2, DiffPtr<const char> text3) noexcept {
					ltextandcolumn(text);
					ltextandcolumn(text1);
					ltextandcolumn(text2);
					ltextandcolumn(text3);
			};

			lcolumnandseparator(4);
			lfullrow(__Texts::Engine::GameObject::StaticText::Name, StaticText::FullPath, __Texts::Engine::GameObject::StaticText::Type, StaticText::Size);
			ImGui::Separator();

			//For every asset
			for (auto& x : lmap) {
				std::string_view a(x.first);

				a.remove_prefix(x.first.find_last_of('/') + 1);
				lfullrow(a.data(), x.first.c_str(), x.second->type, x.second->size.c_str());
			}

			lcolumnandseparator(1);
			ImGui::End();
		}
	}
}
#endif