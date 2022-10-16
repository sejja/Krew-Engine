//
//  SharedResources.h
//  Good Neighbours
//
//  Created by Diego Revilla on 4/11/20.
//  Copyright � 2020 Diego Revilla. All rights reserved.
//

#ifndef _SHARED_RESOURCES__H_
#define _SHARED_RESOURCES__H_

#ifdef _EDITOR_
namespace Editor {
	class ResourceWindow;
}
#endif

namespace Engine {
	namespace Assets {
		struct ENGINE_API IResource {
			virtual ~IResource();
		public:
			const char* type = "DATA";
			std::string size;
		};

		class IResourceImporter {
		public:
			enum eImportMode {
				eInmediate,
				eAsync
			};

			virtual IResource* ImportFromFile(const char* filename, eImportMode mode) const = 0;
			virtual void AsyncLoad(const char* filename, IResource*& asset);
			virtual void AsyncIntegrate(IResource*& asset);
		};

		template<typename Ty_>
		class ENGINE_API TResource : public IResource {
		public:
			~TResource() {
				Allocator<Ty_> alloc;
				alloc.destroy(rawData);
				alloc.deallocate(rawData);
			}
			// ------------------------------------------------------------------------
			/*! Get
			*
			*   Gets the Resource as something
			*/ // --------------------------------------------------------------------
			DONTDISCARD Ty_ inline* Get() {
				return reinterpret_cast<Ty_*>(rawData);
			}

			Ty_* rawData = nullptr;
		};
	}

	class ENGINE_API ResourceManager : public Singleton<ResourceManager> {
#ifdef _EDITOR_
		friend class Editor::ResourceWindow;
#endif
	public:
		using raw_text = const char*;

		// ------------------------------------------------------------------------
		/*! Get Project Path
		*
		*   Gets the Path to the current project
		*/ // --------------------------------------------------------------------
		DONTDISCARD const raw_text inline GetProjectPath() const noexcept {
			return projectPath.c_str();
		}

		// ------------------------------------------------------------------------
		/*! Get Engine Path
		*
		*   Gets the Path to where the Engine resides
		*/ // --------------------------------------------------------------------
		DONTDISCARD const raw_text inline GetEnginePath() const noexcept {
			return enginePath.c_str();
		}

		// ------------------------------------------------------------------------
		/*! Get Resource
		*
		*   Gets a Loaded resource, if not loaded load it
		*/ // --------------------------------------------------------------------
		template<typename Ty_>
		DONTDISCARD std::shared_ptr<Assets::TResource<Ty_>> GetResource(raw_text name, Assets::IResourceImporter::
			eImportMode mode = Assets::IResourceImporter::eImportMode::eAsync) noexcept {
			return std::reinterpret_pointer_cast<Assets::TResource<Ty_>>(GetResource(name, mode));
		}

		// ------------------------------------------------------------------------
		/*! Get Engine Resource
		*
		*   Gets a resource located at the engine
		*/ // --------------------------------------------------------------------
		template<typename Ty_>
		DONTDISCARD std::shared_ptr<Assets::TResource<Ty_>> inline GetEngineResource(raw_text name) noexcept {
			return std::reinterpret_pointer_cast<Assets::TResource<Ty_>>(GetEngineResource(name));
		}

		void SetProjectPath(raw_text str) noexcept;
		DONTDISCARD std::shared_ptr<Assets::IResource> GetResource(raw_text name, Assets::IResourceImporter::eImportMode mode);
		DONTDISCARD std::shared_ptr<Assets::IResource> GetEngineResource(raw_text name);
		void RemoveResource(raw_text name) noexcept;
		bool ReloadResource(raw_text name) noexcept;
		void Initialize();
		DONTDISCARD Assets::IResourceImporter* GetImporterByExtension(raw_text ext) const noexcept;
		void AddImporter(Assets::IResourceImporter* res, raw_text ext) noexcept;
		void ResetManagerUsageFlag() noexcept;
		void CollectGarbage();
		void ReloadContent();
		void ShutDown();
		void Update();
		unsigned PendingResources() { return pendingassets; }

	protected:
		std::shared_ptr<Assets::IResource> AddResource(raw_text mPath, Assets::IResourceImporter::eImportMode mode);

		std::map<std::string, std::shared_ptr<Assets::IResource>> resources;
		std::map<std::string, Assets::IResourceImporter*> importers;
		unsigned pendingassets = 0;
		std::string enginePath;
		std::string projectPath;
		Threading::ThreadPool pool{3};
		std::stack<std::pair<Assets::IResourceImporter*, Assets::IResource*>> mIntegrateAssets;
	};
}

#define GContent (&Engine::ResourceManager::Instance())

template<typename T>
using Asset = std::shared_ptr<Engine::Assets::TResource<T>>;

template<typename T>
using AssetReference = std::weak_ptr<Engine::Assets::TResource<T>>;

#endif
