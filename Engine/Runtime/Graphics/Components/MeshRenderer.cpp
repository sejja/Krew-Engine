#include "Shared.h"

using namespace glm;

namespace Engine
{
	MeshRenderer::MeshRenderer(bool hud)
	{
		mBlendingType = BlendingType::eOpaque;
		isHUD = hud;
		if (!isHUD)
		{
			shader = GGfxPipeline->shaders.simpleShader;
			GGfxPipeline->AddRenderer(this);
		}
		else
			shader = GGfxPipeline->shaders.HUDShader;

		texture = nullptr;

		//space partitioning id
		if (GGfxPipeline->m_gfx_space_part)
			m_ID = ++GGfxPipeline->m_gfx_space_part->idGenerator;
		else
			m_ID = -1;
	}

	MeshRenderer::~MeshRenderer()
	{
		if (!isHUD)
			GGfxPipeline->DelRenderer(this);
	}

	void MeshRenderer::FromJson(const nlohmann::json& val)
	{
		RenderComp::FromJson(val);
		auto it = val.find("mBlendingType");

		if (it != val.end())
			mBlendingType = (*it).get<BlendingType>();
		else
			mBlendingType = BlendingType::eOpaque;

		it = val.find("mSamplingType");

		if (it != val.end())
			mSamplingType = (*it).get<SamplingType>();
		else
			mSamplingType = SamplingType::eRepeat;

		fetch(isVisible, val, "isVisible");
		fetch(useAvg, val, "useAvg");

		// The following variables can't use the fetch function since they call another function,
		// instead of storing the value of the json in the T value of the fetch function
		it = val.find("meshIdx");
		if (it != val.end()) SetMeshByIdx((*it).get<int>());
		
		it = val.find("textureName");
		if (it != val.end()) SetTextureByName((*it).get<std::string>());
		it = val.find("shaderIdx");
		if (it != val.end()) SetShaderByIdx((*it).get<int>());

		auto it1 = val.find("mat.amb");
		auto it2 = val.find("mat.dif");
		auto it3 = val.find("mat.spe");
		for (unsigned i = 0; i < 3; ++i)
		{
			if (it1 != val.end()) mat.amb[i] = val["mat.amb"][i].get<float>();
			if (it2 != val.end()) mat.dif[i] = val["mat.dif"][i].get<float>();
			if (it3 != val.end()) mat.spe[i] = val["mat.spe"][i].get<float>();
		}

		fetch(mat.shiny, val, "mat.shiny");
	}

	void MeshRenderer::ToJson(nlohmann::json& val) const
	{
		RenderComp::ToJson(val);
		val["mBlendingType"] = mBlendingType;
		val["mSamplingType"] = mSamplingType;
		val["isVisible"] = isVisible;
		val["useAvg"] = useAvg;
		val["meshIdx"] = meshIdx;
		if(!textureName.empty())
			val["textureName"] = textureName;
		val["shaderIdx"] = shaderIdx;
		for (unsigned i = 0; i < 3; ++i)
		{
			val["mat.amb"][i] = mat.amb[i];
			val["mat.dif"][i] = mat.dif[i];
			val["mat.spe"][i] = mat.spe[i];
		}
		val["mat.shiny"] = mat.shiny;
	}

