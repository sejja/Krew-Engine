//
//	Composite.h
//	Good Neighbours
//
//	Created by Biaggio Fillice on 30/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _COMPOSITE__H_
#define _COMPOSITE__H_

namespace Engine {
	struct ENGINE_API Composite : Behavior {
#pragma region // Constructors & Destructors
		Composite() noexcept;
		~Composite();
		Composite& operator=(const Composite&) = delete;
#pragma endregion

#pragma region // Functions
		void AddChild(Behavior* const behavior);
		void RemoveChild(Behavior* const behavior);
		Array<Behavior*> GetChildren() const;
#pragma endregion

#pragma region //Members
	protected:
		Array<Behavior*> mChildren;
#pragma endregion
	};
}

#endif // !COMPOSITE_H_