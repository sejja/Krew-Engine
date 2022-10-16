//
//	ChangeScene.h
//	Good Neighbours
//
//	Created by Biaggio Fillice on 30/09/21
//	Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _CHANGE_SCENE__H_
#define _CHANGE_SCENE__H_

namespace Engine {
	struct ENGINE_API ChangeScene : public Leaf {
#pragma region // Constructors & Destructors
		ChangeScene() noexcept;
#pragma endregion

#pragma region // Functions
		virtual void OnInitialize() noexcept override;
		virtual Status Update() noexcept override;
		virtual void OnTerminate() noexcept override;
#pragma endregion
	};
}

#endif // !LEAF_H_