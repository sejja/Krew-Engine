//
//	Texture.h
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _TEXTURE__H_
#define _TEXTURE__H_

namespace Engine {
	namespace Graphics {
		class ENGINE_API Texture {
		public:

			enum TextureType {
				eDiffuse,
				eNormal,
				eSpecular,
				e
			};

			Texture();
			Texture(const size_t width, const size_t height,
				unsigned char* pixels = nullptr);
			Texture(const char* filename);
			~Texture();
			void LoadFromFile(const char* filename);
			void UploadToGPU();
			void Bind() const;
			void inline Unbind() const;

			// ------------------------------------------------------------------------
			/*! Get Width
			*
			*   Gets the Width of the texture
			*/ // --------------------------------------------------------------------
			DONTDISCARD size_t inline GetWidth() const {
				return Width;
			}

			// ------------------------------------------------------------------------
			/*! Get Height
			*
			*   Gets the Height of the Texture
			*/ // --------------------------------------------------------------------
			DONTDISCARD size_t inline GetHeight() const {
				return Height;
			}

			// ------------------------------------------------------------------------
			/*! Get GL Handle
			*
			*   Gets the OpenGL Object Handle
			*/ // --------------------------------------------------------------------
			DONTDISCARD unsigned inline GetGLHandle() const {
				return Handle;
			}

			unsigned char* mPixels;
			TextureType mType;
		private:
			size_t Width;
			size_t Height;
			GLuint Handle;
			void CreateOpenGLTexture();
			void FreeData();
		};
	}
}

#endif
