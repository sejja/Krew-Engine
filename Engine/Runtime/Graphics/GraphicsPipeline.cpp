//
//	GraphicsPipeline.cpp
//	Good Neighbours
//
//	Created by Fernando Uribe on 09/17/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#include "Shared.h"

using namespace glm;

namespace Engine {

	using namespace Graphics;

	void GraphicsPipeline::Initialize() noexcept
	{
		glClearColor(0, 0, 0, 1);
		glClearDepth(1.0f);
		glFrontFace(GL_CCW);
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}

	void GraphicsPipeline::Load() noexcept {
		objLoader = new OBJLoader;
		lightSys = new LightingSystem;

		lineShape.InitSegment();
		lineShape.InitializeBuffers();

		shaders.simpleShader = GContent->GetEngineResource<ShaderProgram>("Shaders\\shaderSimple.program")->Get();
		shaders.shaderParticle = GContent->GetEngineResource<ShaderProgram>("Shaders\\shaderParticle.program")->Get();
		shaders.TextShader = GContent->GetEngineResource<ShaderProgram>("Shaders\\TextShader.program")->Get();
		shaders.SkyboxShader = GContent->GetEngineResource<ShaderProgram>("Shaders\\SkyboxShader.program")->Get();
		shaders.HUDShader = GContent->GetEngineResource<ShaderProgram>("Shaders\\shaderHUD.program")->Get();
		shaders.lineShader = GContent->GetEngineResource<ShaderProgram>("Shaders\\shaderLine.program")->Get();
		shaders.debugShader = GContent->GetEngineResource<ShaderProgram>("Shaders\\debugShader.program")->Get();
		shaders.shadowShader = GContent->GetEngineResource<ShaderProgram>("Shaders\\shaderShadow.program")->Get();
		shaders.PostProcessShader = GContent->GetEngineResource<ShaderProgram>("Shaders\\shaderPostProcess.program")->Get();
#ifdef _EDITOR_
		auto dimensions = GEditor->GetViewportDimensions();//GWindow->GetDimensions();
#else
		auto dimensions = GWindow->GetDimensions();
#endif
		mPostProcessor = new PostProcessor(shaders.PostProcessShader, dimensions.x, dimensions.y);
		shaders.shadowRenderShader = GContent->GetEngineResource<ShaderProgram>("Shaders\\ShadowRender.program")->Get();

		mtxCam = editCam.GetCam()->GetViewMtx();
		mtxProj = editCam.GetCam()->GetProjMtx();

		//load models
		LoadModels();

		//space partitioning
		if (m_using_space_part) {
			m_gfx_space_part = new GraphicsPartitioning;
			m_gfx_space_part->Init();
		}
	}

	void GraphicsPipeline::Unload() noexcept
	{
		if (objLoader)
			delete objLoader;
		if (mPostProcessor)
			delete mPostProcessor;

		if (m_using_space_part) {
			m_gfx_space_part->ShutDown();
			delete m_gfx_space_part;
		}

		//unload models
		for (auto& it : mLoadedModels)
			delete it.second;
	}

	void GraphicsPipeline::RenderScene() noexcept
	{
		mPostProcessor->BeginRender();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_BLEND);

		if (mSkybox && mSkybox->IsVisible())
			mSkybox->RenderSkybox();

		std::set<BlendingStruct> blending;

		RenderLines();

		mFogMachine.ComputeFog();

		for (auto& it : shaderRenders)
		{
			it.first->Bind();
			it.first->SetShaderUniform("threshold", &mPostProcessor->threshold);
			it.first->SetShaderUniform("V", &mtxCam);
			it.first->SetShaderUniform("P", &mtxProj);
			for (auto r : it.second)
			{
				if (r->GetBlendingType() == BlendingType::eOpaque)
					r->Render();
				else
					blending.insert(BlendingStruct(r, it.first));
			}
			glUseProgram(0);
		}

		glEnable(GL_BLEND);
		ShaderProgram* temp = nullptr;
		for (const auto& it : blending)
		{
			if (it.blending_shaders != temp)
			{
				it.blending_shaders->Bind();
				it.blending_shaders->SetShaderUniform("threshold", &mPostProcessor->threshold);
				it.blending_shaders->SetShaderUniform("V", &mtxCam);
				it.blending_shaders->SetShaderUniform("P", &mtxProj);
				temp = it.blending_shaders;
			}
			it.blending_renders->Render();
		}
		glUseProgram(0);

		DebugRenderer::Instance().Render(mtxCam, mtxProj);

		mPostProcessor->EndRender();

