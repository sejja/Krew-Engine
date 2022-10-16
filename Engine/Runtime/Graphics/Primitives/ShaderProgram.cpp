//
//	ShaderProgram.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

namespace Engine {
	namespace Graphics {
		// ------------------------------------------------------------------------
		/*! Default Constructor
		*
		*   Constructs a Shader Program Class
		*/ // --------------------------------------------------------------------
		ShaderProgram::ShaderProgram() {
			Handle = glCreateProgram();
		}

		// ------------------------------------------------------------------------
		/*! Custom Constructor
		*
		*   Constructs a Shader Program class given a Vertex and Fragment Shader
		*/ // --------------------------------------------------------------------
		ShaderProgram::ShaderProgram(Shader* vertexShader, Shader* fragmentShader)
			: ShaderProgram() {
			//Set the vertex shader
			if (vertexShader)
				Shaders[static_cast<unsigned char>(vertexShader->ShaderType)]
				= vertexShader;

			//Set the fragment shader
			if (fragmentShader)
				Shaders[static_cast<unsigned char>(fragmentShader->ShaderType)]
				= fragmentShader;

			Link();
		}

		// ------------------------------------------------------------------------
		/*! Link
		*
		*   Links both Vertex and Fragment Shaders to our Program
		*/ // --------------------------------------------------------------------
		void ShaderProgram::Link() {
			//If we have a Vertex Shader
			if (Shaders[0])
				glAttachShader(Handle, Shaders[0]->Handle);

			//If we have a Fragment Shader
			if (Shaders[1])
				glAttachShader(Handle, Shaders[1]->Handle);

			GLint status;

			glLinkProgram(Handle);
			glGetProgramiv(Handle, GL_LINK_STATUS, &status);

#ifdef _DEBUG
			if (status == GL_FALSE) {
				glGetProgramiv(Handle, GL_INFO_LOG_LENGTH, &status);

				if (status > 0) {
					char* log = reinterpret_cast<char*>(malloc(status));
					GLsizei written;

					glGetProgramInfoLog(Handle, status, &written, log);
					MessageBoxA(NULL, log, "Shader Linking Error",
						MB_TASKMODAL | MB_SETFOREGROUND | MB_ICONERROR);
					free(log);
				}
			}
#endif

			DetachShader(static_cast<Shader::EType>(2));
		}

		// ------------------------------------------------------------------------
		/*! Bind
		*
		*   Binds the Shader Program
		*/ // --------------------------------------------------------------------
		void ShaderProgram::Bind() {
			glUseProgram(Handle);
		}

		// ------------------------------------------------------------------------
		/*! Unbind
		*
		*   Unbinds the Shader Program
		*/ // --------------------------------------------------------------------
		void ShaderProgram::Unbind() {
			glUseProgram(NULL);
		}

		// ------------------------------------------------------------------------
		/*! Load
		*
		*   Loads a Shader Program
		*/ // --------------------------------------------------------------------
		void ShaderProgram::Load() {
			//If we have a vertex shader
			if (Shaders[0]) {
				Shaders[0]->LoadSource();
				Shaders[0]->Compile("vertex shader");
			}

			//If we have a fragment shader
			if (Shaders[1]) {
				Shaders[1]->LoadSource();
				Shaders[1]->Compile("fragment shader");
			}

			Link();
		}

		// ------------------------------------------------------------------------
		/*! Get Shader
		*
		*   Gets a Shader from our Shader Program
		*/ // --------------------------------------------------------------------
		Shader* ShaderProgram::GetShader(Shader::EType shaderType) {
			return Shaders[static_cast<unsigned char>(shaderType)];
		}

