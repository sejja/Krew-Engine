#ifndef _ANIM_STATE_MACH__H_
#define _ANIM_STATE_MACH__H_


namespace Engine {
	
	struct StateMachine;
	struct Actor;

	//! \class State
	//! \brief Represents a state in a state machine. This class is meant to be overwritten. 
	struct ENGINE_API State
	{
		// ------------------------------------------------------------------------
		// DATA

		//! Name of the state.
		std::string mName;

		//! Pointer to the state machine that owns this state.
		StateMachine* mOwnerStateMachine;

		//! Pointer to the game object that owns the state machine.
		Actor* mActor;

		//! Specifies how long this state has been active.
		float mTimeInState;

		// ------------------------------------------------------------------------
		// METHODS

		//! Sets name to specified name
		State(const char* name);

		// !Destructor. Doesn't do anything, needed for inherited classes
		virtual ~State() {}

		//! Default Enter method. Does nothing as it is meant to be overriden by derived classes. 
		virtual void LogicEnter() {}

		//! Default Exit method. Does nothing as it is meant to be overriden by derived classes. 
		virtual void LogicExit() {}

		//! Default Update method. Does nothing as it is meant to be overriden by derived classes. 
		virtual void LogicUpdate() {}

		//! Internal version of Enter called by state machine.
		virtual void InternalEnter();

		//! Internal version of Exit called by state machine.
		virtual void InternalExit();

		//! Internal version of Update called by state machine.
		virtual void InternalUpdate();
	};

	//! \class StateMachine
	//! \brief Represents a finite state machine used for implementing complex behaviors.
	struct ENGINE_API StateMachine
	{
		// ------------------------------------------------------------------------
		// DATA

		//! State container. This class owns the memory for the states and thus should free it when destroyed. 
		std::vector<State*> mStates;

		//! Pointer to the game object that owns the state machine.
		Actor* mActor;

		//! Pointer to the current active state in the state machine. 
		State* mCurrentState;

		//! Pointer to the next active state. Same as mCurrentState unless state change is requested. 
		State* mNextState;

		//! Pointer to the initial state of the state machine. This is the active state when the state  machine is reset. 
		State* mInitialState;

		// ------------------------------------------------------------------------
		// METHODS

		//! \brief	Sets all state machine variables to NULL (note: they are all pointers).
		StateMachine();

		//! \brief	Sets the actor and the rest of variables to NULL (note: they are all pointers).
		StateMachine(Actor* actor);

		//! Clears all the states (hint: calls Clear function below)
		~StateMachine();

		//
		// CONTAINER MANAGEMENT
		//
		//! Adds a new state to the state machine.
		void AddState(State* state);

		//! Removes a State from the state machine.
		void RemoveState(State* state);

		//! Removes all the states from the state machine and sets the state pointers (current, next, initial) to NULL.
		void Clear();

		//! Finds a state by name
		State* GetState(const char* stateName);

		//! Sets the initial state to the given pointer.
		void SetInitState(State* state);

		//! Sets the initial state to one state whose name matches the specified one.
		void SetInitState(const char* stateName);

		//! Changes state to the specified state. 
		void ChangeState(State* state);

		//! Changes state to the specified state by name. 
		void ChangeState(const char* stateName);

		//! Update of the state machine. Meant to be called every frame.
		void Update();

		//! Resets the state machine. 
		void Reset();
	};

	// @TODO
	//! \class SuperState
	//! \brief State that acts as a state machine. Allows nested state machines.
	struct ENGINE_API SuperState : public State
	{
		// ------------------------------------------------------------------------
		// DATA

		//! Internal state machine. InternalUpdate calls Update
		StateMachine mInternalStateMachine;

		// ------------------------------------------------------------------------
		// METHODS

		//! Sets the name and actor of the super state
		SuperState(const char* name, Actor* actor);

		//! Resets the internal state machine and calls the base InternalEnter.
		virtual void InternalEnter();

		//! Updates the internal state machine and calls the base InternalUpdate();
		virtual void InternalUpdate();
	};

	//! \class Actor
	//! \brief Simple GameObject structure that supports state machines. 

	struct ENGINE_API Actor : public Component
	{
	public:
		//! Use a vector to support concurrent state machine
		std::vector<StateMachine> mBrain;
	
		//! Adds one state machine to be used by default.
		Actor()
		{
			// add default state machine
			mBrain.push_back(StateMachine(this));
		}
	
		//! Updates all state machine sequentially.
		virtual void Update()
		{
			for (size_t i = 0; i < mBrain.size(); ++i)
				mBrain[i].Update();
		}
	};
}


#endif