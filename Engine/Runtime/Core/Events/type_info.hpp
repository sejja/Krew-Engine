#pragma once

namespace events
{
    
    class TypeInfo
    {
        public: 
        
           template <typename T> TypeInfo(const T& to_instantiate) // constructor to create TypeInfo from any type
           {
             name = typeid(to_instantiate).name();
           }

           TypeInfo(const std::type_info& to_instantiate); //construct Typeinfo from type_info struct
           
           const std::string get_name() const; //get the name inside typeinfo
           
           bool operator==(const TypeInfo& to_compare) const;  //overload of the comparison operator
           bool operator!=(const TypeInfo& to_diff) const;     //overload of the diff operator
           bool operator<(const TypeInfo& to_compare)const;    //overload of less than operator


        private:
          
        std::string name; //name of the typeinfo 
           
    };


    //return typeinfo of passed argument
    template <typename T> TypeInfo type_of(const T& to_check)
    {
        return TypeInfo(to_check);
    }

    //return typeinfo of a type
    template <typename T> TypeInfo type_of()
    {
        return TypeInfo(typeid(T));
    }

}