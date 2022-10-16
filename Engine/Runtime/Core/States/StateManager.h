#ifndef STATEMANAGER_H_
#define STATEMANAGER_H_

namespace Engine {
    class StateManager : public Singleton<StateManager> {
    public:
        void Update();
        //void BeginPlayQueue();
        //GetCurrentScene() ->Std::string
        void SetNewLevel(std::string const& filename);
        void ReloadLevel();
    private:
        std::string mNewLevel;
        std::string mCurrentLevel;
        bool mReload{false};
        //std::stack<IBase*> mBeginQueue;
    };
}

#define GStateManager (&Engine::StateManager::Instance())

#endif // !STATEMANAGER_H_
