#include <Shared.h>

using namespace glm;

namespace Engine
{
	namespace Graphics
	{
		Vertex3D::Vertex3D(glm::vec3 pos, glm::vec3 n, glm::vec2 uv)
		{
			mPosition = pos;
			mNormal = n;
			mUV = uv;
		}

		Mesh3D::Mesh3D(const std::vector<Vertex3D>& vertices, const std::vector<unsigned int>& indices)
		{
			mIndicesize = indices.size();
			mVertexsize = vertices.size();

			bvMin = vec3(INFINITY);
			bvMax = vec3(-INFINITY);
			for (unsigned i = 0; i < 3; ++i)
				for (auto& v : vertices)
				{
					if (bvMin[i] > v.mPosition[i])
						bvMin[i] = v.mPosition[i];
					if (bvMax[i] < v.mPosition[i])
						bvMax[i] = v.mPosition[i];
				}

			LoadMesh(vertices, indices);
		}

		void Mesh3D::Render(Graphics::ShaderProgram& program)
		{
			program.SetShaderUniform("bAvg", false);
			program.SetShaderUniform("mat.amb", &mMaterial.mAmbient);
			program.SetShaderUniform("mat.dif", &mMaterial.mDiffuse);
			program.SetShaderUniform("mat.spe", &mMaterial.mSpecular);
			program.SetShaderUniform("mat.shiny", &mMaterial.mShine);
			program.SetShaderUniform("mat.dashImageVar", &mMaterial.dashImageVar);
			glBindVertexArray(mVAO);
			glDrawElements(GL_TRIANGLES, mIndicesize, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		void Mesh3D::Render(Graphics::ShaderProgram& program, Material3D& customMaterial)
		{
			program.SetShaderUniform("bAvg", false);
			program.SetShaderUniform("mat.amb", &customMaterial.mAmbient);
			program.SetShaderUniform("mat.dif", &customMaterial.mDiffuse);
			program.SetShaderUniform("mat.spe", &customMaterial.mSpecular);
			program.SetShaderUniform("mat.shiny", &customMaterial.mShine);
			program.SetShaderUniform("mat.dashImageVar", &customMaterial.dashImageVar);
			glBindVertexArray(mVAO);
			glDrawElements(GL_TRIANGLES, mIndicesize, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		void Mesh3D::ShadowRender()
		{
			glBindVertexArray(mVAO);
			glDrawElements(GL_TRIANGLES, mIndicesize, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		void Mesh3D::LoadMesh(const std::vector<Vertex3D>& vertices, const std::vector<unsigned int>& indices)
		{
			//Generate buffers
			glGenVertexArrays(1, &mVAO);
			glGenBuffers(1, &mVBO);
			glGenBuffers(1, &mIBO);

			//Bindings
			glBindVertexArray(mVAO);
			glBindBuffer(GL_ARRAY_BUFFER, mVBO);

			glBufferData(GL_ARRAY_BUFFER, mVertexsize * sizeof(Vertex3D), vertices.data() , GL_STATIC_DRAW);

			//We are loading indices as well
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndicesize * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

			// Positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)0);

			// UVs
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, mUV));
			
			// Normals
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, mNormal));

			// Tangents
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, mTangent));

			// Bitangents
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)offsetof(Vertex3D, mBitangent));

