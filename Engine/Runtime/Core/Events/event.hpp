#pragma once

namespace events
{
    
    
    class ENGINE_API Event
    {
        
        public:
        virtual ~Event() = 0;  // event destructor 
        private:
        
        
    };

    class ENGINE_API HandlerFunction
    {
        public:
        void handle(const Event& concrete_event);  //handle for a specific event of an object
 
        virtual ~HandlerFunction(){} // destructor
        private:
        virtual void call(const Event& concrete_event) = 0; // call function to be overrided by children
    };
    
    template <typename T, typename EVENT>
    class MemberFunctionHandler : public HandlerFunction
    {
        public:
        // constructor of Memberfunctionhandler
        typedef void (T::*MemberFunction)(const EVENT&);
        MemberFunctionHandler(T * who, MemberFunction function) : who_(who), what_(function) {} 

        private:
        T * who_;
        MemberFunction what_;
        
        //call to the object's function to deal with event
        void call(const Event& event)
        {
            (who_->*what_)(static_cast<const EVENT&>(event));
        }

    };

    class ENGINE_API EventHandler
    {
        public:

        template <typename T, typename EVENT>
        void register_handler(T& who, void (T::*function)(const EVENT&)) //register a handler for a specific event of an object
        {
            //store a member function associated with the type of even it needs to be passed
           handler_collection[typeid(EVENT).name()] = new MemberFunctionHandler<T,EVENT>(&who,function);
        }

        void handle(const Event& event) //search the passed event and handle it
        {
            std::map<std::string, HandlerFunction *>::iterator it;
            it = handler_collection.find(typeid(event).name());

            //advance iterator
            if(it != handler_collection.end())
            {
                it->second->handle(event);
            }

        }

        ~EventHandler(); // eventhandler destructor
        private:

        std::map<std::string, HandlerFunction *> handler_collection;
    };
    
}