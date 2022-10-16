//
//	FrameBuffer.h
//	Good Neighbours
//
//	Created by Diego Revilla on 10/10/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _FRAME_BUFFER__H_
#define _FRAME_BUFFER__H_

namespace Engine {
	namespace Graphics {
		struct ENGINE_API Framebuffer
		{
#pragma region // Declarations
			enum class FB_Type : unsigned char
			{
				SceneFramebuffer,
				QuadFramebuffer
			};

#pragma endregion

#pragma region // Constructors & Destructors
			Framebuffer(const unsigned width = 400, const unsigned height = 400, const GLint internalFormat = GL_RGB, GLenum datatype = GL_UNSIGNED_BYTE) noexcept;
			~Framebuffer() noexcept;
#pragma endregion

#pragma region //Members
		private:
			glm::uvec2 mSize;
			GLuint mFbID;
			GLuint mTexID;
			GLuint mDepthID;
#pragma endregion

#pragma region // Methods
		public:
			void inline BindFramebuffer() const noexcept;
			void inline UnbindFramebuffer() const noexcept;
			void inline BindTexture() const noexcept;
			void inline UnbindTexture() const noexcept;
			DONTDISCARD decltype(mFbID) GetFrameBufferID() const noexcept;
			DONTDISCARD decltype(mTexID) GetTextureID() const noexcept;
			DONTDISCARD decltype(mSize) GetSize() const noexcept;
#pragma endregion
		};

		// ------------------------------------------------------------------------
		/*! Get FrameBuffer ID
		*
		*   Gets the OpenGL Handle of the FrameBuffer
		*/ //----------------------------------------------------------------------
		decltype(Framebuffer::mFbID) inline Framebuffer::GetFrameBufferID() const noexcept {
			return mFbID;
		}

		// ------------------------------------------------------------------------
		/*! Get Texture ID
		*
		*   Gets the OpenGL Handle of the FrameBuffer as a Texture
		*/ //----------------------------------------------------------------------
		decltype(Framebuffer::mTexID) inline Framebuffer::GetTextureID() const noexcept {
			return mTexID;
		}

		// ------------------------------------------------------------------------
		/*! Get Size
		*
		*   Gets the Size of the FrameBuffer
		*/ //----------------------------------------------------------------------
		decltype(Framebuffer::mSize) inline Framebuffer::GetSize() const noexcept {
			return mSize;
		}
	}
}

#endif