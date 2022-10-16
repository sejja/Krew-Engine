#pragma once


struct aiNode;
struct aiMesh;
struct aiScene;
namespace Engine
{
	namespace Graphics
	{
		struct Vertex3D
		{
			Vertex3D() {}
			Vertex3D(glm::vec3 pos, glm::vec3 n, glm::vec2 uv);

			glm::vec3 mPosition = glm::vec3(0.0f);
			glm::vec2 mUV = glm::vec2(0.0f);

			glm::vec3 mNormal = glm::vec3(0.0f);
			glm::vec3 mTangent = glm::vec3(0.0f);
			glm::vec3 mBitangent = glm::vec3(0.0f);

			glm::vec3 mAvgNormal = glm::vec3(0.0f);
			glm::vec3 mAvgTangent = glm::vec3(0.0f);
			glm::vec3 mAvgBitangent = glm::vec3(0.0f);
		};

		struct Material3D
		{
			glm::vec3 mAmbient = glm::vec3(0.0f);
			glm::vec3 mDiffuse = glm::vec3(1.0f);
			glm::vec3 mSpecular = glm::vec3(0.5f);
			float mShine = 15.0f;
			float dashImageVar = 1.f;
		};

		struct Mesh3D
		{
		public:
			size_t mIndicesize;
			size_t mVertexsize;

			Mesh3D(const std::vector<Vertex3D>& vertices, const std::vector<unsigned int>& indices);

			void Render(Graphics::ShaderProgram& program);
			void Render(Graphics::ShaderProgram& program, Material3D& customMaterial);
			void ShadowRender();

			glm::vec3 bvMin;
			glm::vec3 bvMax;
			Material3D mMaterial;

		private:
			//  Buffers
			unsigned int mVAO = 0, mVBO = 0, mIBO = 0;
			void LoadMesh(const std::vector<Vertex3D>& vertices, const std::vector<unsigned int>& indices);
		};

		struct ENGINE_API ModelData {
			ModelData(const std::string& modelpath);
			~ModelData();


			void LoadModel();
			void ProcessAssimpNode(aiNode* node, const aiScene* scene);
			virtual Mesh3D* ProcessMesh(aiMesh* mesh, const aiScene* scene);
			void ProcessMeshVtxAndIdx(aiMesh* mesh, std::vector<Vertex3D>& MeshVtx, std::vector<unsigned int>& MeshIdx);

			std::vector<Mesh3D*> mMeshes;
			glm::vec3 mOffset;
			float mFactor = 0;
			AABB bv;


			std::string mModelPath;
		};

		class ENGINE_API Model3D : public RenderComp
		{
			friend class GraphicsPipeline;

		public:

			Model3D();
			Model3D(unsigned custom_shader_id);
			void BeginPlay() override;
			virtual ~Model3D();

			void FromJson(const nlohmann::json& val)override;
			void ToJson(nlohmann::json& val) const override;

			void Render();
			void ShadowRender();
			void SetShader(Graphics::ShaderProgram* p);
			void SetVisible(const bool b);
			void SetTexture(const std::string str);


			//void SetNormalMap(AssetReference<Graphics::Texture> t);
			//void SetNormalMapByName(const std::string& name);
			void OnEditor() override;

			bool GetIsVisible()const;
			bool GetHasShadow()const;

			ModelData* mModelData; // loaded data from file
			std::string mModelName;

			bool customMaterial = false;
			bool initialMaterialSet = false;
			Material3D mMaterial;
		protected:

			bool mVisible = true;
			bool mSmooth = false;
			ShaderProgram* mProgram = nullptr;

			std::string mDiffusePath = __Texts::Engine::RenderComp::StaticText::DefaultTexture;
			std::string mNormalPath;
			//Textures (diffuse is given by default)
			Asset<Graphics::Texture> mDiffTexture = nullptr;
			Asset<Graphics::Texture> mNormalTexture = nullptr;
		};

	} //namespace Graphics


} //namespace Engine
