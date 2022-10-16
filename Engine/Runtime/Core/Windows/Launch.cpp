//
//	Launch.cpp
//	Good Neighbours
//
//	Created by Diego Revilla 15/09/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>
#include "Tests/Tests.h"

int main(int argc, char**) {

	if (argc > 1) {
		do_tests();
	}

	GContent->Initialize();
	GWindow->Initialize();
	GFrames->SetMaxFrameRate(60);
	(void)GAudio;
	GGfxMan->Initialize(GWindow->GetHandle());
#ifdef _EDITOR_
	GEditor->Initialize();
#endif
	GGfxMan->Load();
	GFactory->RegisterAllTypes();
	GWorld->SetScene(new Engine::Scene);
	GBinaryLoader->Bottle("Source.dll");
	{
		//Engine::Threading::ThreadPool tp(3);

#ifndef _EDITOR_

		GStateManager->SetNewLevel("EngineContent/StartScreen.level");
		GStateManager->Update();

#endif // !_EDITOR_

		while (GWindow->Exists()) {

#ifdef _EDITOR_
			GEditor->ShowGUI();
#endif

			GFrames->Update();
			GObjectMgr->InitializeDispatch();
			GInput->ProcessInput();
			const auto _fcol = []() {
#ifdef _EDITOR_
				auto t1 = std::chrono::high_resolution_clock::now();
#endif
				Engine::gCollisionSystem->Update();
#ifdef _EDITOR_
				auto t2 = std::chrono::high_resolution_clock::now();

				/* Getting number of milliseconds as a double. */
				std::chrono::duration<double, std::milli> ms_double = t2 - t1;

				GFrames->PhysicsTime() = ms_double.count();
#endif
			};

			GAudio->Update();

			const auto _fupdate = []() {
#ifdef _EDITOR_
				auto t1 = std::chrono::high_resolution_clock::now();
#endif
				GWorld->UpdateScene();
#ifdef _EDITOR_
				auto t2 = std::chrono::high_resolution_clock::now();

				/* Getting number of milliseconds as a double. */
				std::chrono::duration<double, std::milli> ms_double = t2 - t1;

				GFrames->LogicTime() = ms_double.count();
#endif
			};

			_fupdate();

#ifdef _EDITOR_
			if (!GEditor->IsPaused() && GEditor->IsPlaying()) {
#endif
				//tp.AddTask(_fcol);
				_fcol();
				//tp.AddTask(_faud);
#ifdef _EDITOR_
			}

			//tp.AddTask(_fupdate);
#endif

			auto t1 = std::chrono::high_resolution_clock::now();

#ifdef _EDITOR_
			if (!(GEditor->IsPlaying() && !GEditor->IsViewport())) {
				glBindFramebuffer(GL_FRAMEBUFFER, GEditor->GetBuffer());
				GGfxMan->Render();
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
			else {
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				GGfxMan->Render();
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
#else
			GGfxMan->Render();
#endif

			// Weird way to solve wrong GameObject sync
			auto scene = GWorld->GetScene();
			int count = scene->GetObjectCount();

			GGfxMan->Present();

			auto t2 = std::chrono::high_resolution_clock::now();

			/* Getting number of milliseconds as a double. */
			std::chrono::duration<double, std::milli> ms_double = t2 - t1;

			GFrames->GraphicsTime() = ms_double.count();

			GWindow->Update();

#ifdef _EDITOR_
			if (!GEditor->IsPaused())
#endif
				GStateManager->Update();
			Engine::GCollector::Instance().GOGarbageCollection();
			GContent->Update();
		}
	}
#ifdef _EDITOR_
	GEditor->Shutdown();
#endif
	GWorld->SetScene(nullptr);
	GBinaryLoader->Unbottle();
	GGfxMan->Unload();
	Engine::gCollisionSystem->Shutdown();
	GContent->ShutDown();
	GFactory->ShutDown();
	GWindow->Close();
#ifdef _DEBUG
	GMemoryDbg->TerminateMemoryDebugging();
#endif
}