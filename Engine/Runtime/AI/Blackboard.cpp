#include <Shared.h>

namespace Engine {

	OnMessageEvent::OnMessageEvent()
	{
	}
	OnMessageEvent::~OnMessageEvent(){}
	
	Blackboard::Blackboard() 
	{
		EventDisp->subscribe(*this, events::type_of<OnMessageEvent>());
	}
	void Blackboard::handle_event(const events::Event&){}
}