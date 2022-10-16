#include <shared.h>


namespace events
{


    /******************************************************************************/
    /*!
    \brief
        listener destructor
    */
    /******************************************************************************/
    Listener::~Listener()
    {

    }



    /******************************************************************************/
    /*!
    \brief
        link the object to the event (so that when it is triggered, it reacts)
    \param who
        object to subscribe
    \param what
        event to subscribe to
    */
    /******************************************************************************/
    void EventDispatcher::subscribe(Listener& who,const TypeInfo what)
    {

        //link the event and the listener to that event
        event_collection[what].push_back(&who);

    }

    /******************************************************************************/
    /*!
    \brief
        unlink the object and the event 
    \param who
        object to unsubscribe
    \param what
        event from which to unsubscribe
    */
    /******************************************************************************/
    void EventDispatcher::unsubscribe(Listener& who,const TypeInfo what)
    {
        //find iterator starting at key "what"
        std::map<TypeInfo, std::vector<Listener*> >::iterator it = event_collection.find(what); 

        //iterator for the vector
        std::vector<Listener*>::iterator it_2;

        //iterate through the vector
        while(it_2 != it->second.end())
        {
            //if it's the same kind of listener (sanity check, should be)
           if(typeid(**it_2).name() == typeid(who).name())
           {

               //remove it from list
               (*it).second.erase(it_2);


           }
        }
        
    }

    /******************************************************************************/
    /*!
    \brief
        clear all subscribed objects and all events subscribed to
    */
    /******************************************************************************/
    void EventDispatcher::clear()
    {
        //iterate throught the listener vectors clearing them
        std::map<TypeInfo, std::vector<Listener*> >::iterator it = event_collection.begin();
        while(it != event_collection.end())
        {
            it->second.clear();
            it++;
        }
        //clear the event collection
        event_collection.clear();

    }

    /******************************************************************************/
    /*!
    \brief
        trigger an event and make each subscriber to it react
    \param event
        event to trigger
    */
    /******************************************************************************/
    void EventDispatcher::trigger_event(const Event& event)
    {

        //iterator for the map
        std::map<TypeInfo, std::vector<Listener*> >::iterator it = event_collection.find(event);

        //iterator for the vectors inside
        std::vector<Listener*>::iterator it_2 = it->second.begin();
        while(it_2 != it->second.end())
        {
            (*it_2)->handle_event(event);

            //advance second iterator
            it_2++;
        }
    }

    /******************************************************************************/
    /*!
    \brief
        overload of operator << to print subscribers and events
    \param output
        where to print the subscribers to
    \param dispatcher
        dispatcher to check for printing
    */
    /******************************************************************************/
    std::ostream& operator<<(std::ostream& output, EventDispatcher& dispatcher)
    {
        //iterator for traversing the map
        std::map <TypeInfo, std::vector<Listener*> >::iterator it = dispatcher.event_collection.begin();

        while(it != dispatcher.event_collection.end())
        {
            //iterator to iterate the inside vector of Listener * 
            std::vector<Listener*>::iterator it_2 = it->second.begin();;

            output << "The event type " << it->first.get_name() << " has the following subscribers:\n";
            while(it_2 != it->second.end())
            {
                output << "\tAn instance of type " << typeid(**it_2).name() << "\n";
                
                //advance second iterator
                it_2++;
            }

            //advance first iterator
            it++;
        }

        return output;
    }   

    /******************************************************************************/
    /*!
    \brief
         trigger an event and make each subscriber to it react (it can be done from
         outside because instance is static)
    \param event
        event to trigger
    */
    /******************************************************************************/
    void trigger_event(const Event& event)
    {
        //call trigger event using the existing instance of the dispatcher
        EventDisp->trigger_event(event);
    }


}
