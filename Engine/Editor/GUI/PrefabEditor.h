#pragma once
namespace Editor {
	class PrefabWindow : public IWindow {
#pragma region // Functions
	public:
		PrefabWindow();
		~PrefabWindow();
		void Present() noexcept override;
		void SetPath(const std::string& s);
		std::string mPath;
		nlohmann::json mCurrentEdition;
#pragma endregion
	};
}