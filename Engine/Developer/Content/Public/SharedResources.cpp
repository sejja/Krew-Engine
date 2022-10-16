//
//  SharedResources.cpp
//  Good Neighbours
//
//  Created by Diego Revilla on 4/11/20.
//  Copyright � 2020 Diego Revilla. All rights reserved.
//

#include <Shared.h>

namespace Engine {
	// ------------------------------------------------------------------------
	/*! Initialize
	*
	*  Initializes the Resource Manager
	*/ // --------------------------------------------------------------------
	void ResourceManager::Initialize() {
		enginePath = std::filesystem::current_path().string() += "\\EngineContent\\";
		importers.insert({"png", new Engine::Assets::Importers::TextureImporter});
		importers.insert({ "jpg", new Engine::Assets::Importers::TextureImporter });
		importers.insert({"program", new Engine::Assets::Importers::ShaderProgramImporter});
		importers.insert({ "wav", new Engine::Assets::Importers::AudioImporter });
		importers.insert({ "mp3", new Engine::Assets::Importers::AudioImporter });
	}

	// ------------------------------------------------------------------------
	/*! Get Resource
	*
	*   Gets a resource given the name
	*/ // --------------------------------------------------------------------
	std::shared_ptr<Assets::IResource> ResourceManager::GetResource(raw_text name, Assets::IResourceImporter::eImportMode mode) {
		auto it = resources.find(name);

		//If we do indeed have the resource
		if (it != resources.end()) {
			return it->second;
		}

		return AddResource(name, mode);
	}

	// ------------------------------------------------------------------------
	/*! Reset Manager Usage Flag
	*
	*   Sets all assets to unused
	*/ // --------------------------------------------------------------------
	void ResourceManager::ResetManagerUsageFlag() noexcept {
	}

	// ------------------------------------------------------------------------
	/*! Get Resource
	*
	*   Gets a resource given the name
	*/ // --------------------------------------------------------------------
	std::shared_ptr<Assets::IResource> ResourceManager::GetEngineResource(raw_text name) {
		std::string mPath(enginePath + name);

		auto it = resources.find(mPath);

		//If we do indeed have the resource
		if (it != resources.end()) {
			return it->second;
		}

		return AddResource(mPath.c_str(), Assets::IResourceImporter::eImportMode::eInmediate);
	}

	// ------------------------------------------------------------------------
	/*! Add Importer
	*
	*   Adds an Importer to the Resource Manager
	*/ // --------------------------------------------------------------------
	void ResourceManager::AddImporter(Assets::IResourceImporter* res, raw_text ext) noexcept {
		importers.insert(std::make_pair(ext, res));
	}

	static Array<std::string> toloadresources;

	// ------------------------------------------------------------------------
	/*! Add Resource
	*
	*   Adds a Resource given the resource, the path and the name
	*/ // --------------------------------------------------------------------
	std::shared_ptr<Assets::IResource> ResourceManager::AddResource(raw_text mPath, Assets::IResourceImporter::eImportMode mode) {
		std::string_view temp_(mPath);

		//assertThis(!temp_.empty(), "Maybe you should think about giving an actual asset path, you fucking clown");
		if (temp_.empty()) return nullptr;

		//Try importing from file
		try {
			Assets::IResourceImporter* ireimp_ =
				GetImporterByExtension(temp_.substr(temp_.find_last_of(".") + 1).data());
			std::shared_ptr<Assets::IResource> res_(ireimp_->ImportFromFile(mPath, mode));

			auto _fasyncload = [this](std::string filename, Assets::IResourceImporter* importer, Assets::IResource* res,	
				std::stack<std::pair<Assets::IResourceImporter*, Assets::IResource*>>* resources) {
					importer->AsyncLoad(filename.c_str(), res);
					resources->push({ importer, res });
			};

			if (mode == Assets::IResourceImporter::eImportMode::eAsync) {
				pool.AddTask(_fasyncload, std::string(mPath), ireimp_, res_.get(), &mIntegrateAssets);
				pendingassets++;
			}

			if (res_)
				resources.insert(std::make_pair(mPath, res_));

			return res_;

			//If we could not find an importer
		}
		catch (...) {
			return nullptr;
		}
	}

	// ------------------------------------------------------------------------
	/*! Collect Garbage
	*
	*   REmoves all unused content
	*/ // --------------------------------------------------------------------
	void ResourceManager::CollectGarbage() {
	}

	// ------------------------------------------------------------------------
	/*! Get Importer By Extension
	*
	*   Gets an importer by it's extension
	*/ // --------------------------------------------------------------------
	Assets::IResourceImporter* ResourceManager::GetImporterByExtension(raw_text ext) const noexcept {
		return importers.at(ext);
	}

	// ------------------------------------------------------------------------
	/*! Remove Resource
	*
	*   Removes a resource
	*/ // --------------------------------------------------------------------
	void ResourceManager::RemoveResource(raw_text name) noexcept {
		resources.erase(name);
	}

	// ------------------------------------------------------------------------
	/*! Reload Resource
	*
	*   REloads a resource given it's path
	*/ // --------------------------------------------------------------------
	bool ResourceManager::ReloadResource(raw_text) noexcept {
		return false;
	}

	// ------------------------------------------------------------------------
	/*! Set Project Path
	*
	*   Sets the project path
	*/ // --------------------------------------------------------------------
	void ResourceManager::SetProjectPath(raw_text str) noexcept {
		std::filesystem::current_path(str);
		projectPath = str;
	}

	// ------------------------------------------------------------------------
	/*! Reload Content
	*
	*   Reloads all the content that has been modified outside the Engine
	*/ // --------------------------------------------------------------------
	void ResourceManager::ReloadContent() {
	}

	void ResourceManager::ShutDown() {
		std::for_each(importers.begin(), importers.end(), [](const std::pair<std::string, Assets::IResourceImporter*>& x)
			{
				Allocator<Assets::IResourceImporter>::deallocate(x.second);
			});

		toloadresources.clear();
		resources.clear();
	}

	void ResourceManager::Update() {
		while (!mIntegrateAssets.empty()) {
			auto p = mIntegrateAssets.top();
			mIntegrateAssets.pop();
			p.first->AsyncIntegrate(p.second);
			pendingassets--;
		}
	}

	namespace Assets {
		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   EMPTY FUNCTION
		*/ // --------------------------------------------------------------------
		IResource::~IResource() {}
		void IResourceImporter::AsyncLoad(const char* filename, IResource*&) {}
		void IResourceImporter::AsyncIntegrate(IResource*&) {}
	}
}