			glBindVertexArray(0);	
		}

		Model3D::Model3D()
		{
			mBlendingType = BlendingType::eOpaque;
			SetShader(GGfxPipeline->shaders.simpleShader);

			GGfxPipeline->AddModel(this);

			mModelData = nullptr;
		}

		Model3D::Model3D(unsigned custom_shader_id)
		{
			SetShader(GGfxPipeline->shaders.customs[custom_shader_id]);

			GGfxPipeline->AddModel(this);
		}

		void Model3D::BeginPlay() {
			initialMaterialSet = false;
		}

		Model3D::~Model3D()
		{
			GGfxPipeline->DelModel(this);
		}
		void Model3D::FromJson(const nlohmann::json& val)
		{
			RenderComp::FromJson(val);
			auto it = val.find("mSamplingType");

			if (it != val.end())
				mSamplingType = (*it).get<SamplingType>();
			else
				mSamplingType = SamplingType::eRepeat;

			it = val.find("mBlendingType");

			if (it != val.end())
				mBlendingType = (*it).get<BlendingType>();
			else
				mBlendingType = BlendingType::eOpaque;
			

			fetch(mVisible, val, "Visible");
			fetch(mModelName, val, "ModelName");
			fetch(mDiffusePath, val, "DiffusePath");

			auto modelIT = GGfxPipeline->mLoadedModels.find(mModelName);
			if (modelIT != GGfxPipeline->mLoadedModels.end()) {
				mModelData = modelIT->second;
				fetch(mModelData->mModelPath, val, "ModelPath");
			}
			
			auto found = val.find("NormalPath");
			if (found != val.end())
			{
				mNormalPath = found.value().get<std::string>();
				//SetNormalMapByName(mNormalPath);
			}
		}
		void Model3D::ToJson(nlohmann::json& val) const
		{
			RenderComp::ToJson(val);
			val["mBlendingType"] = mBlendingType;
			val["mSamplingType"] = mSamplingType;
			val["Visible"] = mVisible;
			if (!mModelName.empty())
				val["ModelName"] = mModelName;
			if(mModelData && !(mModelData)->mModelPath.empty())
				val["ModelPath"] = (mModelData)->mModelPath;
			if (!mDiffusePath.empty())
				val["DiffusePath"] = mDiffusePath;
			if (!mNormalPath.empty())
				val["NormalPath"] = mNormalPath;
		}
		void Model3D::Render()
		{
			if (drawBV)
				GDebug->DebugDrawAABB(GetBV(), { 1, 0, 0, 1 });

			if (mVisible)
			{
				glEnable(GL_DEPTH_TEST);
				switch (mBlendingType)
				{
				case BlendingType::eOpaque:
					glDisable(GL_BLEND);
					break;
				case BlendingType::eAdditive:
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE);
					break;
				case BlendingType::eModular:
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					break;
				}

				switch (mSamplingType)
				{
				case SamplingType::eRepeat:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					break;
				case SamplingType::eMirroredRepeat:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
					break;
				case SamplingType::eClampToEdge:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					break;

				default:
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

					float borderColor[] = {0.0f, 0.0f, 0.0f, 1.0f };
					glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
				}

				if (mModelData == nullptr) return;

				glm::mat4 Model = GetOwner()->GetWorldMatrix() * translate(mat4(1), (mModelData)->mOffset) * scale(mat4(1), vec3((mModelData)->mFactor));

				//Transformation matrices
				mProgram->SetShaderUniform("M", &Model);
				if (hasBloom)
					mProgram->SetShaderUniform("bloomIntensity", &bloomIntensity);
				else
				{
					float zero = 0;
					mProgram->SetShaderUniform("bloomIntensity", &zero);
				}

#ifdef _EDITOR_
				if(GEditor->RenderWireframe())
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

				if (!mDiffTexture)
					mDiffTexture = GContent->GetResource<Graphics::Texture>(mDiffusePath.c_str());

				Graphics::Texture* texture = mDiffTexture->Get();

				if (texture)
				{
					glActiveTexture(GL_TEXTURE31);
					texture->Bind();
					int texUnit = 31;
					mProgram->SetShaderUniform("colorsTex", &texUnit);
				}

				//Simply call draw to all meshes on scene
				if (customMaterial) {
					if (!initialMaterialSet) {
						initialMaterialSet = true;
						mMaterial = mModelData->mMeshes[0]->mMaterial;
					}

					mModelData->mMeshes[0]->Render(*mProgram, mMaterial);
				}
				else {
					for (auto it : (mModelData)->mMeshes)
						it->Render(*mProgram);
				}
			}
		}
		void Model3D::ShadowRender()
		{
			if (mModelData == nullptr)
				return;
			glm::mat4 Model = GetOwner()->GetWorldMatrix() * translate(mat4(1), (mModelData)->mOffset) * scale(mat4(1), vec3((mModelData)->mFactor));
			mProgram->SetShaderUniform("M", &Model);
			for (auto it : (mModelData)->mMeshes)
				it->ShadowRender();
		}

		void Model3D::SetShader(Graphics::ShaderProgram* p)
		{
			mProgram = p;
		}


		bool Model3D::GetIsVisible() const
		{
			return mVisible;
		}

		bool Model3D::GetHasShadow() const
		{
			return hasShadow;
		}

		void Model3D::SetVisible(const bool b)
		{
			mVisible = b;
		}

		//void Model3D::SetNormalMap(AssetReference<Graphics::Texture> t)
		//{
		//	mNormalTexture = t.lock();
		//	hasNormalMap = true;
		//}
		//
		//void Model3D::SetNormalMapByName(const std::string& name)
		//{
		//	if (name.empty())
		//	{
		//		mNormalPath = "";
		//		hasNormalMap = false;
		//		return;
		//	}
		//	mNormalPath = name;
		//
		//	auto t = GContent->GetResource<Graphics::Texture>(mNormalPath.c_str());
		//
		//	if (t)
		//		SetNormalMap(t);
		//}
		void Model3D::SetTexture(const std::string str)
		{
			mDiffusePath = str;
			mDiffTexture = GContent->GetResource<Graphics::Texture>(mDiffusePath.c_str());
		}

		void Model3D::OnEditor()
		{
#ifdef _EDITOR_
			//Toggle visibility
			ImGui::Checkbox("Visible", &mVisible);
			ImGui::Checkbox("Draw BV", &drawBV);
			ImGui::Checkbox("Has Shadow", &hasShadow);
			ImGui::Checkbox("Has Bloom", &hasBloom);
			if (hasBloom)
				ImGui::DragFloat("Bloom Intensity", &bloomIntensity, 0.01f, 0.0f, 10.0f);

			//Toggle smooth lighting (average normals)
			ImGui::Checkbox("Smooth lighting", &mSmooth);

			const char* customNames[3] = { "Opaque", "Additive", "Modular" };
			// Choosing blending type
			int t = (int)mBlendingType;
			ImGui::Combo("Blending Type", &t, customNames, 3);
			mBlendingType = (BlendingType)(t);

			const char* customNamesSamping[] = { TEXT("Repeat"), TEXT("Mirrored Repeat"), TEXT("Clamp To Edge"),
				TEXT("Clamp To Border")};
			// Choosing blending type
			t = (int)mSamplingType;
			ImGui::Combo("Sampling Type", &t, customNamesSamping, 4);
			mSamplingType = (SamplingType)(t);

			std::string previewVal = mModelData == nullptr ? "none" : mModelData->mModelPath;
			if (ImGui::BeginCombo("Model", previewVal.c_str())) {
				for (auto& it : GGfxPipeline->mLoadedModels)
					if (ImGui::Selectable(it.first.c_str())) {
						mModelName = it.first;
						mModelData = it.second;
					}
				ImGui::EndCombo();
			}

			if (mModelData == nullptr) return;

			
			if (ImGui::InputText("Diffuse", &mDiffusePath))
				mDiffTexture = GContent->GetResource<Graphics::Texture>(mDiffusePath.c_str());
			
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_TEXTURE_FILE"))
				{
					if (payload->DataSize == sizeof(std::string))
					{
						std::string payload_n = *(std::string*)payload->Data;
						mDiffusePath = payload_n;
						mDiffTexture = GContent->GetResource<Graphics::Texture>(mDiffusePath.c_str());
					}
				}
				ImGui::EndDragDropTarget();
			}
