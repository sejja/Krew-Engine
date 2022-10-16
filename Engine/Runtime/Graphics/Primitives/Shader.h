//
//	Shader.h
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _SHADER__H_
#define _SHADER__H_

namespace Engine {
	namespace Graphics {
		class  Shader {
		public:
			enum class EType : unsigned char {
				Vertex,
				Fragment
			};
			Shader();
			~Shader();
			Shader(const char* filename, EType type);
			bool LoadSource(const char* filename = nullptr);
			bool Compile(const char* filename);
			void SetShaderType(EType type, bool createDeviceShader = false);
			bool CreateDeviceShader();

		private:
			friend class ShaderProgram;
			unsigned int Handle;
			char* Source;
			EType ShaderType;
		};
	}
}

#endif
