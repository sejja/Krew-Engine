#include "Shared.h"

namespace Engine {
	/*!
	\brief	Sets all state machine values to NULL (note: they are all pointers).
	*/
	StateMachine::StateMachine()
	{
		mActor = nullptr;
		mCurrentState = nullptr;
		mInitialState = nullptr;
		mNextState = nullptr;
	}


	/*!
	\brief	Sets the actor and the rest of variables to NULL (note: they are all pointers).
	*/
	StateMachine::StateMachine(Actor* actor)
	{
		mActor = actor;
		mCurrentState = nullptr;
		mInitialState = nullptr;
		mNextState = nullptr;
	}


	/*!
	\brief		Clears all the states.
	\details	You should only call Clear() and avoid redundant call
	*/
	StateMachine::~StateMachine()
	{
		Clear();
	}


	/*!
	\brief	Adds a new state to the state machine.

	Implementation Details:
		- Sanity check 1: state is not NULL
		- Sanity check 2: state name isn't duplicated
		- You must set the mActor and mOwnerStateMachine for the new state in this function.
	*/
	void StateMachine::AddState(State* state)
	{
		if (state == nullptr) //Sanity check
			return;
		// Go iterating through states list
		for (auto it = mStates.begin(); it != mStates.end(); it++)
			if ((*it) == state) // if we find the one that we were looking 
				return;
		//Give to the state passed as parameter the values of "this"
		state->mActor = mActor;
		state->mOwnerStateMachine = this;
		mStates.push_back(state); // add the state in the list
	}


	/*!
	\brief	Removes a State from the state machine.

	Implementation Details:
		- Sanity check 1: state is not NULL
		- You must delete the state memory (it's assumed to have been
		allocated previously).
		- You should call the erase() method on the mStates vector.
	*/
	void StateMachine::RemoveState(State* state)
	{
		if (state == nullptr) // Sanity check
			return;
		// Go iterating through states list
		for (auto it = mStates.begin(); it != mStates.end();)
			if (state == (*it))  // if we find the one that we were looking for
			{
				mStates.erase(it); // We erase it
				return;
			}
			else
				it++;
	}


	/*!
	\brief	Removes all the states from the state machine and sets the
	state pointers (current, next, initial) to nullptr.

	\details	Remember to delete the memory for the states.
	*/
	void StateMachine::Clear()
	{
		State* mystate; //Create a pointer to a state

		while (!mStates.empty()) // while the list is not empty
		{
			mystate = mStates.back(); // keep the last state in our pointer
			mStates.pop_back(); // Call pop back
			delete mystate; //Delete our pointer
		}

		mCurrentState = nullptr;
		mInitialState = nullptr;
		mNextState = nullptr;
	}


	/*!
	\brief	Finds a state by name

	\return	A pointer to the state if name matches, nullptr otherwise.
	*/
	State* StateMachine::GetState(const char* stateName)
	{
		if (stateName == nullptr) // Sanity check
			return nullptr;
		// Go iterating through states list
		for (auto it = mStates.begin(); it != mStates.end(); it++)
			if ((*it)->mName == stateName) // if we find the one that we were looking for
				return (*it);
		return nullptr;
	}

	/*!
	\brief	Sets the initial state to the given pointer.
	\details	You only modify mInitialState IF AND ONLY IF 'state'
	exists in the mStates vector. (i.e., it's been added to state
	machine prior to calling this function.
	*/
	void StateMachine::SetInitState(State* state)
	{
		// Go iterating through states list
		for (auto it = mStates.begin(); it != mStates.end(); it++)
			if ((*it) == state) // if we find the one that we were looking for
				mInitialState = state; //Set the state to the initial state
	}


	/*!
	\brief	Sets the initial state to one state whose name matches the specified one.
	\details	This function should re-use other functions to do its work.
	*/
	void StateMachine::SetInitState(const char* stateName)
	{
		auto it = GetState(stateName); //Call get state and keep the return in a pointer
		mInitialState = it;	// Set as initialstate what the pointer stores

	}


	/*!
	 \brief	Changes state to the specified state.
	 \details
		- Note that the state change only takes effect in StateMachine::Update.
		This only modifies the value of mNextState.
		- Sanity check 1: state is not nullptr
		- Sanity check 2: state exists in mStates.
	*/
	void StateMachine::ChangeState(State* state)
	{
		if (state == nullptr) //Sanity check
			return;
		// Go iterating through states list
		for (auto it = mStates.begin(); it != mStates.end(); it++)
			if ((*it) == state)// if we find the one that we were looking for
			{
				mNextState = state; // Set the next to the state passed as parameter
				return;
			}
	}