	void MeshRenderer::OnEditor()
	{
#ifdef _EDITOR_
		ImGui::Checkbox("Visile", &isVisible);
		ImGui::Checkbox("Wireframe", &mWireframe);
		ImGui::Checkbox("Use average", (bool*)&useAvg);
		ImGui::Checkbox("Debug", &debugMode);
		ImGui::Checkbox("BV", &drawBV);
		ImGui::Checkbox("Has Shadow", &hasShadow);
		ImGui::Checkbox("Has Bloom", &hasBloom);
		if (hasBloom)
			ImGui::DragFloat("Bloom Intensity", &bloomIntensity, 0.01f, 0.0f, 10.0f);

		if (ImGui::TreeNode("Material"))
		{
			ImGui::ColorPicker3("Ambient", &mat.amb[0]);
			ImGui::ColorPicker3("Diffuse", &mat.dif[0]);
			ImGui::ColorPicker3("Specular", &mat.spe[0]);
			ImGui::ColorPicker4("Shiny", &mat.shiny);
			ImGui::TreePop();
		}

		const char* customNames[3] = { "Opaque", "Additive", "Modular" };
		// Choosing blending type
		int t = (int)mBlendingType;
		ImGui::Combo("Blending Type", &t, customNames, 3);
		mBlendingType = (BlendingType)(t);

		const char* customNamesSamping[] = { TEXT("Repeat"), TEXT("Mirrored Repeat"), TEXT("Clamp To Edge"),
				TEXT("Clamp To Border") };
		// Choosing blending type
		t = (int)mSamplingType;
		ImGui::Combo("Sampling Type", &t, customNamesSamping, 4);
		mSamplingType = (SamplingType)(t);

		if (ImGui::InputText("Texture", &textureName, ImGuiInputTextFlags_EnterReturnsTrue))
			SetTextureByName(textureName);

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_TEXTURE_FILE"))
			{
				if (payload->DataSize == sizeof(std::string)) {
					std::string payload_n = *(std::string*)payload->Data;
					textureName = payload_n;
					SetTextureByName(textureName);
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::Combo("Shader", &shaderIdx, GGfxPipeline->shaders.customNames, GGfxPipeline->shaders.customSize))
			SetShaderByIdx(shaderIdx);

		if (ImGui::Combo("Mesh type", &meshIdx, meshTypes.data(), meshTypes.size()))
			SetMeshByIdx(meshIdx);
#endif
	}

	void MeshRenderer::SetTexture(AssetReference<Graphics::Texture> t)
	{
		texture = t.lock();
	}

	void MeshRenderer::SetTextureByName(const std::string& name)
	{
		if (name.empty())
		{
			SetTexture(GContent->GetResource<Graphics::Texture>(__Texts::Engine::RenderComp::StaticText::DefaultTexture));
			return;
		}

		textureName = name;

		auto t = GContent->GetResource<Graphics::Texture>(textureName.c_str());

		if (t)
			SetTexture(t);
	}

	Mesh* MeshRenderer::GetMesh()const
	{
		return mesh;
	}

	void MeshRenderer::SetMesh(Mesh* m)
	{
		mesh = m;
		bv.min = m->bvMin;
		bv.max = m->bvMax;
	}

	void MeshRenderer::SetMeshByName(const std::string& name)
	{
		for (int i = 0; i < meshTypes.size(); ++i)
		{
			if (strcmp(meshTypes[i],name.c_str()) == 0)
			{
				meshIdx = i;
				SetMesh(GGfxPipeline->objLoader->meshes[name]);
			}
		}
	}

	void MeshRenderer::SetMeshByIdx(int i)
	{
		if (-1 < i && i < meshTypes.size())
		{
			meshIdx = i;
			SetMesh(GGfxPipeline->objLoader->meshes[meshTypes[i]]);
		}
	}

	void MeshRenderer::SetIsVisible(bool b)
	{
		isVisible = b;
	}

	bool MeshRenderer::GetIsVisible()const
	{
		return isVisible;
	}

	void MeshRenderer::SetHasShadow(bool b)
	{
		hasShadow = b;
	}

	bool MeshRenderer::GetHasShadow()const
	{
		return hasShadow;
	}

	void MeshRenderer::SetShader(Graphics::ShaderProgram* s)
	{
		shader = s;
	}

	void MeshRenderer::SetShaderByIdx(int i)
	{
		if (-1 < i && i < GGfxPipeline->shaders.customSize)
		{
			shaderIdx = i;
			shader = GGfxPipeline->shaders.customs[i];
		}
	}

	void MeshRenderer::Render()
	{
		if (!m_render || !isVisible || !mesh || !shader || !GetOwner())
			return;

		glEnable(GL_DEPTH_TEST);
		switch (mBlendingType)
		{
		case BlendingType::eOpaque:
			glDisable(GL_BLEND);
			break;
		case BlendingType::eAdditive: 
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		}
		case BlendingType::eModular:
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		}
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

			float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		}

		glm::mat4 mtxModel = GetOwner()->GetWorldMatrix();

