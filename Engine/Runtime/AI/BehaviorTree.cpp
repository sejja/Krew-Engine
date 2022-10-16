//
//    BehaviorTree.cpp
//    Good Neighbours
//
//    Created by Biaggio Fillice on 30/09/21
//    Copyright © 2021 Diego Revilla. All Rights reserved
//

#include "Shared.h"

namespace Engine {
    // ------------------------------------------------------------------------
    /*! Default Constructor
    *
    *   Initialize default variables
    */ // ---------------------------------------------------------------------
    Behavior::Behavior() :
        mStatus{ Status::INVALID }, mOwner(nullptr) {}

    // ------------------------------------------------------------------------
    /*! Tick
    *
    *   Calls the proper functions depending on the Behavior State
    */ // ---------------------------------------------------------------------
    void Behavior::Tick() {
        // this tick updates, initializes, and terminates the node based on its thing
        if (mStatus != Status::RUNNING) OnInitialize();
        mStatus = Update();
        if (mStatus != Status::RUNNING) OnTerminate();
    }
    Behavior::Status Behavior::GetStatus() const
    {
        return mStatus;
    }
    // ------------------------------------------------------------------------
    /*! On Initialize
    *
    *   EMPTY FUNCTION
    */ // ---------------------------------------------------------------------
    void Behavior::OnInitialize() {}

    // ------------------------------------------------------------------------
    /*! On Terminate
    *
    *   EMPTY FUNCTION
    */ // ---------------------------------------------------------------------
    void Behavior::OnTerminate() {}

    // ------------------------------------------------------------------------
    /*! Add Tree
    *
    *   Adds a Behavior to this tree
    */ // ---------------------------------------------------------------------
    void BehaviorTree::AddTree(Behavior* const tree) noexcept {
        mTree = tree;
        tree->mOwner = this;
    }
    // ------------------------------------------------------------------------
    /*! Default Constructor
    *
    *   Initializes the tree to an empty tree
    */ // ---------------------------------------------------------------------
    BehaviorTree::BehaviorTree() noexcept :
        mTree(nullptr) {}

    // ------------------------------------------------------------------------
    /*! Conversion Constructor
    *
    *   Initializes the tree with a precreated behavior
    */ // ---------------------------------------------------------------------
    BehaviorTree::BehaviorTree(Behavior* const tree) noexcept
        : mTree(tree) {}

    // ------------------------------------------------------------------------
    /*! Free Behavior
    *
    *   Destroys and frees a behavior
    */ // ---------------------------------------------------------------------
    void BehaviorTree::FreeBehavior(Behavior* const bh) {
        PageAllocator<Behavior> mAlloc;
        mAlloc.destroy(bh);
        mAlloc.deallocate(bh);
    }

    DONTDISCARD Component* BehaviorTree::GetOwner() const noexcept
    {
        return mComp;
    }
    void BehaviorTree::SetOwner(Component* comp)
    {
        mComp = comp;
    }
    void BehaviorTree::Reset()
    {
        mTree->mStatus = Behavior::Status::FAIL;
    }
}