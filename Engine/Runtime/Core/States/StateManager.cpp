#include <Shared.h>

namespace Engine {
	void StateManager::Update()
	{
		// if the new level doesn't match the current level
		if (mNewLevel != mCurrentLevel || mReload)
		{
			// get the level through a json
			nlohmann::json j_;
			std::fstream sf_(mNewLevel);

			//if we could open the file
			if (sf_.good())
				sf_ >> j_;

			// set the world's scene and make the current level equal to the new level
			GWorld->GetScene()->FromJson(j_);
			mCurrentLevel = mNewLevel;
			mReload = false;
		}
	}

	void StateManager::SetNewLevel(std::string const& filename)
	{
		mNewLevel = filename;
	}

	void StateManager::ReloadLevel()
	{
		mReload = true;
	}
}
