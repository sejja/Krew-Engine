//
//	UI.cpp
//	Good Neighbours
//
//	Created by Adrian Montes on 29/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#include <Shared.h>
namespace Engine
{
	namespace UI
	{
		// ----------------------------------------------------------------------
		/*! HUD
		*
		*   Default hud constructor
		*/ //--------------------------------------------------------------------
		HUD::HUD()
		{
			GGfxPipeline->AddHUD(this);
		}
		// ----------------------------------------------------------------------
		/*! ~HUD
		*
		*   Default hud destructor
		*/ //--------------------------------------------------------------------
		HUD::~HUD()
		{
			GGfxPipeline->DelHUD(this);
			for (int i = 0; i < mWidgets.size(); i++)
				delete mWidgets[i];
		}
		// ----------------------------------------------------------------------
		/*! Display
		*
		*   Displays HUD on screen
		*/ //--------------------------------------------------------------------
		void HUD::Display() noexcept
		{
			mDisplayed = true;
		}
		// ----------------------------------------------------------------------
		/*! Hide
		*
		*   Hides HUD off screen
		*/ //--------------------------------------------------------------------
		void HUD::Hide() noexcept
		{
			mDisplayed = false;
		}
		// ----------------------------------------------------------------------
		/*! AddWidget
		*
		*   Adds a new widget to the hud
		*/ //--------------------------------------------------------------------
		void HUD::AddWidget(Widget* w) noexcept
		{
			w->SetHUD(this);
			mWidgets.push_back(w);
		}
		// ----------------------------------------------------------------------
		/*! Update
		*
		*   Updates HUD widgets
		*/ //--------------------------------------------------------------------
		void HUD::Update() const noexcept
		{
			for (auto it : mWidgets)
				it->Update();
		}
		// ----------------------------------------------------------------------
		/*! Render
		*
		*   Renders widgets
		*/ //--------------------------------------------------------------------
		void HUD::Render() const noexcept
		{
			if (!mDisplayed)
				return;

			for (auto it : mWidgets)
				it->Render();
		}
		// ----------------------------------------------------------------------
		/*! IsDisplayed
		*
		*   Gets visual state of HUD
		*/ //--------------------------------------------------------------------
		bool HUD::IsDisplayed() const noexcept
		{
			return mDisplayed;
		}
		// ----------------------------------------------------------------------
		/*! GetWidget
		*
		*   Returns the widget of the given name
		*/ //--------------------------------------------------------------------
		//Widget* HUD::GetWidget(const char* name) const noexcept
		//{
		//	for (auto it : mWidgets)
		//	{
		//		if (strcmp(it->GetName(), name))
		//			return it;
		//	}
		//}
		// ----------------------------------------------------------------------
		/*! GetWidgets
		*
		*   Gets all the HUD widgets
		*/ //--------------------------------------------------------------------
		Array<Widget*> HUD::GetWidgets() const noexcept
		{
			return mWidgets;
		}
	}
}
