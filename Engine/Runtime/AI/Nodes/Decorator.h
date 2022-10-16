//
//	Composite.h
//	Good Neighbours
//
//	Created by Biaggio Fillice on 30/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _DECORATOR__H_
#define _DECORATOR__H_

namespace Engine {
	struct ENGINE_API Decorator : Behavior {
#pragma region // Constructors & Destructors
		Decorator() noexcept;
		virtual ~Decorator();
#pragma endregion

#pragma region //Functions
		void AddChild(Behavior* const state) noexcept;
#pragma endregion

#pragma region // Members
	protected:
		Behavior* mChild;
#pragma endregion
	};
}

#endif // !DECORATOR_H_