#include <Shared.h>

namespace Engine
{
	void TriggerBox::handle_event(const events::Event& event)
	{
		mHandler.handle(event);
	}

	void TriggerBox::trigger_volume(const OnCollisionStartedEvent&)
	{
		return;
	}

	void TriggerBox::BeginPlay()
	{
		EventDisp->subscribe(*this, events::type_of<OnCollisionStartedEvent>());
		EventDisp->subscribe(*this, events::type_of<OnCollisionPersistedEvent>());
		EventDisp->subscribe(*this, events::type_of<OnCollisionEndedEvent>());
	}
}
