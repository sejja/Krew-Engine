
#include <shared.h>
namespace events
{
    

    /******************************************************************************/
    /*!
    \brief
        constructor of Typeinfo from a type_info 
    \param to_instantiate
        type info with the name needed for instantiation
    */
    /******************************************************************************/
    TypeInfo::TypeInfo(const std::type_info& to_instantiate)
    {
        name = to_instantiate.name();
    }

    /******************************************************************************/
    /*!
    \brief
         get the name stored in Typeinfo
    */
    /******************************************************************************/
    const std::string TypeInfo::get_name() const
    {
        return name;
    }

    /******************************************************************************/
    /*!
    \brief
        overload of comparison operator
    \param to_compare
        Typeinfo to compare to
    */
    /******************************************************************************/     
    bool TypeInfo::operator==(const TypeInfo& to_compare) const
    {
        //check if what was passed was a typeinfo in the first place
        return name == to_compare.name;
    }

    /******************************************************************************/
    /*!
    \brief
        overload of not equal operator
    \param to_compare
        Typeinfo to compare to
    */
    /******************************************************************************/     
    bool TypeInfo::operator!=(const TypeInfo& to_diff) const
    {
        //check if what was passed was a typeinfo in the first place
        return name != to_diff.name;

    }

    /******************************************************************************/
    /*!
    \brief
        overload of "less than" operator
    \param to_compare
        Typeinfo to compare to
    */
    /******************************************************************************/     
    bool TypeInfo::operator<(const TypeInfo& to_compare)const
    {
        //check if what was passed was a typeinfo in the first place
        return name < to_compare.name;

    }


}