#endif
		}

		//model data
		ModelData::ModelData(const std::string& modelpath) : mModelPath(modelpath) {
			LoadModel();
		}

		ModelData::~ModelData() {
			unsigned Count = mMeshes.size();

			//Free memory used by meshes
			for (unsigned i = 0; i < Count; i++)
				delete mMeshes[i];

			mMeshes.clear();
		}

		void ModelData::LoadModel()
		{
			//Call an importer to load a scene from a file
			Assimp::Importer Imp;
			const aiScene* Scene = Imp.ReadFile(mModelPath,
				aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

			//Check if it loaded correctly
			if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
			{
				std::cout << "Error while loading Assimp model:" << Imp.GetErrorString() << std::endl;
				return;
			}

			//Check if it loaded correctly
			if (!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
			{
				GConsole->Log(Console::eMessageType::e_Error, "Error while loading Assimp model: %s", Imp.GetErrorString());
				return;
			}
			//Process scene
			ProcessAssimpNode(Scene->mRootNode, Scene);
			////Load texture
			//mDiffTexture->LoadFromFile(mDiffusePath.c_str());

			bv.min = vec3(INFINITY);
			bv.max = vec3(-INFINITY);
			for (auto m : mMeshes)
			{
				bv.min = glm::min(bv.min, m->bvMin);
				bv.max = glm::max(bv.max, m->bvMax);
			}

			vec3 size = bv.max - bv.min;
			float biggestAxis =
				(size.x > size.y)
				? (size.x > size.z
					? size.x
					: size.z)
				: (size.y > size.z
					? size.y
					: size.z);
			mFactor = 1.0f / biggestAxis;
			bv.min *= mFactor;
			bv.max *= mFactor;

			mOffset = -(bv.max + bv.min) / 2.0f;
			bv.min += mOffset;
			bv.max += mOffset;
		}

		void ModelData::ProcessAssimpNode(aiNode* node, const aiScene* scene)
		{
			// Process all the node's meshes (if any)
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
				mMeshes.push_back(ProcessMesh(mesh, scene));

				aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

				auto lastmesh = mMeshes.back();
				aiColor4D diffuse, ambient, specular;
				float shininess;
				if (aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE,
					&diffuse) == AI_SUCCESS) {
					lastmesh->mMaterial.mDiffuse = { diffuse.r, diffuse.g, diffuse.b };
				}

				if (aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR,
					&specular) == AI_SUCCESS) {
					lastmesh->mMaterial.mAmbient = { specular.r, specular.g, specular.b };
				}

				if (aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT,
					&ambient) == AI_SUCCESS) {
					lastmesh->mMaterial.mAmbient = { ambient.r, ambient.g, ambient.b };
				}

				if (aiGetMaterialFloat(material, AI_MATKEY_SHININESS,
					&shininess) == AI_SUCCESS) {
					lastmesh->mMaterial.mShine = shininess;
				}
			}
			// Do the same for this node's children
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				ProcessAssimpNode(node->mChildren[i], scene);
			}
		}

		Mesh3D* ModelData::ProcessMesh(aiMesh* mesh, const aiScene* scene)
		{
			std::vector<Vertex3D> MeshVtx;
			std::vector<unsigned int> MeshIdx;
			ProcessMeshVtxAndIdx(mesh, MeshVtx, MeshIdx);

			//Build new mesh
			Mesh3D* M = new Mesh3D(MeshVtx, MeshIdx);

			return M;
		}

		void ModelData::ProcessMeshVtxAndIdx(aiMesh* mesh, std::vector<Vertex3D>& MeshVtx, std::vector<unsigned int>& MeshIdx)
		{
			//Check vertices' internal data
			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				//Position
				glm::vec3 Pos;
				Pos.x = mesh->mVertices[i].x;
				Pos.y = mesh->mVertices[i].y;
				Pos.z = mesh->mVertices[i].z;

				//Normal
				glm::vec3 Normal;
				Normal.x = mesh->mNormals[i].x;
				Normal.y = mesh->mNormals[i].y;
				Normal.z = mesh->mNormals[i].z;

				//Tangent
				glm::vec3 Tangent;
				Tangent.x = mesh->mTangents[i].x;
				Tangent.y = mesh->mTangents[i].y;
				Tangent.z = mesh->mTangents[i].z;

				//Bitangent
				glm::vec3 Bitangent;
				Bitangent.x = mesh->mBitangents[i].x;
				Bitangent.y = mesh->mBitangents[i].y;
				Bitangent.z = mesh->mBitangents[i].z;

				//UVs (might not contain, so check first)
				glm::vec2 UV;
				if (mesh->mTextureCoords[0])
				{
					UV.x = mesh->mTextureCoords[0][i].x;
					UV.y = mesh->mTextureCoords[0][i].y;
				}
				else
					UV = glm::vec2(0.0f, 0.0f);

				Vertex3D Vtx(Pos, Normal, UV);
				Vtx.mUV = UV;
				Vtx.mPosition = Pos;//REDUNDANT
				Vtx.mNormal = Normal;//REDUNDANT
				Vtx.mTangent = Tangent;//REDUNDANT
				Vtx.mBitangent = Bitangent;//REDUNDANT

				MeshVtx.push_back(Vtx);
			}
			// Process indices
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for (unsigned int j = 0; j < face.mNumIndices; j++)
					MeshIdx.push_back(face.mIndices[j]);
			}
		}

	} //namespace Graphics

} //namespace Engine