		shaders.HUDShader->Bind();
		for (auto it : huds)
			it->Render();
		glUseProgram(0);
	}

	void GraphicsPipeline::Render() noexcept
	{
		//camPos = glm::rotate(glm::mat4(1), aDegree, glm::vec3(0, 1, 0)) * glm::vec4(camPos.x, 10 * glm::sin(time * 2.0f), camPos.z, 1);
		//mtxCam = glm::lookAt(camPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		//time += 0.008f;

		if (!lerp.CurveIsEmpty())
		{
			if (lerp.IsInterpolating())
			{
				lerp.Update();
				glm::vec3 lerpPos = lerp.GetPosition();
				currentCam->GetOwner()->SetPosition(lerpPos);
			}
			else
			{
				currentCam = transitionCam;
				transitionCam = nullptr;
				lerp.RemoveRegion(0);
			}
		}

		for (auto it : allCameras)
			it->Update();

#ifdef _EDITOR_
		if (currentCam
			&& GEditor->IsPlaying()
			)
		{
			mtxCam = currentCam->GetViewMtx();
			mtxProj = currentCam->GetProjMtx();
		}
		else
		{
			editCam.UpdateEditCam();
			mtxCam = editCam.GetCam()->GetViewMtx();
			mtxProj = editCam.GetCam()->GetProjMtx();
		}
#else
		mtxCam = currentCam->GetViewMtx();
		mtxProj = currentCam->GetProjMtx();
#endif

		//update space part
		if (m_using_space_part)
			m_gfx_space_part->Update();

		lightSys->Update();

		//mPostProcessor->BeginRender();

#ifdef _EDITOR_
		vec2 wSize = GEditor->GetViewportDimensions();
		glViewport(0, 0, wSize.x, wSize.y);
		//auto i = GEditor->GetBuffer();
		//glBindFramebuffer(GL_FRAMEBUFFER, GEditor->GetBuffer());

#else
		vec2 wSize = GWindow->GetDimensions();
		glViewport(0, 0, wSize.x, wSize.y);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
#ifdef _EDITOR_
		switch (GEditor->GetRenderOption())
		{
		case 0:
			RenderScene();
			break;
		default:
			shaders.shadowRenderShader->Bind();
			mat4 mtx1 = translate(mat4(1.0f), { 0, 0, 0 })
				* scale(mat4(1.0f), { 2, 2, 1 });
			shaders.shadowRenderShader->SetShaderUniform("mtx", &mtx1);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, lightSys->depthTex[GEditor->GetRenderOption() - 1]);
			glUniform1i(0, 0);
			shaders.shadowRenderShader->SetShaderUniform("texture0", 0);
			shaders.shadowRenderShader->SetShaderUniform("bShadow", true);
			objLoader->meshes[meshTypes[0]]->Render();
			break;
		}
#else

		RenderScene();

