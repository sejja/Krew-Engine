//
//	ParticleEmitter.cpp
//	Good Neighbours
//
//	Created by Jon Ibarra on 06/10/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

namespace Engine
{
	namespace Graphics
	{
		Engine::Threading::ThreadPool ParticleEmitter::particle_tp = Engine::Threading::ThreadPool(2);


		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Sets the default paramenters and initializes the emitter
		*/ //--------------------------------------------------------------------
		ParticleEmitter::ParticleEmitter(): mMaxParticles(1000u), mActiveParticles(0u), mParticleGenerationRate(10u), mLastUsedParticle(0),
			mActive(true), mEmit(true), mParticleColor(1,1,1,1), mParticleSize(1.f), mSpeedDirection(0), mSpeedIntensity(1.f), mUsingThreadPools(false)
		{
			isEmitter = true;
			mBlendingType = BlendingType::eModular;
			Initialize();
			GGfxPipeline->AddRenderComp(mShader, this);
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Deletes the shader
		*/ //--------------------------------------------------------------------
		ParticleEmitter::~ParticleEmitter()
		{
			GGfxPipeline->DelRenderComp(mShader, this);
		}

		// ------------------------------------------------------------------------
		/*! From Json
		*
		*   Gets its values from a json
		*/ //--------------------------------------------------------------------
		void ParticleEmitter::FromJson(const nlohmann::json& val)
		{
			fetch(mBlendingType, val, "mBlendingType");
			fetch(mMaxParticles, val, "mMaxParticles");
			fetch(mActiveParticles, val, "mActiveParticles");
			fetch(mParticleGenerationRate, val, "mParticleGenerationRate");
			fetch(mEmit, val, "mEmit");

			auto it = val.find("mParticleColor");
			if (it != val.end())
			{
				mParticleColor.x = val["mParticleColor"][0].get<float>();
				mParticleColor.y = val["mParticleColor"][1].get<float>();
				mParticleColor.z = val["mParticleColor"][2].get<float>();
			}

			fetch(mParticleSize, val, "mParticleSize");

			it = val.find("mSpeedDirection");
			if (it != val.end())
			{
				mSpeedDirection.x = val["mSpeedDirection"][0].get<float>();
				mSpeedDirection.y = val["mSpeedDirection"][1].get<float>();
				mSpeedDirection.z = val["mSpeedDirection"][2].get<float>();
			}

			fetch(mSpeedIntensity, val, "mSpeedIntensity");
			mRange = val["mRange"].get<float>();

			fetch(mUsingThreadPools, val, "mUsingThreadPools");

			mShader = GContent->GetEngineResource<ShaderProgram>("Shaders\\shaderParticle.program")->Get();

			InitializeParticleEmision();
			//mMesh = GGfxPipeline->objLoader->meshes[meshTypes[particle]];
		}

		// ------------------------------------------------------------------------
		/*! To Json
		*
		*   Adds its values to a json
		*/ //--------------------------------------------------------------------
		void ParticleEmitter::ToJson(nlohmann::json& val) const
		{
			val["mBlendingType"] = mBlendingType;
			val["mMaxParticles"] = mMaxParticles;
			val["mActiveParticles"] = mActiveParticles;
			val["mParticleGenerationRate"] = mParticleGenerationRate;
			val["mEmit"] = mEmit;
			val["mParticleColor"][0] = mParticleColor.x;
			val["mParticleColor"][1] = mParticleColor.y;
			val["mParticleColor"][2] = mParticleColor.z;
			val["mParticleSize"] = mParticleSize;
			val["mSpeedDirection"][0] = mSpeedDirection.x;
			val["mSpeedDirection"][1] = mSpeedDirection.y;
			val["mSpeedDirection"][2] = mSpeedDirection.z;
			val["mSpeedIntensity"] = mSpeedIntensity;
			val["mUsingThreadPools"] = mUsingThreadPools;
			val["mRange"] = mRange;
		}

		// ------------------------------------------------------------------------
		/*! Initialize
		*
		*   Initilaizes the values for the shader to use
		*/ //--------------------------------------------------------------------
		void ParticleEmitter::Initialize()
		{
			InitializeParticleEmision();

			mShader = GGfxPipeline->shaders.shaderParticle;

			Mesh* mesh = GGfxPipeline->objLoader->meshes[meshTypes[particle]];
			size_t size = mesh->vtx.size();
			for (size_t i = 0; i < size; i++)
			{
				g_vertex_buffer_data[i * 3 + 0] = mesh->vtx[i].position.x;
				g_vertex_buffer_data[i * 3 + 1] = mesh->vtx[i].position.y;
				g_vertex_buffer_data[i * 3 + 2] = mesh->vtx[i].position.z;
			}

			mShader->Bind();

			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			glGenBuffers(1, &vertex_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glVertexAttribDivisor(0, 0);

			// The VBO containing the positions and sizes of the particles
			glGenBuffers(1, &particles_position_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
			// Initialize with empty (NULL) buffer : it will be updated later, each frame.
			glBufferData(GL_ARRAY_BUFFER, mMaxParticles * sizeof(glm::vec4), NULL, GL_STREAM_DRAW);

			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glVertexAttribDivisor(1, 1);

			//// The VBO containing the colors of the particles
			//glGenBuffers(1, &particles_color_buffer);
			//glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
			//// Initialize with empty (NULL) buffer : it will be updated later, each frame.
			//glBufferData(GL_ARRAY_BUFFER, mMaxParticles * sizeof(glm::vec4), NULL, GL_STREAM_DRAW);
		}

		// ------------------------------------------------------------------------
		/*! Initiliaze Particle Emision
		*
		*   Fills the particle and particle_position data with default values
		*/ //--------------------------------------------------------------------
		void ParticleEmitter::InitializeParticleEmision()
		{
			particles.clear();
			g_particule_position_data.clear();
			for (unsigned i = 0; i < mMaxParticles; ++i)
			{
				particles.push_back(Particle());
				g_particule_position_data.push_back(glm::vec4(0.f));
				//g_particule_color_data.push_back(glm::vec4(1.f));
			}
		}

		// ------------------------------------------------------------------------
		/*! First Unused Particle
		*
		*   Returns an integer to the first unused particle of the particle container
		*/ //--------------------------------------------------------------------
		unsigned ParticleEmitter::FirstUnusedParticle()
		{
			unsigned size = particles.size();
			// search from last used particle, this will usually return almost instantly
			for (unsigned int i = mLastUsedParticle; i < size; ++i) {
				if (particles[i].mLife <= 0.0f) {
					mLastUsedParticle = i;
					return i;
				}
			}
			// otherwise, do a linear search
			for (unsigned int i = 0; i < mLastUsedParticle; ++i) {
				if (particles[i].mLife <= 0.0f) {
					mLastUsedParticle = i;
					return i;
				}
			}
			// override first particle if all others are alive
			mLastUsedParticle = 0;
			return -1;
		}

		// ------------------------------------------------------------------------
		/*! Respawn Particle
		*
		*   Sets a given particle to respawn, and is given new life and speed values
		*/ //--------------------------------------------------------------------
		void ParticleEmitter::RespawnParticle(Particle& particle, GameObject& object, glm::vec3 offset)
		{
			float random = ((rand() % 100) - 50) * mRange;
			float random_sx = ((rand() % 100) - 50) * mRange;
			float random_sy = ((rand() % 100) - 50) * mRange;
			float random_sz = ((rand() % 100) - 50) * mRange;
			float rColor = 0.5f + ((rand() % 100) / 100.0f);
			particle.mPosition = object.GetWorldPosition();
			if (mSpeedDirection != glm::vec3(0))
				particle.mVelocity = (glm::vec3(random_sx, random_sy, random_sz) + mSpeedDirection * mSpeedIntensity);
			else
				particle.mVelocity = (glm::vec3(random_sx, random_sy, random_sz) * mSpeedIntensity);
			particle.mColor = glm::vec4(rColor, rColor, rColor, 1.0f);
			particle.mLife = 1.0f;
			//particle.mVelocity = object.Velocity * 0.1f; 
		}

		// ------------------------------------------------------------------------
		/*! Update Particle Pool
		*
		*   Updates the position and alive statu of all the particles in the pool
		*/ //--------------------------------------------------------------------
		void ParticleEmitter::UpdateParticlePool()
		{
			auto offset = (mSpeedDirection == glm::vec3(0) ? glm::vec3(1) : mSpeedDirection) * mSpeedIntensity * 0.5f;
			glm::vec3 half_size = glm::vec3(50.f * mRange) + offset;
			glm::vec3 pos = GetOwner()->GetWorldPosition() + mSpeedDirection * half_size;
			bv.min = pos - half_size;
			bv.max = pos + half_size;

			const auto respawning_particles = [this]()
			{
			SortParticles();
				GameObject* owner = GetOwner();
				// add new particles
				if (mActive && mEmit)
				{
					for (int i = 0; i < mParticleGenerationRate; ++i)
					{
						int unusedParticle = FirstUnusedParticle();
						if (unusedParticle == -1)
							break;
						RespawnParticle(particles[unusedParticle], *owner);
					}
				}
				mActiveParticles = 0;
				//glm::vec3 camera_pos = GWorld->GetScene()->GetCamera();
			};

			const auto updating_particles = [this]()
			{
				// update all particles
				float dt = GFrames->GetFrameTime();
				for (int i = 0; i < mMaxParticles; i++)
				{
					Particle& p = particles[i];

					if (p.mLife > 0.f)
					{
						// Decrease life
						p.mLife -= dt;
						if (p.mLife > 0.f)
						{
							// Simulate physics (gravity, no collision)
							p.mVelocity += (float)dt * 0.5f;
							p.mPosition += p.mVelocity * (float)dt;
							p.mCameraDistance = glm::length2(p.mPosition - GGfxPipeline->camPos);

							// Fill the GPU buffer
							g_particule_position_data[mActiveParticles].x = p.mPosition.x;
							g_particule_position_data[mActiveParticles].y = p.mPosition.y;
							g_particule_position_data[mActiveParticles].z = p.mPosition.z;

							g_particule_position_data[mActiveParticles].w = p.mLife;

							//g_particule_color_data[mActiveParticles].r = p.mColor.r;
							//g_particule_color_data[mActiveParticles].g = p.mColor.g;
							//g_particule_color_data[mActiveParticles].b = p.mColor.b;
							//g_particule_color_data[mActiveParticles].a = p.mColor.a;
						}
						else
						{
							// Particles that just died will be put at the end of the buffer in SortParticles();
							p.mCameraDistance = -1.0f;
						}

						mActiveParticles++;
					}
				}
			};

			if (mUsingThreadPools)
			{
				particle_tp.AddTask(respawning_particles);
				particle_tp.AddTask(updating_particles);
			}
			else
			{
				respawning_particles();
				updating_particles();
			}
		}

		// ------------------------------------------------------------------------
		/*! Sort Particles
		*
		*   Sorts the particles in the container according to their mLife value
		*/ //--------------------------------------------------------------------
		void ParticleEmitter::SortParticles()
		{
			std::sort(particles.begin(), particles.end());
		}

		// ------------------------------------------------------------------------
		/*! Tick
		*
		*   Update of the class on Gameplay
		*/ //--------------------------------------------------------------------
		void ParticleEmitter::Tick()
		{
			UpdateParticlePool();
		}

		// ------------------------------------------------------------------------
		/*! Render
		*
		*   Draws the pzarticle pool instanced
		*/ //--------------------------------------------------------------------
		void ParticleEmitter::Render()
		{
			if (drawBV)
				GDebug->DebugDrawAABB(GetBV(), { 1, 0, 0, 1 });

			// Update the buffers that OpenGL uses for rendering
			mShader->Bind();

			glBindVertexArray(vao);

			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 72 vertices -> 0

			// Updating the buffer object with the new list of particle positions
			glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
			glBufferData(GL_ARRAY_BUFFER, mMaxParticles * sizeof(glm::vec4), NULL, GL_STREAM_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, mActiveParticles * sizeof(glm::vec4), g_particule_position_data.data());

			// 2nd attribute buffer : positions of particles' centers
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1

			//glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
			//glBufferData(GL_ARRAY_BUFFER, mMaxParticles * sizeof(glm::vec4), NULL, GL_STREAM_DRAW);
			//glBufferSubData(GL_ARRAY_BUFFER, 0, mActiveParticles * sizeof(glm::vec4), g_particule_color_data.data());

			/*// 3rd attribute buffer : particles' colors
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
			glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);
			glVertexAttribDivisor(2, 1); // color : one per quad -> 1*/

			glBindVertexArray(vao);
			
			//glEnable(GL_DEPTH_TEST);
			switch (mBlendingType)
			{
			case BlendingType::eOpaque:
				glDisable(GL_BLEND);
				break;
			case BlendingType::eAdditive:
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				break;
			case BlendingType::eModular:
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				break;
			}

			mShader->SetShaderUniform("P", &GGfxPipeline->mtxProj);
			mShader->SetShaderUniform("V", &GGfxPipeline->mtxCam);
			mShader->SetShaderUniform("color", &mParticleColor);
			mShader->SetShaderUniform("scale", &mParticleSize);

			// Draw the particules !
			// This draws many times a small triangle_strip (which looks like a quad).
			// This is equivalent to :
			// for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4),
			// but faster.
			glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, sizeof(g_vertex_buffer_data), mActiveParticles);

			glBindVertexArray(0);


			glEnable(GL_CULL_FACE);
		}

		void ParticleEmitter::SetEmission(bool e)
		{
			mEmit = e;
		}
		void ParticleEmitter::SetActive(bool a)
		{
			mActive = a;
		}

		void ParticleEmitter::SetParticleColor(glm::vec4 c)
		{
			mParticleColor = c;
		}		
		void ParticleEmitter::SetParticleSize(float s)
		{
			mParticleSize = s;
		}
		void ParticleEmitter::SetEmissionDirection(glm::vec3 d)
		{
			mSpeedDirection = d;
		}
		void ParticleEmitter::SetEmissionSpeed(float s)
		{
			mSpeedIntensity = s;
		}
		void ParticleEmitter::SetEmissionRange(float r)
		{
			mRange = r;
		}
		void ParticleEmitter::SetEmissionIntensity(float i)
		{
			mSpeedIntensity = i;
		}

		// ------------------------------------------------------------------------
		/*! Editor Tick
		*
		*   Update of the class in Editor
		*/ //--------------------------------------------------------------------
		void ParticleEmitter::EditorTick()
		{
			UpdateParticlePool();
		}

		// ------------------------------------------------------------------------
		/*! On Editor
		*
		*   Imgui implementations of the class
		*/ //--------------------------------------------------------------------
		void ParticleEmitter::OnEditor()
		{
#ifdef _EDITOR_
			// On/Off Button for the threads
			ImGui::Checkbox("Use Thread Pools", &mUsingThreadPools);

			ImGui::Checkbox("Debug", &drawBV);

			const char* customNames[3] = { "Opaque", "Additive", "Modular" };
			// Choosing blending type
			int t = (int)mBlendingType;
			ImGui::Combo("Blending Type", &t, customNames, 3);
			mBlendingType = (BlendingType)(t);

			// On/Off Button
			ImGui::Checkbox("Emit Particles", &mEmit);
			bool real_active = mEmit;

			// Printing the current particle count
			std::string s1("Current number of particles: ");
			std::string s2(std::to_string(mActiveParticles));
			ImGui::BulletText((s1 + s2).c_str());

			mActive = true;
			// Particles generated per frame
			if (ImGui::DragInt("New particles per frame", &mParticleGenerationRate, 0.5f, 0, mMaxParticles, "%d"))
				mActive = false;

			// Maximun particles of the emitter
			if (ImGui::DragInt("Maximun particle number", &mMaxParticles, 0.5f, 0, 5000, "%d"))
			{
				mActive = false;
				// Updating the particle vector size if necessary
				while (particles.size() < mMaxParticles)
				{
					particles.push_back(Particle());
					g_particule_position_data.push_back(glm::vec4(0.f));
				}
			}

			ImGui::Text("Color widget with Float Display:");
			ImGui::ColorEdit4("MyColor##2f", (float*)&mParticleColor, ImGuiColorEditFlags_Float);

			// Particle size
			ImGui::DragFloat("Particle size", &mParticleSize, 0.1f, 0.1f, 100.f, "%.3f");

			// Speed direction
			if (ImGui::DragFloat3("Speed direction", &mSpeedDirection[0], 0.01f, 0.0f, 25.0f) && glm::length2(mSpeedDirection))
				mSpeedDirection = glm::normalize(mSpeedDirection);

			// Speed intensity
			ImGui::DragFloat("Speed intensity", &mSpeedIntensity, 0.01f, 0.1f, 100.f, "%.3f");

			// Range
			ImGui::DragFloat("Range", &mRange, 0.01f, 0.01f, 1.f, "%.3f");
#endif
		}
	}
	
}
