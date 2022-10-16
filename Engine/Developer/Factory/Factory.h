//
//  Factory.h
//  Good Neighbours
//
//  Created by Diego Revilla on 28/9/20.
//  Copyright � 2020 Diego Revilla. All rights reserved.
//

#ifndef _FACTORY__H_
#define _FACTORY__H_

namespace Editor {
	class PaletteWindow;
}

namespace Engine {
	class ENGINE_API Factory : public Singleton<Factory> {
	public:
		friend Editor::PaletteWindow;

		struct ICreator {
			virtual IBase* Create() = 0;
		};

		std::map<const std::string, UniquePointer<ICreator>> EngineCreators;

		void RegisterAllTypes();
		IBase* Create(const char* class_type);
		void ShutDown();

		template<typename Ty_>
		struct ENGINE_API TCreator : public ICreator {
			TCreator() {
			}

			// ------------------------------------------------------------------------
			/*! Create
			*
			*   Creates a new Class Given the name
			*/ //--------------------------------------------------------------------
			virtual IBase* Create() {
				return Allocator<Ty_>::New();
			}
		};

		// ------------------------------------------------------------------------
		/*! Register
		*
		*   Registers a new type, given the type and the name
		*/ //--------------------------------------------------------------------
		template<typename Ty_>
		void Register(const char* name) {
			EngineCreators[name] = Allocator<TCreator<Ty_>>::New();
		}

		// ------------------------------------------------------------------------
		/*! Register
		*
		*   Registers a new type, given the type and the name
		*/ //--------------------------------------------------------------------
		template<typename Ty_>
		void RegisterCustomCreator(const char* name) {
			_register_custom_creator(name, Allocator<TCreator<Ty_>>::New());
		}

		IBase* CreateTypeByName(const char* nam);
		void ClearCreators();
		std::map<const std::string, UniquePointer<ICreator>>& GetCreators();

	private:
		void _register_custom_creator(const char* name, ICreator* t);

	};
}

#define GFactory (&Engine::Factory::Instance())

#endif
