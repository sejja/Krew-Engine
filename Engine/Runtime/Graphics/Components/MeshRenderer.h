//
//	MeshRenderer.h
//	Good Neighbours
//
//	Created by Fernando Uribe on 09/17/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#ifndef _GFX_MESH_RENDERER__H_
#define _GFX_MESH_RENDERER__H_

namespace Engine
{
	static const std::vector<const char*> meshTypes = {
		"Plane",
		"Cube",
		"Cone",
		"Cylinder",
		"Sphere",
		"Light",
		"Particle"
	};

	struct ENGINE_API MeshRenderer : public RenderComp
	{
		friend struct GraphicsPipeline;

		struct MaterialData
		{
			glm::vec3 amb = glm::vec3(0.0f);
			glm::vec3 dif = glm::vec3(1.0f);
			glm::vec3 spe = glm::vec3(0.5f);
			float shiny = 15.0f;
			float dashImageVar = 1.f;
		};

		MeshRenderer(bool isHUD = false);
		~MeshRenderer();

		void FromJson(const nlohmann::json& val)override;
		void ToJson(nlohmann::json& val) const override;

		virtual void OnEditor()override;

		void SetTexture(AssetReference<Graphics::Texture> t);
		void SetTextureByName(const std::string& name);

		Mesh* GetMesh()const;
		void SetMesh(Mesh* m);
		void SetMeshByName(const std::string& name);
		void SetMeshByIdx(int i);

		void SetIsVisible(bool b);
		bool GetIsVisible()const;

		void SetHasShadow(bool b);
		bool GetHasShadow()const;

		const Mesh* get_mesh() const { return mesh; }

		void SetShader(Graphics::ShaderProgram* s);
		void SetShaderByIdx(int i);

		virtual void Render();
		void RenderHUDs(glm::mat4 world);
		void ShadowRender();

		void setAlphaValue(const float& v) { mat.dashImageVar = v; }

		// octree data
		Octree<MeshRenderer>::node* m_octree_node = nullptr;
		MeshRenderer* m_octree_next_obj = nullptr;
		MeshRenderer* m_octree_prev_obj = nullptr;
		int m_ID = -1;
		bool m_render = true;

	protected:
		bool isHUD = false;
		bool mWireframe = false;
		int useAvg = false;
		bool debugMode = false;
		Mesh* mesh = nullptr;
		int meshIdx = 1;
		Graphics::ShaderProgram* shader = nullptr;
		int shaderIdx = 0;
		std::string textureName = __Texts::Engine::RenderComp::StaticText::DefaultTexture;
		Asset<Graphics::Texture> texture = nullptr;
		MaterialData mat;
	};
}

#endif