	/*!
	 \brief	Changes state to the specified state by name.
	 \details	This function should re-use other functions to do its work.
	*/
	void StateMachine::ChangeState(const char* stateName)
	{
		auto it = GetState(stateName); //Call get state and keep the return in a pointer
		mNextState = it;  // Set the next to the state got from GetState function
	}

	/*!
	 \brief	Update of the state machine. Meant to be called every frame.
	 \details
		- The function should call only call the "Internal...()" functions
			on the current state.
		- This function also performs the state change. This happens when
			mCurrentState != mNextState.
		- when changing state, you must call InternalExit and InternalEnter
			on the old state and new state, respectively.
	*/
	void StateMachine::Update()
	{
		if (mCurrentState) // Sanity check
		{
			mCurrentState->InternalUpdate(); // call internal update of the current
			if (mCurrentState != mNextState) // if we do perform state change
			{
				mCurrentState->InternalExit(); //Call internal exit for the current
				mNextState->InternalEnter(); // Call internal enter for the next one

				mCurrentState = mNextState;
			}
		}
	}

	/*!
	\brief		Resets the state machine.
	\details
	- Calls exit on the current state if valid.
	- Changes the current state to the initial state
	- Calls enter on the new current state
	*/
	void StateMachine::Reset()
	{
		if (mCurrentState) //Sanity check
		{
			mCurrentState->InternalExit(); // call internal exit for the current state
		}

		mCurrentState = mInitialState; // set the current as initial
		mNextState = mInitialState; // set next to initial
		mCurrentState->InternalEnter(); // call internal enter on current
	}

	// ----------------------------------------------------------------------------
	// STATE
	// ----------------------------------------------------------------------------

	/*!
	 \brief	Sets the default values of the state.

	 Implementation Details:
		- mName is set to the specified name
		- all other values are set to nullptr.
	*/
	State::State(const char* name)
	{
		if (name == nullptr) //sanity check
		{
			mName = "unnamed";
		}
		else
		{
			mName = name; // give to mName the char passed as parameter
		}

		//all other values are set to nullptr. 
		mOwnerStateMachine = nullptr;
		mActor = nullptr;
		mTimeInState = 0.0f;
	}


	// @TODO
	/*! ---------------------------------------------------------------------------
	 \brief	Called by the state machine when switchin to this state.

	 \details
		- Sets mTimeState = 0.0f
		- Calls LogicEnter().
	*/
	void State::InternalEnter()
	{
		mTimeInState = 0.0f;
		LogicEnter();

	}

	// @TODO
	/*!
	 \brief	Called by the state machine when switchin out of this state.

	 Implementation Details: Only calls LogicExit().
	*/
	void State::InternalExit()
	{
		LogicExit();
	}

	// @TODO
	/*!
	 \brief	Called by the state machine while it's in this state.

	 Implementation Details:
		- Increments mTimeInState by gAEFrameTime
		- Calls LogicUpdate();
	*/
	void State::InternalUpdate()
	{
		mTimeInState += GFrames->GetFrameTime();
		LogicUpdate();
	}

	// ----------------------------------------------------------------------------
	// SUPER STATE (STATE AS A STATE MACHINE)
	// ----------------------------------------------------------------------------

	// @TODO
	/*!
	\brief	Sets the name and actor of the super state

	Implementation Details:
		- Call the base State constructor
		- Also sets the internal state machine mActor variable.
	*/
	SuperState::SuperState(const char* name, Actor* actor) : State(name)
	{
		mInternalStateMachine.mActor = actor; // set the internal state machines actor variable
		mActor = actor; //sets the mactor variable

	}

	// @TODO
	/*!
	 \brief	Resets the internal state machine and calls the base InternalEnter.

	 Implementation Details:
		- Before doing anything, you must set the mActor variable for EVERY
		state in the mInternalStateMachine.mStates vector equal to this state's
		mActor variable.
		- the internal state machine must be reset.
		- Don't forget to call State::InternalEnter to inherit the default
		behavior of the state. (this is also what calls LogicEnter()).
	*/
	void SuperState::InternalEnter()
	{
		// Go iterating through internal state machine  list
		for (auto it = mInternalStateMachine.mStates.begin(); it != mInternalStateMachine.mStates.end(); it++)
		{
			(*it)->mActor = mActor; //et the mActor variable for EVERY state
		}

		mInternalStateMachine.Reset(); //the internal state machine must be reset
		State::InternalEnter();
	}


	// @TODO
	/*!
	\brief	Updates the internal state machine first, then calls State::InternalUpdate()
	*/
	void SuperState::InternalUpdate()
	{
		mInternalStateMachine.mInitialState->InternalUpdate(); //Updates the internal state machine
		State::InternalUpdate();
	}
}