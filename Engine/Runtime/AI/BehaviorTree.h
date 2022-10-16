//
//    BehaviorTree.h
//    Good Neighbours
//
//    Created by Biaggio Fillice on 30/09/21
//    Copyright © 2021 Diego Revilla. All Rights reserved
//

#ifndef _BEHAVIOR_TREE__H_
#define _BEHAVIOR_TREE__H_

namespace Engine {
    class BehaviorTree;

    struct ENGINE_API Behavior : public IBase {
#pragma region // Declarations
        friend class BehaviorTree;
        enum class Status : unsigned char { INVALID, RUNNING, FAIL, SUCCESS };
#pragma endregion

#pragma region // Constructors & Destructors
        Behavior();
#pragma endregion

#pragma region // Functions
        void Tick();
        DONTDISCARD BehaviorTree inline* GetOwner() const noexcept;
        void inline SetOwner(BehaviorTree* const tree) noexcept;
        Status GetStatus() const;
    protected:
        virtual void OnInitialize();
        virtual Status Update() = 0;
        virtual void OnTerminate();
#pragma endregion

#pragma region // Members
        Status mStatus;
        BehaviorTree* mOwner;
#pragma endregion
    };

    class ENGINE_API BehaviorTree {
#pragma region // Declarations
        friend struct Behavior;
#pragma endregion

#pragma region // Constructors & Destructors
    public:
        BehaviorTree() noexcept;
        BehaviorTree(Behavior* const tree) noexcept;
#pragma endregion

#pragma region // Functions
        virtual void inline Tick();
        virtual void inline Destroy();
        //virtual void OnEditor();
        void AddTree(Behavior* const tree) noexcept;
        void FreeBehavior(Behavior* const bh);
        template<typename Ty, typename...Args>
        Ty* SpawnBehavior(Args...args);
        DONTDISCARD Component* GetOwner() const noexcept;
        void SetOwner(Component* comp);
        Behavior* GetTree() const { return mTree; }
        Blackboard* GetBlackboard() { return mBlackboard; }
        void SetBlackboard(Blackboard* bb) { mBlackboard = bb; }
        void Reset();
#pragma endregion

#pragma region // Members
    protected:
        Behavior* mTree;
        Component* mComp;
        Blackboard* mBlackboard;
#pragma endregion
    };

    // ------------------------------------------------------------------------
    /*! Get Owner
    *
    *   Returns the Behavior Tree Owner
    */ // ---------------------------------------------------------------------
    BehaviorTree inline* Behavior::GetOwner() const noexcept {
        return mOwner;
    }

    // ------------------------------------------------------------------------
    /*! Set Owner
    *
    *   Sets the Behavior Owner
    */ // ---------------------------------------------------------------------
    void inline Behavior::SetOwner(BehaviorTree* const bt) noexcept {
        mOwner = bt;
    }

    // ------------------------------------------------------------------------
    /*! Tick
    *
    *   Update the Entire Tree
    */ // ---------------------------------------------------------------------
    void inline BehaviorTree::Tick() {
        // tick to start the entire tree
        mTree->Tick();
    }
    // ------------------------------------------------------------------------

    /*! Default Constructor
    *
    *   Initializes the tree to an empty tree
    */ // ---------------------------------------------------------------------
    void inline BehaviorTree::Destroy() {
        FreeBehavior(mTree);
    }

    // ------------------------------------------------------------------------
    /*! Spawn Behavior
    *
    *   Allocates and Constructs a new Behavior
    */ // ---------------------------------------------------------------------
    template<typename Ty, typename...Args>
    Ty* BehaviorTree::SpawnBehavior(Args...args) {
        PageAllocator<Ty> mAlloc;
        auto mem = mAlloc.allocate();
        mAlloc.construct(mem, args...);
        return mem;
    }
}

#endif