#endif
		//DebugRenderer::Instance().Render(mtxCam, mtxProj);

		//mPostProcessor->EndRender();
		//mPostProcessor->Render();
	}

	void GraphicsPipeline::AddCamera(Camera* cam) {
		allCameras.push_back(cam);
		currentCam = cam;
	}

	void GraphicsPipeline::DelCamera(Camera* cam) {
		allCameras.remove(cam);

		currentCam = nullptr;
	}

	Camera* GraphicsPipeline::GetCurrentCamera() const
	{
		return currentCam;
	}

	void GraphicsPipeline::AddRenderComp(Graphics::ShaderProgram* s, RenderComp* c)
	{
		shaderRenders[s].push_back(c);
	}

	void GraphicsPipeline::DelRenderComp(Graphics::ShaderProgram* s, RenderComp* c)
	{
		auto foundShader = shaderRenders.find(s);

		if (foundShader == shaderRenders.end())
			return;

		auto& list = foundShader->second;
		auto foundRender = std::find(list.begin(), list.end(), c);

		if (foundRender != list.end())
			list.erase(foundRender);
	}

	void GraphicsPipeline::SetCurrentCamera(Camera* cam, bool interpolate, float time, float speed)
	{
		if (!cam || cam == currentCam)
			return;

		if (!interpolate)
			currentCam = cam;
		else
		{
			transitionCam = cam;
			lerp.AddRegion(currentCam->GetPos(), transitionCam->GetPos(), Math::Interpolators::LINEAR, &time, speed);
			lerp.Play();
			lerp.Update();
		}
	}

	void GraphicsPipeline::AddRenderer(MeshRenderer* r)
	{
		rendComps.insert(r);
		AddRenderComp(r->shader, r);
	}

	void GraphicsPipeline::DelRenderer(MeshRenderer* r)
	{
		auto found = std::find(rendComps.begin(), rendComps.end(), r);
		if (found == rendComps.end())
			return;
		rendComps.erase(found);

		DelRenderComp(r->shader, r);
	}

	void GraphicsPipeline::AddText(Text3D* t)
	{
		text3DComps.push_back(t);
		AddRenderComp(t->mProgram, t);
	}

	void GraphicsPipeline::DelText(Text3D* t)
	{
		auto found = std::find(text3DComps.begin(), text3DComps.end(), t);
		if (found == text3DComps.end())
			return;
		text3DComps.erase(found);

		DelRenderComp(t->mProgram, t);
	}

	void GraphicsPipeline::AddHUD(Engine::UI::HUD* h)
	{
		huds.push_back(h);
	}

	void GraphicsPipeline::DelHUD(Engine::UI::HUD* h)
	{
		auto found = std::find(huds.begin(), huds.end(), h);
		if (found == huds.end())
			return;
		huds.erase(found);
	}
	void GraphicsPipeline::AddModel(Graphics::Model3D* m)
	{
		model3DComps.push_back(m);
		AddRenderComp(m->mProgram, m);
	}

	void GraphicsPipeline::DelModel(Graphics::Model3D* m)
	{
		auto Obj = std::find(model3DComps.begin(), model3DComps.end(), m);
		if (Obj == model3DComps.end())
			return;
		model3DComps.erase(Obj);

		DelRenderComp(m->mProgram, m);
	}

	void GraphicsPipeline::AddLight(Light* l)
	{
		if (lightSys->numLights >= lightSys->maxLights)
			return;

		lightSys->lights.push_back(l);
		lightSys->numLights++;
		for(int i = 0; i < lightSys->maxLights; ++i)
			if (!lightSys->depthBool[i])
			{
				lightSys->depthBool[i] = true;
				l->shadowID = i;
				break;
			}
	}

	void GraphicsPipeline::DelLight(Light* l)
	{
		if (lightSys->numLights <= 0)
			return;

		auto& lights = lightSys->lights;
		auto found = std::find(lights.begin(), lights.end(), l);
		if (found != lights.end())
		{
			lights.erase(found);
			lightSys->numLights--;
			lightSys->depthBool[l->shadowID] = false;
		}
	}

	Skybox::SkyboxComp* GraphicsPipeline::GetSkybox()
	{
		return mSkybox;
	}

	PostProcessor* GraphicsPipeline::GetPostProcessor()
	{
		return mPostProcessor;
	}

	void GraphicsPipeline::SetSkybox(Skybox::SkyboxComp* sb)
	{
		mSkybox = sb;
	}

	void GraphicsPipeline::DelSkybox()
	{
		if (mSkybox)
			delete mSkybox;
		mSkybox = nullptr;
	}

	//HERE IS WHERE ALL MODELS ARE INSERTED
	void GraphicsPipeline::LoadModels() {
		mLoadedModels.insert({ "Enemy/Zombie", new ModelData("EngineContent/Models/Enemies/Zombie.obj") });
		mLoadedModels.insert({ "Enemy/TurretBase", new ModelData("EngineContent/Models/Enemies/TurretBase.obj") });
		mLoadedModels.insert({ "Enemy/TurretHead", new ModelData("EngineContent/Models/Enemies/TurretHead.obj") });

		mLoadedModels.insert({ "Player/Player", new ModelData("EngineContent/Models/Players/Player1.obj") });

		mLoadedModels.insert({ "Rooms/Barrier", new ModelData("EngineContent/Models/Rooms/Barrier.obj") });
		mLoadedModels.insert({ "Rooms/Bridge", new ModelData("EngineContent/Models/Rooms/BridgePlatform.obj") });
		mLoadedModels.insert({ "Rooms/Pillar", new ModelData("EngineContent/Models/Rooms/Pillar.obj") });
		mLoadedModels.insert({ "Rooms/RoomFloor", new ModelData("EngineContent/Models/Rooms/RoomFloor.obj") });
		mLoadedModels.insert({ "Rooms/RoomWall_1", new ModelData("EngineContent/Models/Rooms/RoomWalls1.obj") });
		mLoadedModels.insert({ "Rooms/RoomWall_2", new ModelData("EngineContent/Models/Rooms/RoomWalls2.obj") });
		mLoadedModels.insert({ "Rooms/RoomWall_3", new ModelData("EngineContent/Models/Rooms/RoomWalls3.obj") });
		mLoadedModels.insert({ "Rooms/RoomWall_4", new ModelData("EngineContent/Models/Rooms/RoomWalls4.obj") });
		mLoadedModels.insert({ "Props/Light", new ModelData("EngineContent/Models/Props/Light.obj") });
		mLoadedModels.insert({ "Props/Wires", new ModelData("EngineContent/Models/Props/Wires.obj") });
		mLoadedModels.insert({ "Extra/Bullet", new ModelData("EngineContent/Models/Extra/Bullet.obj") });

		// ... 
	}

	void GraphicsPipeline::DrawLine(const glm::vec3& s, const glm::vec3& e, const glm::vec4& c)
	{
		linesData.push_back(std::make_pair(glm::translate(glm::mat4(1), s) * glm::scale(glm::mat4(1), e - s), c));
	}

	void DrawLine(const glm::vec3& s, const glm::vec3& e, const glm::vec4& c)
	{
		GGfxPipeline->DrawLine(s, e, c);
	}

	void GraphicsPipeline::RenderLines()
	{
		shaders.debugShader->Bind();
		glUniformMatrix4fv(1, 1, GL_FALSE, &mtxCam[0][0]);
		glUniformMatrix4fv(2, 1, GL_FALSE, &mtxProj[0][0]);
		for (const auto& data : linesData)
		{
			const glm::mat4& m = data.first;
			const glm::vec4& c = data.second;
			glUniformMatrix4fv(0, 1, GL_FALSE, &m[0][0]);
			glUniform4f(3, c.r, c.g, c.b, c.a);
			lineShape.BindForDrawing();
			glDrawArrays(GL_LINES, 0, lineShape.VtxCount());
			glBindVertexArray(0);
		}
		linesData.clear();
	}
}