		shader->SetShaderUniform("isLight", &mesh->isLight);
		if(hasBloom)
			shader->SetShaderUniform("bloomIntensity", &bloomIntensity);
		else
		{
			float zero = 0;
			shader->SetShaderUniform("bloomIntensity", &zero);
		}
		shader->SetShaderUniform("bAvg", &useAvg);
		shader->SetShaderUniform("mat.amb", &mat.amb);
		shader->SetShaderUniform("mat.dif", &mat.dif);
		shader->SetShaderUniform("mat.spe", &mat.spe);
		shader->SetShaderUniform("mat.shiny", &mat.shiny);
		shader->SetShaderUniform("mat.dashImageVar", &mat.dashImageVar);
		shader->SetShaderUniform("M", &mtxModel);
		shader->SetShaderUniform("V", &GGfxPipeline->mtxCam);
		shader->SetShaderUniform("P", &GGfxPipeline->mtxProj);

		if (!texture)
			texture = GContent->GetResource<Graphics::Texture>(textureName.c_str());

		Assets::TResource<Graphics::Texture>* tex = texture.get();

		if (tex && tex->Get())
		{
			glActiveTexture(GL_TEXTURE31);
			tex->Get()->Bind();
			int texUnit = 31;
			shader->SetShaderUniform("colorsTex", &texUnit);
		}

#ifndef _EDITOR_
		if (mWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#else
		if (mWireframe || GEditor->RenderWireframe())
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

		mesh->Render();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (drawBV)
			GDebug->DebugDrawAABB(GetBV(), { 1, 1, 0, 1 });

		if(debugMode)
		{
			GGfxPipeline->shaders.lineShader->Bind();
			GGfxPipeline->shaders.lineShader->SetShaderUniform("M", &mtxModel);
			GGfxPipeline->shaders.lineShader->SetShaderUniform("V", &GGfxPipeline->mtxCam);
			GGfxPipeline->shaders.lineShader->SetShaderUniform("P", &GGfxPipeline->mtxProj);
			if (useAvg)
			{
				glBindVertexArray(mesh->vaoAvg);
				glDrawArrays(GL_LINES, 0, (unsigned)mesh->avgLines.size());
			}
			else
			{
				glBindVertexArray(mesh->vaoFace);
				glDrawArrays(GL_LINES, 0, (unsigned)mesh->faceLines.size());
			}
			glBindVertexArray(0);
		}
	}
	void MeshRenderer::RenderHUDs(glm::mat4 world)
	{
		if (!isVisible || !mesh || !shader)
			return;

		shader->Bind();
		switch (mBlendingType)
		{
		case BlendingType::eOpaque:
			glDisable(GL_BLEND);
			break;
		case BlendingType::eAdditive:
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		}
		case BlendingType::eModular:
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		}
		}

		shader->SetShaderUniform("M", &world);
		glm::vec2 size = GWindow->GetDimensions() / 2.f;
		glm::mat4 ortho = glm::ortho(-size.x, size.x, -size.y, size.y);
		shader->SetShaderUniform("P", &ortho);

		if (!texture)
			texture = GContent->GetResource<Graphics::Texture>(textureName.c_str());

		Graphics::Texture* tex = texture->Get();

		if (tex)
		{
			glActiveTexture(GL_TEXTURE31);
			tex->Bind();
			int texUnit = 31;
			shader->SetShaderUniform("colorsTex", &texUnit);
		}

#ifndef _EDITOR_
		if (mWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#else
		if (mWireframe || GEditor->RenderWireframe())
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

		mesh->Render();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glUseProgram(0);
	}
	void MeshRenderer::ShadowRender()
	{
		if (!isVisible || !mesh || !shader || mesh->isLight)
			return;

		glm::mat4 mtxModel = GetOwner()->GetWorldMatrix();
		GGfxPipeline->shaders.shadowShader->Bind();
		GGfxPipeline->shaders.shadowShader->SetShaderUniform("M", &mtxModel);
		// draw mesh
		glBindVertexArray(mesh->vaoMesh);
		glDrawArrays(GL_TRIANGLES, 0, (unsigned)mesh->vtx.size());

		// unbind
		glBindVertexArray(0);

	}
}