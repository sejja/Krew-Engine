//
//	Shader.cpp
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
		*   Constructs a Shader
		*/ // --------------------------------------------------------------------
		Shader::Shader() :
			Handle(NULL), ShaderType(EType::Vertex), Source(nullptr) {
		}

		// ------------------------------------------------------------------------
		/*! Destrutor
		*
		*   Frees a Shader
		*/ // --------------------------------------------------------------------
		Shader::~Shader() {
			//If we have a valid Source
			if (this && Source)
				free(Source);
		}

		// ------------------------------------------------------------------------
		/*! Custom Constructor
		*
		*   Constructs a Shader given a file
		*/ // --------------------------------------------------------------------
		Shader::Shader(const char* filename, EType type) :
			ShaderType(type) {
#ifdef _DEBUG
			if (LoadSource(filename))
#else
			LoadSource(filename);
#endif
			Compile(filename);
		}

		// ------------------------------------------------------------------------
		/*! Load Source
		*
		*   Loads a Source for our Shader
		*/ // --------------------------------------------------------------------
		bool Shader::LoadSource(const char* filename) {
			std::fstream shaderFile(filename);
			std::stringstream shaderSource;

			shaderSource << shaderFile.rdbuf();

			//If we could allocate the string
			if (Source = reinterpret_cast<char*>(malloc(strlen(shaderSource.str().c_str()) + 1)))

				strcpy(Source, shaderSource.str().c_str());
			else
				return false;

			return true;
		}

		// ------------------------------------------------------------------------
		/*! Compile
		*
		*   Compiles the Shader
		*/ // --------------------------------------------------------------------
		bool Shader::Compile(const char* filename) {
			//If there is a valid source file
			if (Source && strlen(Source)) {
				SetShaderType(ShaderType, !Handle);
				glShaderSource(static_cast<GLuint>(Handle), 1, &Source, NULL);
				glCompileShader(static_cast<GLuint>(Handle));

				// sanity check
				GLint result;
				glGetShaderiv(static_cast<GLuint>(Handle), GL_COMPILE_STATUS, &result);

#ifdef _DEBUG
				//If there has been errors during compilation
				if (!result) {
					GLint logLen;
					glGetShaderiv(static_cast<GLuint>(Handle), GL_INFO_LOG_LENGTH, &logLen);

					//If there are multiple error's
					if (logLen > 0) {
						char* log = (char*)malloc(logLen);
						GLsizei written;

						glGetShaderInfoLog(static_cast<GLuint>(Handle), logLen, &written, log);

						char str[100];

						//sprintf_s(str, "Shader Compilation Error: %s", filename);
						MessageBoxA(NULL, log, str, MB_TASKMODAL | MB_SETFOREGROUND | MB_ICONERROR);
						free(log);
					}

					return false;
				}
#endif

				return true;
			}

			return false;
		}

		// ------------------------------------------------------------------------
		/*! Create Device Shader
		*
		*   Creates a Device for the Shader
		*/ // --------------------------------------------------------------------
		bool Shader::CreateDeviceShader() {
			GLenum err = glewInit();

			//If we have a valid Handler
			if (Handle)
				glDeleteShader(static_cast<GLuint>(Handle));

			Handle = glCreateShader(ShaderType == EType::Vertex ?
				GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);

			return true;
		}

		// ------------------------------------------------------------------------
		/*! Set Shader Type
		*
		*   Sets the type of the shader
		*/ // --------------------------------------------------------------------
		void Shader::SetShaderType(Shader::EType shaderType,
			bool createDeviceShader) {
			ShaderType = shaderType;

			//If we need to create a device
			if (createDeviceShader)
				CreateDeviceShader();
		}
	}
}