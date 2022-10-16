//
//	UI.h
//	Good Neighbours
//
//	Created by Adrian Montes on 29/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _UI_H_
#define _UI_H_

namespace Engine
{
	namespace UI
	{
		class Widget;
		class ENGINE_API HUD
		{
		public:
#pragma region // Functions
			HUD();
			~HUD();
			HUD& operator=(const HUD&) = delete;
			void Display() noexcept;
			void Hide() noexcept;
			void AddWidget(Widget* w) noexcept;
			void Update() const noexcept;
			void Render() const noexcept;
#pragma endregion

#pragma region // Gettors
			bool IsDisplayed() const noexcept;
			//Widget* GetWidget(const char* name) const noexcept;
			Array<Widget*> GetWidgets() const noexcept;
			template <typename T> Array<T*> GetWidgetsOfType() const noexcept;
#pragma endregion
		private:
			Array<Widget*> mWidgets;
			bool mDisplayed;
		};
		// ----------------------------------------------------------------------
		/*! GetWidgetsOfType
		*
		*   Given a type of widget, returns an array of only widgets of that type
		*/ //--------------------------------------------------------------------
		template<typename T>
		inline Array<T*> HUD::GetWidgetsOfType() const noexcept
		{
			Array<T*> r; T* tmp;

			//Loop through all the widgets and get the ones of the type
			for (auto it : mWidgets)
			{
				if(strcmp(typeid(tmp).name(), typeid(it).name()) == 0)
					r.push_back(it);
			}

			//Return array of widgets
			return r;
		}
	}
}
#endif // !_UI_H_

