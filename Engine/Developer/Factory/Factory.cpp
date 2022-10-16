//
//  Factory.cpp
//  Good Neighbours
//
//  Created by Diego Revilla on 28/9/20.
//  Copyright � 2020 Diego Revilla. All rights reserved.
//

#include <Shared.h>

namespace Engine {
	static std::map<const std::string, UniquePointer<Factory::ICreator>> Creators;

	// ------------------------------------------------------------------------
	/*! Create
	*
	*   Creates a type given the name
	*/ //--------------------------------------------------------------------
	IBase* Factory::Create(const char* class_type) {
		auto it = Creators.find(class_type);

		//if we find a valid creator
		if (it != Creators.end())
			return it->second->Create();

		return nullptr;
	}

	void Factory::ShutDown() {
		EngineCreators.clear();
		Creators.clear();
	}

	// ------------------------------------------------------------------------
	/*! Register All Types
	*
	*   Creates Creators for all types in the game
	*/ //--------------------------------------------------------------------
	void Factory::RegisterAllTypes() {
		Register<Collider>("Collider");
		Register<Audio::AudioEmitter>("Sound Emitter");
		Register<TriggerBox>("TriggerBox");
		Register<Camera>("Camera");
		Register<RigidBody>("RigidBody");
		Register<MeshRenderer>("Mesh Renderer");
		Register<Light>("Light");
		Register<SpringArm>("SpringArm");
		Register<Graphics::ParticleEmitter>("ParticleEmitter");
    Register<Text3D>("3D Text");
		Register<Audio::SoundEmitter3D>("3D Sound Emitter");
		Register<Audio::SoundListener3D>("3D Sound Listener");
		Register<Graphics::Model3D>("3D Model");
		Register<Skybox::SkyboxComp>("Skybox settings");
		Register<Graphics::FogComponent>("Fog");
	}

	IBase* Factory::CreateTypeByName(const char* nam) {
		auto it = Creators.find(nam);

		//If there are creators
		if (it != Creators.end())
			return it->second->Create();

		it = EngineCreators.find(nam);

		//If there are creators
		if (it != EngineCreators.end())

			return it->second->Create();
		GConsole->Log(Console::eMessageType::e_Message, std::string("No such component" + std::string(nam)).c_str());
		//throw std::exception(
			//std::string("No such component" + std::string(nam)).c_str()
		//);
		return nullptr;
	}
	void Factory::ClearCreators() {
		Creators.clear();
	}
	std::map<const std::string, UniquePointer<Factory::ICreator>>& Factory::GetCreators() {
		return Creators;
	}
	void Factory::_register_custom_creator(const char* name, ICreator* t) {
		Creators[name] = t;
	}
}
