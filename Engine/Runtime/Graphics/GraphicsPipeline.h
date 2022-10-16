//
//	GraphicsPipeline.h
//	Good Neighbours
//
//	Created by Fernando Uribe on 09/17/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _GFX_PIPELINE__H_
#define _GFX_PIPELINE__H_

namespace Editor {
	class Gizmo;
}

namespace Engine {

	namespace Graphics {
		class ParticleEmitter;
		class ShaderProgram;
		class Model3D;

		struct ShaderList
		{
			static const int customSize = 2;
			const char* customNames[customSize] =
			{
				"simpleShader",
				"skeletalAnimShader",
			};
			union
			{
				ShaderProgram* customs[customSize];

				struct
				{
					ShaderProgram* simpleShader;
					ShaderProgram* skeletalAnimShader;
				};
			};
			ShaderProgram* shaderParticle;
			ShaderProgram* TextShader;
			ShaderProgram* SkyboxShader;
			ShaderProgram* PostProcessShader;
			ShaderProgram* HUDShader;
			ShaderProgram* shadowShader;
			ShaderProgram* shadowRenderShader;
			ShaderProgram* lineShader;
			ShaderProgram* debugShader;
		};
	}
	struct RenderComp;
	struct MeshRenderer;
	struct OBJLoader;
	struct PostProcessor;
	struct Light;
	struct LightingSystem;

	struct BlendingStruct
	{
		RenderComp* blending_renders;
		Graphics::ShaderProgram* blending_shaders;

		BlendingStruct(RenderComp* a, Graphics::ShaderProgram* b) : blending_renders(a), blending_shaders(b) {}
		bool operator<(const BlendingStruct& that) const
		{
			return *blending_renders < *that.blending_renders;
		}
	};


	class GraphicsPipeline : public Singleton<GraphicsPipeline>
	{
		friend struct RenderComp;
		friend struct MeshRenderer;
		friend class Engine::UI::ButtonW;
		friend struct Graphics::ParticleEmitter;
		friend struct Camera;
		friend class GraphicsManager;
		friend struct Light;
		friend struct LightingSystem;
		friend class Editor::Gizmo;
		friend class Skybox;
		friend class PostProcessor;
		friend class Text3D;
		friend class Graphics::Model3D;
		friend class Graphics::FogMachine;

#pragma region // Functions
	public:
		void Initialize() noexcept;
		void Load() noexcept;
		void Unload() noexcept;
		void RenderScene() noexcept;
		void Render() noexcept;
		void AddCamera(Camera* cam);
		void DelCamera(Camera* cam);
		void SetCurrentCamera(Camera* cam, bool interpolate, float time = 0.0f, float speed = 1.0f);
		Camera* GetCurrentCamera() const;
		void AddRenderComp(Graphics::ShaderProgram* s, RenderComp* c);
		void DelRenderComp(Graphics::ShaderProgram* s, RenderComp* c);
		void AddRenderer(MeshRenderer* r);
		void DelRenderer(MeshRenderer* r);
		void AddText(Text3D* t);
		void DelText(Text3D* t);
		void AddHUD(Engine::UI::HUD* h);
		void DelHUD(Engine::UI::HUD* h);
		void AddModel(Graphics::Model3D* m);
		void DelModel(Graphics::Model3D* m);
		void AddLight(Light* r);
		void DelLight(Light* r);
		Skybox::SkyboxComp* GetSkybox();
		PostProcessor* GetPostProcessor();
		void SetSkybox(Skybox::SkyboxComp* sb);
		void DelSkybox();
		const glm::mat4 get_proj_mtx() { return mtxProj; }
		const glm::mat4 get_view_mtx() { return mtxCam; }
		const std::set<MeshRenderer*> get_renderables() { return rendComps; }

		glm::mat4 GetMtxProj() { return mtxProj * mtxCam; }

		void LoadModels();
		void DrawLine(const glm::vec3& s, const glm::vec3& e, const glm::vec4& c);

		glm::mat4 mtxCam;
		glm::mat4 mtxProj;
		EditorCam editCam;
		Graphics::FogMachine mFogMachine;
		//gfx space partitioning
		bool m_using_space_part = false;
		GraphicsPartitioning* m_gfx_space_part = nullptr;

#pragma endregion

#pragma region // Variables
		LightingSystem* lightSys;
		Graphics::ShaderList shaders;
	private:
		std::set<MeshRenderer*> rendComps;
		std::vector<Text3D*> text3DComps;
		std::vector<Engine::UI::HUD*> huds;
		std::vector<Graphics::Model3D*> model3DComps;
		Array<Camera*> allCameras;
		Camera* currentCam = nullptr;
		float time = 0.0f;
		glm::vec3 camPos;

		// Line rendering
		Graphics::DebugShape lineShape;
		std::vector<std::pair<glm::mat4, glm::vec4>> linesData;

		OBJLoader* objLoader;
		PostProcessor* mPostProcessor;
		Skybox::SkyboxComp* mSkybox = nullptr;

		std::map<Graphics::ShaderProgram*, std::vector<RenderComp*> > shaderRenders;

		Math::Interpolation::Curve lerp;
		Camera* transitionCam;

		void RenderLines();

	public:
		std::unordered_map<std::string, Graphics::ModelData*> mLoadedModels;
#pragma endregion
	};

	void ENGINE_API DrawLine(const glm::vec3& s, const glm::vec3& e, const glm::vec4& c);
}

#define GGfxPipeline (&Engine::GraphicsPipeline::Instance())

#endif
