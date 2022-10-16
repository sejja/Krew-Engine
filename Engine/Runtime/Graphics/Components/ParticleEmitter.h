//
//	ParticleEmitter.h
//	Good Neighbours
//
//	Created by Jon Ibarra on 06/10/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#ifndef _GFX_PARTICLE_EMITTER__H_
#define _GFX_PARTICLE_EMITTER__H_

namespace Engine {
	namespace Graphics {
		struct Particle;

		class ENGINE_API ParticleEmitter : public RenderComp
		{
		public:
			static Engine::Threading::ThreadPool particle_tp;

			ParticleEmitter();
			~ParticleEmitter();

			void FromJson(const nlohmann::json& val) override;
			void ToJson(nlohmann::json& val) const override;

			void Initialize();
			void InitializeParticleEmision();
			void UpdateParticlePool();
			unsigned FirstUnusedParticle();
			void RespawnParticle(Particle& particle, GameObject& object, glm::vec3 offset = glm::vec3(0.f));
			void SortParticles();

			void Tick() override;
			void Render() override;
			void EditorTick() override;
			void OnEditor() override;


			void SetEmission(bool e);
			void SetActive(bool a);
			void SetParticleColor(glm::vec4 c);
			void SetParticleSize(float c);
			void SetEmissionDirection(glm::vec3 d);
			void SetEmissionSpeed(float s);
			void SetEmissionRange(float r);
			void SetEmissionIntensity(float i);

			float GetEmissionRange() const { return mRange; }

		private:
			std::vector<Particle> particles;

			int mMaxParticles;
			int mActiveParticles;
			int mParticleGenerationRate;

			int mLastUsedParticle;

			bool mActive;
			bool mEmit;
			glm::vec4 mParticleColor;
			float mParticleSize;
			glm::vec3 mSpeedDirection;
			float mSpeedIntensity;
			float mRange = 1 / 10.0f; 
			bool mDebug = false;

			bool mUsingThreadPools;

			Graphics::ShaderProgram* mShader = nullptr;

			//Mesh* mMesh = nullptr;
			GLfloat g_vertex_buffer_data[216];

			Array<glm::vec4> g_particule_position_data;
			//Array<glm::vec4> g_particule_color_data;

			GLuint vao;
			GLuint vertex_buffer;
			GLuint particles_position_buffer;
			//GLuint particles_color_buffer;
		};
	}
}

#endif