		// ------------------------------------------------------------------------
		/*! Set Shader
		*
		*   Sets a shader in our program
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShader(Shader* pShader, bool link) {
			//If we have a valid pointer to our shader
			if (pShader) {
				DetachShader(pShader->ShaderType);
				Shaders[static_cast<unsigned char>(pShader->ShaderType)]
					= pShader;

				//If we are requested to link it
				if (link)
					Link();
			}
		}

		void ShaderProgram::SetShaderUniform(const char* name, int value) {
			int loc = glGetUniformLocation(Handle, name);

			if (loc >= 0) {
				glUniform1i(loc, value);
			}
		}

		void ShaderProgram::SetShaderUniform(const char* name, bool value) {
			int loc = glGetUniformLocation(Handle, name);

			if (loc >= 0) {
				glUniform1i(loc, value);
			}
		}

		// ------------------------------------------------------------------------
		/*! Attach Shader
		*
		*   Attach one of the shaders in this program
		*/ // --------------------------------------------------------------------
		void ShaderProgram::AttachShader(Shader::EType shaderType) {
			//If we have a valid shader
			if (Shaders[static_cast<unsigned char>(shaderType)])
				glAttachShader(Handle, Shaders[
					static_cast<unsigned char>(shaderType)]->Handle);
		}

		// ------------------------------------------------------------------------
		/*! Detach Shader
		*
		*   Detach one of the shaders is this program
		*/ // --------------------------------------------------------------------
		void ShaderProgram::DetachShader(Shader::EType shaderType) {
			// specific shadertype ->set if necessary
			if (static_cast<unsigned char>(shaderType) != 2)
				if (Shaders[static_cast<unsigned char>(shaderType)]) {
					glDetachShader(Handle, Shaders[static_cast<unsigned char>(shaderType)]->Handle);
				}
				else {
					if (Shaders[0])
						glDetachShader(Handle, Shaders[0]->Handle);

					if (Shaders[1])
						glDetachShader(Handle, Shaders[1]->Handle);
				}
		}

		// ------------------------------------------------------------------------
		/*! Get Open GL Handler
		*
		*   Gets the OpenGL Handle of this program
		*/ // --------------------------------------------------------------------
		unsigned int ShaderProgram::GetOpenGLHandle() {
			return Handle;
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of integers
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const char* name, int* value, int count) {
			glUniform1iv(glGetUniformLocation(Handle, name), count, value);
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of floats
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const char* name, float* value, int count) {
			glUniform1fv(glGetUniformLocation(Handle, name), count, value);
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of Matrices 4x4
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const char* name, glm::mat4* value, int count) {
#ifdef _DEBUG
			int loc = glGetUniformLocation(Handle, name);

			//If we could retrieve the uniform location
			if (loc != -1)
				glUniformMatrix4fv(loc, count, GL_FALSE, reinterpret_cast<float*>(value));
#else
			glUniformMatrix4fv(glGetUniformLocation(Handle, name), count, GL_FALSE,
				reinterpret_cast<float*>(value));
#endif
		}

		Graphics::ShaderProgram::~ShaderProgram() {
			Allocator<Shader>::destroy(Shaders[0]);
			Allocator<Shader>::destroy(Shaders[1]);
			Allocator<Shader>::deallocate(Shaders[0]);
			Allocator<Shader>::deallocate(Shaders[1]);
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of matrices 3x3
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const char* name, glm::mat3* value, int count) {
			glUniformMatrix4fv(glGetUniformLocation(Handle, name), count, GL_FALSE,
				reinterpret_cast<float*>(value));
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of Vectors of size 2
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const char* name, glm::vec2* value, int count) {
			glUniform2fv(glGetUniformLocation(Handle, name), count,
				reinterpret_cast<float*>(value));
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of Vectors of size 3
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const char* name, glm::vec3* value, int count) {
			glUniform3fv(glGetUniformLocation(Handle, name), count,
				reinterpret_cast<float*>(value));
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of Vectors of size 4
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const char* name, glm::vec4* value, int count) {
			int loc = glGetUniformLocation(Handle, name);
			glUniform4fv(loc, count,
				reinterpret_cast<float*>(value));
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Uniform
		*
		*   Sets uniform values in form of a Color
		*/ // --------------------------------------------------------------------
		void ShaderProgram::SetShaderUniform(const char* name, Color* value, int count) {
			glUniform4fv(glGetUniformLocation(Handle, name), count,
				reinterpret_cast<float*>(value));
		}
	}
}