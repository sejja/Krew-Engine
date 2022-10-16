//
//	FrameBuffer.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 10/10/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>

namespace Engine {
	namespace Graphics {
		// ------------------------------------------------------------------------
		/*! Custom Constructor
		*
		*   Constructs a FrameBuffer for the GPU given a size
		*/ //----------------------------------------------------------------------
		Framebuffer::Framebuffer(const unsigned width, const unsigned height, const GLint internalFormat, GLenum datatype) noexcept : 
			mSize(width, height) {
			glGenFramebuffers(1, &mFbID);
			glBindFramebuffer(GL_FRAMEBUFFER, mFbID);
			glGenTextures(1, &mTexID);
			glBindTexture(GL_TEXTURE_2D, mTexID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, static_cast<int>(width),
				static_cast<int>(height), 0, GL_RGB, GL_FLOAT, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glGenRenderbuffers(1, &mDepthID);
			glBindRenderbuffer(GL_RENDERBUFFER, mDepthID);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<int>(width),
				static_cast<int>(height));
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthID);
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mTexID, 0);
			unsigned DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
			glDrawBuffers(1, DrawBuffers);
			UnbindFramebuffer();
		}

		// ------------------------------------------------------------------------
		/*! Destructor
		*
		*   Prevents the FrameBuffers from Leaking when destroyed
		*/ //----------------------------------------------------------------------
		Framebuffer::~Framebuffer() noexcept {
			glDeleteFramebuffers(1, &mFbID);
			glDeleteTextures(1, &mTexID);
		}

		// ------------------------------------------------------------------------
		/*! Bind Texture
		*
		*   Binds the Framebuffer as a Texture to further sampling
		*/ //----------------------------------------------------------------------
		void Framebuffer::BindTexture() const noexcept {
			glBindTexture(GL_TEXTURE_2D, mTexID);
		}

		// ------------------------------------------------------------------------
		/*! Unbind Texture
		*
		*   Unbinds the Texture from the GPU
		*/ //----------------------------------------------------------------------
		void Framebuffer::UnbindTexture() const noexcept {
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		// ------------------------------------------------------------------------
		/*! Bind FrameBuffer
		*
		*   Binds the FrameBuffer to the GPU to be drawing target
		*/ //----------------------------------------------------------------------
		void Framebuffer::BindFramebuffer() const noexcept {
			glBindFramebuffer(GL_FRAMEBUFFER, mFbID);
		}

		// ------------------------------------------------------------------------
		/*! UnBind FrameBuffer
		*
		*   Unbinds the FrameBuffer from the GPU
		*/ //----------------------------------------------------------------------
		void Framebuffer::UnbindFramebuffer() const noexcept {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}
}