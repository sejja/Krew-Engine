//
//	Lighting.h
//	Good Neighbours
//
//	Created by Fernando Uribe on 09/17/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#ifndef _GFX_LIGHTING__H_
#define _GFX_LIGHTING__H_

namespace Engine
{
	namespace Graphics
	{
		class Model3D;
	}

	struct ENGINE_API Light : public Component
	{
		Light();
		virtual ~Light();

		void FromJson(const nlohmann::json& val)override;
		void ToJson(nlohmann::json& val) const override;

		void OnEditor()override;

		void Update();
		void UpdateShadows();
		void UpdateFrustum();
		bool CheckFrustum();
		virtual void UpdateMtx();
		//int type = 0;
		//Camera space
		glm::vec3 target = glm::vec3(0.0f);
		glm::vec3 pos = glm::vec3(0.0f);
		glm::vec3 dir = glm::vec3(0.0f);
		//Color
		glm::vec3 att = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 amb = glm::vec3(0.0f);
		glm::vec3 dif = glm::vec3(1.0f);
		glm::vec3 spe = glm::vec3(1.0f);
		//Spot light stuff
		float angleIn = 20.0f;
		float angleOut = 30.0f;
		float fallOff = 1.0f;
		//Shadow stuff
		float nearP = 0.1f;
		float farP = 1000.0f;
		float bias = 0.0000001f;
		int neighbor = 2;
		glm::mat4 mtx = glm::mat4(1);
		int shadowID = 0;
		bool debugMode = false;
		bool isStatic = false;
		bool needUpdate = false;
		bool firstUpdate = true;
		std::vector<MeshRenderer*> inFrustumMesh;
		std::vector<Graphics::Model3D*> inFrustumModel;
	};

	struct LightingSystem
	{
		LightingSystem();
		~LightingSystem();
		void UpdateFrustums();
		void Update();

		static constexpr float shadowMapSize = (1 << 12);
		static constexpr int maxLights = 8;
		int numLights = 0;
		std::vector<Light*> lights;
		std::unordered_map<GameObject*, bool> mUpdatePending;
		unsigned depthTex[maxLights] = { 0 };
		unsigned depthFBO[maxLights] = { 0 };
		bool depthBool[maxLights] = { 0 };
	};
}
#endif