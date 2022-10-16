#pragma once

namespace events
{   
    class ENGINE_API Listener
    {
        public:
        Listener(){}; // default constructor
        virtual ~Listener() = 0;    //destructor
        virtual void handle_event(const Event&){}; //handle specific event (it is going to be overrided)

        private:
    };

    class ENGINE_API EventDispatcher : public Singleton<EventDispatcher>
    {

        public:

        //subscribe the passed object ("who") to a specific type of listener or event
        void subscribe(Listener& who,const TypeInfo what);

        //unsubscribe passed object from event of type what
        void unsubscribe(Listener& who,const TypeInfo what);

        //clear all events and subscribers
        void clear();

        //trigger specific event and have subscribers react
        void trigger_event(const Event& event);

        //overload of << operrator for printing purposes
        friend std::ostream& operator<<(std::ostream&, EventDispatcher& dispatcher);

        //declaration of trigger of the dispatcher from the outside
        friend void trigger_event(const Event& event);

        private:

        std::map<TypeInfo, std::vector<Listener*> > event_collection; 
    };

    //trigger event (no need to reference the dispatcher directly; cleaner code)
    void trigger_event(const Event& event);

}

#define EventDisp (&events::EventDispatcher::Instance())