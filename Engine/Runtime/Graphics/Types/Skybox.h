/* -----------------------------------------------------------------------------------
	Copyright (C) 2021 Diego Revilla Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior written
	consent of Diego Revilla Institute of Technology is prohibited.

	File: Skybox.h
	Author: Javier Madariaga (javier.madariaga)
	Creation date: 09/29/2021
	Brief:
		Declaration of class Skybox. A special GameObject that is rendered
		as a background for the world. Includes a unique component,
		called SkyboxComp.

------------------------------------------------------------------------------------*/

#pragma once

namespace Engine
{
	struct Mesh;

	class Skybox : public GameObject
	{
	public:
		class SkyboxComp : public Component
		{
		public:
			void OnEditor() override;

			bool mVisible = true;
			bool mMoving = false;
			float rotation = 0.0f;
			float speed = 0.1f;
			glm::vec3 axis = { 0, 1, 0 };
			glm::mat4 uvRot = glm::mat4(1);
			std::string mCubeMapPath = "";

			//Read / write
			void FromJson(const nlohmann::json& val) noexcept;
			void ToJson(nlohmann::json& val) const noexcept;
			SkyboxComp();
			~SkyboxComp();

			void SetCubeMapPath(std::string path);
			void LoadShader(std::string vtxPath, std::string fragPath);
			void CreateCubeMap();
			void RenderSkybox();
			bool IsVisible();
		private:

			glm::ivec2 mTexSize; //Texture size (used when loading textures)
			Graphics::ShaderProgram* mProgram = nullptr; //Shader program
			Mesh* mMesh = nullptr;
			//Camera* mCamera = nullptr

			//Texture handles
			GLuint mMapHandle;
			GLuint mFBOHandle[6] = { 0 };

			//TEMP: values for camera transformations
			glm::mat4* mCamMtx = nullptr;
			glm::mat4* mProjMtx = nullptr;

		};
	};
}