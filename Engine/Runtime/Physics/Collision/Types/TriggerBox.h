
#ifndef GTriggerBox
#define GTriggerBox

namespace Engine
{
	struct OnCollisionStartedEvent;

	struct ENGINE_API TriggerBox: public Component, public events::Listener
	{

		void BeginPlay() override;

		template<typename T, typename EVENT>
		void AddEvent(T& who, void (T::* function)(const EVENT&))
		{
			mHandler.register_handler(who, function);
		}

		void handle_event(const events::Event& event);
		void trigger_volume(const OnCollisionStartedEvent& type);

		private:
			events::EventHandler mHandler;
	};
}

#endif