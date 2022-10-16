#include <shared.h>
namespace events
{
   

    /******************************************************************************/
    /*!
    \brief
        destructor of even class
    */
    /******************************************************************************/
    Event::~Event()
    {
        
    }

    /******************************************************************************/
    /*!
    \brief
        given an event, call the concrete "call" function of the handlerfunction
    \param concrete_event
        event to call
    */
    /******************************************************************************/
    void HandlerFunction::handle(const Event& concrete_event)
    {
        call(concrete_event);
    }

    /******************************************************************************/
    /*!
    \brief
        de-allocate memory for handler functions and collection
    */
    /******************************************************************************/
    EventHandler::~EventHandler()
    {
        //iterator
        std::map<std::string, HandlerFunction *>::iterator it = handler_collection.begin();
        //de-allocate memory for all handler function

        while(it != handler_collection.end())
        {
            delete it->second;
            it++;
        }

        //deallocate collection
        handler_collection.clear();

    }

}