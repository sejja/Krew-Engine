//
//	GraphicsManager.cpp
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#include "Shared.h"

namespace Engine {
	// ------------------------------------------------------------------------
	/*! Initialize
	*
	*   Initializes the Graphics Manager
	*/ // ---------------------------------------------------------------------
	void GraphicsManager::Initialize(const HWND& hwnd) noexcept
	{
		mDeviceContext = 0;
		mRenderContext = 0;

		PIXELFORMATDESCRIPTOR _pfd;

		memset(&_pfd, NULL, sizeof(_pfd));
		mDeviceContext = GetDC(hwnd);
		_pfd.nSize = sizeof(_pfd);
		_pfd.nVersion = 1;
		_pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		_pfd.iPixelType = PFD_TYPE_RGBA;
		_pfd.cColorBits = 32;
		_pfd.cDepthBits = 32;
		_pfd.iLayerType = PFD_MAIN_PLANE;
		::SetPixelFormat(mDeviceContext, ::ChoosePixelFormat(mDeviceContext, &_pfd), &_pfd);
		mRenderContext = wglCreateContext(mDeviceContext);

		// Create fake context
		if (!mRenderContext) {
			ReleaseDC(hwnd, mDeviceContext);
			mDeviceContext = 0;

			abort();
		}

		wglMakeCurrent(mDeviceContext, mRenderContext);
		glewExperimental = GL_TRUE;
		GLenum _err = glewInit();

		//If glew failed
		if (GLEW_OK != _err) {
			fprintf(stderr, "Error: %s\n", glewGetErrorString(_err));

			abort();
		}

		int _attribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 3,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			NULL
		};

		// Check that GLEW is supported
		if (wglewIsSupported("WGL_ARB_create_context") == 1) {
			// Create a new context and delete the temporary one
			HGLRC temp = mRenderContext;
			mRenderContext = wglCreateContextAttribsARB(mDeviceContext, 0,
				_attribs);
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(temp);
			wglMakeCurrent(mDeviceContext, mRenderContext);
		}

		char _strError[20];

		sprintf_s(_strError, "GL_VERSION_%d_%d", 3, 3);

		if (!glewIsSupported(_strError)) {
			char sErrorMessage[255], sErrorTitle[255];

			sprintf_s(sErrorMessage, "OpenGL %d.%d is not supported! Please download latest GPU drivers!",
				3, 3);
			sprintf_s(sErrorTitle, "OpenGL %d.%d Not Supported",
				3, 3);
			MessageBox(hwnd,
				sErrorMessage,
				sErrorTitle,
				MB_ICONINFORMATION);

			abort();
		}
		// enable depth buffering & back-face removal
		GGfxPipeline->Initialize();
	}

	// ------------------------------------------------------------------------
	/*! Load
	*
	*   Loads the default Assets
	*/ // ---------------------------------------------------------------------
	void GraphicsManager::Load() noexcept
	{
		GGfxPipeline->Load();

		//{ //Floor
		//	GameObject* go = new GameObject;
		//	MeshRenderer* r = new MeshRenderer;
		//	go->AddComp(r);
		//	r->SetMesh(GGfxPipeline->objLoader->meshes["plane"]);
		//	r->SetMatrix(
		//		glm::translate(glm::mat4(1), { 0,-5,0 }) *
		//		glm::inverse(glm::lookAt(glm::vec3(0), glm::vec3(0, -1, 0), glm::vec3(0, 0, 1))) *
		//		glm::scale(glm::mat4(1), glm::vec3(50))
		//	);
		//}
		//{ //Cube
		//	GameObject* go = new GameObject;
		//	MeshRenderer* r = new MeshRenderer;
		//	go->AddComp(r);
		//	r->SetMesh(GGfxPipeline->objLoader->meshes["sphere"]);
		//	r->SetMatrix(
		//		glm::scale(glm::mat4(1), glm::vec3(5))
		//	);
		//}
	}

	// ------------------------------------------------------------------------
	/*! Render
	*
	*   Renders the Scene onto the main buffer
	*/ // ---------------------------------------------------------------------
	void GraphicsManager::Render() noexcept
	{
		GGfxPipeline->Render();
	}

	// ------------------------------------------------------------------------
	/*! Unload
	*
	*   Unloads the default assets
	*/ // ---------------------------------------------------------------------
	void GraphicsManager::Unload() const noexcept
	{

	}

	// ------------------------------------------------------------------------
	/*! Present
	*
	*   Swaps the Back and View Buffer
	*/ // ---------------------------------------------------------------------
	void GraphicsManager::Present() const noexcept
	{
		SwapBuffers(mDeviceContext);
	}
}