#include "Shared.h"

using namespace glm;

namespace Engine
{
	LightingSystem::LightingSystem()
	{
		lights.reserve(maxLights);
		glGenTextures(maxLights, depthTex);
		glGenFramebuffers(maxLights, depthFBO);
		for (int i = 0; i < maxLights; ++i)
		{
			glBindTexture(GL_TEXTURE_2D, depthTex[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize,
				shadowMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

			// Create and set up the FBO
			glBindFramebuffer(GL_FRAMEBUFFER, depthFBO[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
				GL_TEXTURE_2D, depthTex[i], 0);
			GLenum depth[] = { GL_NONE };
			glDrawBuffers(1, depth);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				printf("depthFBO is not complete.\n");
		}
	}
	LightingSystem::~LightingSystem()
	{
		for (auto l : lights)
			delete l;
		lights.clear();
		glDeleteTextures(maxLights, depthTex);
		glDeleteFramebuffers(maxLights, depthFBO);
	}

	void LightingSystem::UpdateFrustums()
	{
		for (auto l : lights)
			l->UpdateFrustum();
	}

	void LightingSystem::Update()
	{

		UpdateFrustums();
		auto shadowShader = GGfxPipeline->shaders.shadowShader;
		glCullFace(GL_FRONT);
		shadowShader->Bind();
		for (int i = 0; i < numLights; i++)
			lights[i]->Update();
		shadowShader->Unbind();
		glCullFace(GL_BACK);
#ifdef _EDITOR_
		vec2 wSize = GEditor->GetViewportDimensions();
		glViewport(0, 0, wSize.x, wSize.y);
		glBindFramebuffer(GL_FRAMEBUFFER, GEditor->GetBuffer());
#else
		vec2 wSize = GWindow->GetDimensions();
		glViewport(0, 0, wSize.x, wSize.y);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
		auto shader = GGfxPipeline->shaders.simpleShader;
		const mat4& camMtx = GGfxPipeline->mtxCam;
		shader->Bind();
		shader->SetShaderUniform("numLights", &numLights);
		for (int i = 0; i < numLights; i++)
		{
			auto l = lights[i];
			vec3 pos = camMtx * vec4(l->pos, 1.0f);
			//if(type != 0)
			vec3 dir = normalize(mat3(camMtx) * (l->target - l->pos));
			//l->type = type;
			std::string text = "lights[" + std::to_string(i) + "].";
			//shader->SetUniform(text + "type", l->type);
			shader->SetShaderUniform((text + "pos").c_str(), &pos);
			shader->SetShaderUniform((text + "dir").c_str(), &dir);
			shader->SetShaderUniform((text + "att").c_str(), &l->att);
			shader->SetShaderUniform((text + "amb").c_str(), &l->amb);
			shader->SetShaderUniform((text + "dif").c_str(), &l->dif);
			shader->SetShaderUniform((text + "spe").c_str(), &l->spe);
			float cosIn = cos(radians(l->angleIn));
			float cosOut = cos(radians(l->angleOut));
			shader->SetShaderUniform((text + "cosIn").c_str(), &cosIn);
			shader->SetShaderUniform((text + "cosOut").c_str(), &cosOut);
			shader->SetShaderUniform((text + "fallOff").c_str(), &l->fallOff);
			shader->SetShaderUniform((text + "bias").c_str(), &l->bias);
			shader->SetShaderUniform((text + "neighbor").c_str(), &l->neighbor);
			shader->SetShaderUniform((text + "mtx").c_str(), &l->mtx);
			glActiveTexture(GL_TEXTURE2 + l->shadowID);
			glBindTexture(GL_TEXTURE_2D, GGfxPipeline->lightSys->depthTex[l->shadowID]);
			int value = 2 + l->shadowID;
			shader->SetShaderUniform((text + "shadowMap").c_str(), &value);
		}
	}

	Light::Light()
	{
		GGfxPipeline->AddLight(this);
	}

	Light::~Light()
	{
		GGfxPipeline->DelLight(this);
	}

	void Light::FromJson(const nlohmann::json& val)
	{
		fetch(angleIn, val, "angleIn");
		fetch(angleOut, val, "angleOut");
		fetch(fallOff, val, "fallOff");
		fetch(nearP, val, "nearP");
		fetch(farP, val, "farP");
		fetch(isStatic, val, "isStatic");

		auto j_target 	= val.find("target");
		auto j_dir	 	= val.find("dir");
		auto j_att	 	= val.find("att");
		auto j_amb	 	= val.find("amb");
		auto j_dif	 	= val.find("dif");
		auto j_spe	 	= val.find("spe");
		for (unsigned i = 0; i < 3; ++i)
		{
			if (j_target != val.end()) target[i] = j_target.value()[i].get<float>();
			if (j_dir != val.end()) dir[i] = j_dir.value()[i].get<float>();
			if (j_att != val.end()) att[i] = j_att.value()[i].get<float>();
			if (j_amb != val.end()) amb[i] = j_amb.value()[i].get<float>();
			if (j_dif != val.end()) dif[i] = j_dif.value()[i].get<float>();
			if (j_spe != val.end()) spe[i] = j_spe.value()[i].get<float>();
		}
	}

	void Light::ToJson(nlohmann::json& val) const
	{
		for (unsigned i = 0; i < 3; ++i)
		{
			val["target"][i] = target[i];
			val["dir"][i] = dir[i];
			val["att"][i] = att[i];
			val["amb"][i] = amb[i];
			val["dif"][i] = dif[i];
			val["spe"][i] = spe[i];
		}
		val["angleIn"] = angleIn;
		val["angleOut"] = angleOut;
		val["fallOff"] = fallOff;
		val["nearP"] = nearP;
		val["farP"] = farP;
		val["isStatic"] = isStatic;
	}

	void Light::OnEditor()
	{
#ifdef _EDITOR_
		ImGui::Checkbox("Debug", &debugMode);
		if (debugMode)
		{
			ImGui::Text("Meshes:");
			for (auto r : inFrustumMesh)
				ImGui::BulletText(r->GetOwner()->GetName());
			ImGui::Text("3D Models:");
			for (auto r : inFrustumModel)
				ImGui::BulletText(r->GetOwner()->GetName());
		}
		ImGui::Checkbox("Static", &isStatic);
		if (ImGui::DragFloat3("Target", &target[0]))
			firstUpdate = true;
		ImGui::DragFloat3("Attenuation", &att[0], 0.001f, 0.001f, 1.000f);
		ImGui::ColorEdit3("Ambient", &amb[0]);
		ImGui::ColorEdit3("Diffuse", &dif[0]);
		ImGui::ColorEdit3("Specular", &spe[0]);
		ImGui::DragFloat("Inner angle", &angleIn, 1.0f, 1.0f, angleOut);
		if (ImGui::DragFloat("Outter angle", &angleOut, 1.0f, 1.0f, 60.0f))
			firstUpdate = true;
		ImGui::DragFloat("fallOff", &fallOff, 0.1f, 0.0f, 10.0f);
		if (ImGui::DragFloat("nearP", &nearP, 0.01f, 0.05f, 2.0f))
			firstUpdate = true;
		if (ImGui::DragFloat("farP", &farP, 10.0f, 50.0f, 1000.0f))
			firstUpdate = true;
		if (ImGui::DragFloat("bias", &bias, 0.0000001f, 0.0000001f, 0.000001f, "%.7f"))
			firstUpdate = true;
		if (ImGui::DragInt("neighbor", &neighbor, 1, 0, 5))
			firstUpdate = true;
#endif
	}

	void Light::Update()
	{
		vec4 debugColor = vec4(1);

		if (firstUpdate)
		{
			firstUpdate = false;
			UpdateMtx();
			UpdateShadows();
		}

		// Dynamic light
		if (!isStatic && GGfxPipeline->lightSys->mUpdatePending[GetOwner()])
		{
			UpdateMtx();
			UpdateShadows();
			debugColor = vec4(1, 0, 0, 1);
		}
		// Static light
		else if (needUpdate || CheckFrustum())
		{
			UpdateShadows();
			debugColor = vec4(0, 1, 0, 1);
		}
		else
			debugColor = vec4(1);

		if (debugMode) GDebug->DebugDrawFrustum(mtx, debugColor);
	}

	void Light::UpdateShadows()
	{
		auto shadowShader = GGfxPipeline->shaders.shadowShader;
		//Change depth fbo
		auto sSize = GGfxPipeline->lightSys->shadowMapSize;
		glViewport(0, 0, sSize, sSize);
		glBindFramebuffer(GL_FRAMEBUFFER, GGfxPipeline->lightSys->depthFBO[shadowID]);
		glClear(GL_DEPTH_BUFFER_BIT);
		shadowShader->SetShaderUniform("lightMtx", &mtx);
		//Render depth
		for (auto r : inFrustumMesh)
			r->ShadowRender();
		for (auto r : inFrustumModel)
			r->ShadowRender();
	}

	void Light::UpdateFrustum()
	{
		needUpdate = false;
		Frustum f(mtx);
		for (auto r : GGfxPipeline->rendComps)
		{
			if (!r->GetMesh() || r->GetMesh()->isLight || !r->GetHasShadow() || !r->GetIsVisible() || !r->GetOwner())
				continue;
			AABB bv = r->GetBV();
			if (debugMode)
			{
				vec3 size = bv.max - bv.min;
				vec3 pos = bv.min + (size / 2.0f);
				GDebug->DebugDrawAABB(bv, vec4(1));
			}

			auto found = std::find(inFrustumMesh.begin(), inFrustumMesh.end(), r);

			if (classify_frustum_aabb_naive(f, bv) != eOUTSIDE) // inside
			{
				if (found == inFrustumMesh.end()) // add & update shadows
				{
					inFrustumMesh.push_back(r);
					needUpdate = true;
				}
			}
			else
			{
				if (found != inFrustumMesh.end()) // remove & update shadows
				{
					inFrustumMesh.erase(found);
					needUpdate = true;
				}
			}
		}

		for (auto r : GGfxPipeline->model3DComps)
		{
			if (!r->GetHasShadow() || !r->GetIsVisible())
				continue;
			AABB bv = r->GetBV();
			if (debugMode)
			{
				vec3 size = bv.max - bv.min;
				vec3 pos = bv.min + (size / 2.0f);
				GDebug->DebugDrawAABB(bv, vec4(1));
			}

			auto found = std::find(inFrustumModel.begin(), inFrustumModel.end(), r);

			if (classify_frustum_aabb_naive(f, bv) != eOUTSIDE) // inside
			{
				if (found == inFrustumModel.end()) // add & update shadows
				{
					inFrustumModel.push_back(r);
					needUpdate = true;
				}
			}
			else
			{
				if (found != inFrustumModel.end()) // remove & update shadows
				{
					inFrustumModel.erase(found);
					needUpdate = true;
				}
			}
		}
	}

	bool Light::CheckFrustum()
	{
		for (auto r : inFrustumMesh)
			if (GGfxPipeline->lightSys->mUpdatePending[r->GetOwner()])
				return true;
		for (auto r : inFrustumModel)
			if (GGfxPipeline->lightSys->mUpdatePending[r->GetOwner()])
				return true;
		return false;
	}

	void Light::UpdateMtx()
	{
		pos = GetOwner()->GetPosition();
		vec3 view = normalize(target - pos);
		bool goodDir = view != vec3(0, 1, 0) && view != vec3(0, -1, 0);
		vec3 right = goodDir ? cross(view, vec3(0, 1, 0)) : vec3(1, 0, 0);
		vec3 up = cross(right, view);

		mat4 proj = perspective(radians(angleOut * 2.0f), 1.0f, nearP, farP);
		mat4 cam = lookAt(pos, target, up);

		mtx = proj	* cam;
	}
}
