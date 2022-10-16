#ifndef BLACKBOARD_H_
#define BLACKBOARD_H_

namespace Engine {
	class ENGINE_API OnMessageEvent : public events::Event
	{
	public:
		OnMessageEvent();
		~OnMessageEvent();
	};

	class ENGINE_API Blackboard : public events::Listener
	{
	public:
		Blackboard();
		virtual void handle_event(const events::Event&);
	protected:
	};
	
}

#endif // !BLACKBOARD_H_

