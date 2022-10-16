//
//	RTTI.h
//	Good Neighbours
//
//	Created by Diego Revilla on 06/05/21
//	Copyright � 2021 Diego Revilla. All Rights reserved
//

#ifndef _RTTI__H_
#define _RTTI__H_

namespace Engine {
	class RunTimeTypeInformation : public Singleton<RunTimeTypeInformation> {
	public:
		template<typename TYPE, typename OTHERTYPE>
		constexpr bool inline IsA(OTHERTYPE* other) const noexcept;

		template<typename TYPE>
		constexpr const char inline* GetName() const noexcept;

		template<typename PARENT, typename CHILD>
		constexpr bool inline IsChildOf(const PARENT* parent = nullptr, const CHILD* child = nullptr) const noexcept;

		template<typename TYPE>
		constexpr size_t inline GetTypeHash() const noexcept;

		template<typename TYPE>
		constexpr bool inline IsVoid(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsNullptr(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsInt(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsFloat(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsArray(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsEnumerator(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsUnion(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsClass(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsFunction(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsPointer(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsLValue(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsRValue(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsMemberObject(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsMemberFunction(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsFundamental(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsArithmetic(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsScalar(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsObject(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsCompound(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsReference(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsMember(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsConst(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsVolatile(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsTrivial(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsStandardLayout(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsPlainOldData(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline HasUniqueRepresentation(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsEmpty(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsPolymorphic(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsAbstract(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsFinal(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsAggregate(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsSigned(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsUnsigned(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsBoundedArray(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsUnboundedArray(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool IsConstructible(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool IsDefaultConstructible(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool IsCopyConstructible(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool IsMoveConstructible(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool IsAssignable(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool IsDestructible(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline HasVirtualDestructor(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr size_t inline AlignmentOf(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr size_t inline Rank(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr size_t inline Extent(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE, typename OTHERTYPE>
		constexpr bool inline IsEqual(const TYPE* type = nullptr, const OTHERTYPE* othertype = nullptr) const noexcept;

		template<typename FROM, typename TO>
		constexpr bool inline IsConvertible(const FROM* from = nullptr, const TO* to = nullptr)  const noexcept;

		template<typename TYPE>
		constexpr bool inline IsInvocable(const TYPE* type = nullptr) const noexcept;

		template<typename TYPE>
		constexpr bool inline IsLayoutCompatible(const TYPE* type = nullptr) const noexcept;
	};

#define _RTTI	RunTimeTypeInformation::

	// ------------------------------------------------------------------------
	/*! Is A
	*
	*   Is "other" a <<type>>
	*/ // ---------------------------------------------------------------------
	template<typename TYPE, typename OTHERTYPE>
	constexpr bool inline _RTTI IsA(OTHERTYPE* other) const noexcept {
		return dynamic_cast<TYPE*>(other);
	}

	// ------------------------------------------------------------------------
	/*! Get Name
	*
	*   Gets the Name of <<TYPE>>
	*/ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr const char inline* _RTTI GetName() const noexcept {
		return typeid(TYPE).name();
	}

	// ------------------------------------------------------------------------
	/*! Is Child Of
	*
	*   Is <<child>> a derivate of <<parent>>
	*/ // ---------------------------------------------------------------------
	template<typename PARENT, typename CHILD>
	constexpr  bool inline _RTTI IsChildOf(const PARENT* parent, const CHILD* child) const noexcept {
		return std::is_base_of<CHILD, PARENT>::value;
	}

	// ------------------------------------------------------------------------
	/*! Get Type Hash
	*
	*   Gets the corresponding Hash of the type
	*/ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr  size_t inline _RTTI GetTypeHash() const noexcept {
		return typeid(TYPE).hash_code();
	}

	// ------------------------------------------------------------------------
   /*! Is void
   *
   *   Is "type" a <<void>>
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline  _RTTI IsVoid(const TYPE* type) const noexcept {
		return std::is_void<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is nullptr
   *
   *   Is "type" a <<nullptr>>
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsNullptr(const TYPE* type) const noexcept {
		return std::is_null_pointer<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is int
   *
   *   Is "type" a <<int>>
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsInt(const TYPE* type) const noexcept {
		return std::is_integral<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is float
   *
   *   Is "type" a <<float>>
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsFloat(const TYPE* type) const noexcept {
		return std::is_floating_point<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is array
   *
   *   Is "type" a <<array>>
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsArray(const TYPE* type) const noexcept {
		return std::is_array<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Enum
   *
   *   Is "type" a <<Enum>>
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsEnumerator(const TYPE* type) const noexcept {
		return std::is_enum<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Union
   *
   *   Is "type" a <<Union>>
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsUnion(const TYPE* type) const noexcept {
		return std::is_union<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Class
   *
   *   Is "type" a <<Class>>
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsClass(const TYPE* type) const noexcept {
		return std::is_class<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Function
   *
   *   Is "type" a <<Function>>
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsFunction(const TYPE* type) const noexcept {
		return std::is_function<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Pointer
   *
   *   Is "type" a <<Pointer>>
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsPointer(const TYPE* type) const noexcept {
		return std::is_pointer<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is L Value
   *
   *   Is "type" a <<L Value>>
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsLValue(const TYPE* type) const noexcept {
		return std::is_lvalue_reference<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is R Value
   *
   *   Is "type" a <<R Value>>
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsRValue(const TYPE* type) const noexcept {
		return std::is_rvalue_reference<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Member Object
   *
   *   Is "type" a Member Object
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsMemberObject(const TYPE* type) const noexcept {
		return std::is_member_object_pointer<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Member Function
   *
   *   Is "type" a Member Function
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsMemberFunction(const TYPE* type) const noexcept {
		return std::is_member_function_pointer<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Fundamental
   *
   *   Is "type" a Fundamental
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsFundamental(const TYPE* type) const noexcept {
		return std::is_fundamental<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Airthmetic
   *
   *   Is "type" an Arithmetic
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsArithmetic(const TYPE* type) const noexcept {
		return std::is_arithmetic<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Scalar
   *
   *   Is "type" a Scalar
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsScalar(const TYPE* type) const noexcept {
		return std::is_scalar<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Object
   *
   *   Is "type" an Object
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsObject(const TYPE* type) const noexcept {
		return std::is_object<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Compound
   *
   *   Is "type" a Compound
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsCompound(const TYPE* type) const noexcept {
		return std::is_compound<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Rederence
   *
   *   Is "type" a <<Rederence>>
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsReference(const TYPE* type) const noexcept {
		return std::is_reference<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Member
   *
   *   Is "type" a Member
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsMember(const TYPE* type) const noexcept {
		return std::is_member_pointer<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Const
   *
   *   Is "type" a const
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsConst(const TYPE* type) const noexcept {
		return std::is_const<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is volatile
   *
   *   Is "type" a volatile
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsVolatile(const TYPE* type) const noexcept {
		return std::is_volatile<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Trivial
   *
   *   Is "type" a Trivial
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsTrivial(const TYPE* type) const noexcept {
		return std::is_trivial<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Standard Layout
   *
   *   Has the type members been suffled?
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsStandardLayout(const TYPE* type) const noexcept {
		return std::is_standard_layout<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Plain Old Data
   *
   *   Guess
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsPlainOldData(const TYPE* type) const noexcept {
		return std::is_pod<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Has Unique Representation
   *
   *   Wether type have aliases or not
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI HasUniqueRepresentation(const TYPE* type) const noexcept {
		return std::has_unique_object_representations<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Empty
   *
   *   Is "type" empty
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsEmpty(const TYPE* type) const noexcept {
		return std::is_empty<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Polymorphic
   *
   *   Is "type" Polymorphic
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsPolymorphic(const TYPE* type) const noexcept {
		return std::is_polymorphic<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Abstract
   *
   *   Is "type" Abstract
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsAbstract(const TYPE* type) const noexcept {
		return std::is_abstract<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Final
   *
   *   Is "type" Final
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsFinal(const TYPE* type) const noexcept {
		return std::is_final<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Aggregate
   *
   *   Is "type" Aggregate
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsAggregate(const TYPE* type) const noexcept {
		return std::is_aggregate<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Signed
   *
   *   Is "type" Signed
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsSigned(const TYPE* type) const noexcept {
		return std::is_signed<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Unsigned
   *
   *   Is "type" Unsigned
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsUnsigned(const TYPE* type) const noexcept {
		return std::is_unsigned<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Constructible
   *
   *   Is "type" Constructible
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool _RTTI IsConstructible(const TYPE* type) const noexcept {
		return std::is_constructible<TYPE>::value || std::is_trivially_constructible<TYPE>::value
			|| std::is_nothrow_constructible<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Default Constructible
   *
   *   Is "type" Default Constructible
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool _RTTI IsDefaultConstructible(const TYPE* type) const noexcept {
		return std::is_default_constructible<TYPE>::value || std::is_trivially_default_constructible<TYPE>::value
			|| std::is_nothrow_default_constructible<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Copy Constructible
   *
   *   Is "type" Copy Constructible
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool _RTTI IsCopyConstructible(const TYPE* type) const noexcept {
		return std::is_copy_constructible<TYPE>::value || std::is_trivially_copy_constructible<TYPE>::value
			|| std::is_nothrow_copy_constructible<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Move Constructible
   *
   *   Is "type" Move Constructible
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool _RTTI IsMoveConstructible(const TYPE* type) const noexcept {
		return std::is_move_constructible<TYPE>::value || std::is_trivially_move_constructible<TYPE>::value
			|| std::is_nothrow_move_constructible<TYPE>::value;;
	}

	// ------------------------------------------------------------------------
   /*! Is Assignable
   *
   *   Is "type" Assignable
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool _RTTI IsAssignable(const TYPE* type) const noexcept {
		return std::is_assignable<TYPE>::value || std::is_trivially_assignable<TYPE>::value
			|| std::is_nothrow_assignable<TYPE>::value || std::is_copy_assignable<TYPE>::value
			|| std::is_trivially_copy_assignable<TYPE>::value || std::is_nothrow_copy_assignable<TYPE>::value
			|| std::is_move_assignable<TYPE>::value || std::is_trivially_move_assignable<TYPE>::value
			|| std::is_nothrow_move_assignable<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Destructible
   *
   *   Is "type" Destructible
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool _RTTI IsDestructible(const TYPE* type) const noexcept {
		return std::is_constructible<TYPE>::value || std::is_trivially_constructible<TYPE>::value
			|| std::is_nothrow_constructible<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Has Virtual Destructor
   *
   *   Wether or not <<TYPE>> has a virtual destructor
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI HasVirtualDestructor(const TYPE* type) const noexcept {
		return std::has_virtual_destructor<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! AlignmentOf
   *
   *   Returns best alignment for field
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr size_t inline _RTTI AlignmentOf(const TYPE* type) const noexcept {
		return std::alignment_of<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Rank
   *
   *   Given an array, retrieve it's Dimensions
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr size_t inline _RTTI Rank(const TYPE* type) const noexcept {
		return std::rank<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Extent
   *
   *   Return the extent of an array Dimension
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr size_t inline _RTTI Extent(const TYPE* type) const noexcept {
		return std::extent<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Equal
   *
   *   Are two types the same
   */ // ---------------------------------------------------------------------
	template<typename TYPE, typename OTHERTYPE>
	constexpr bool inline _RTTI IsEqual(const TYPE* type, const OTHERTYPE* othertype) const noexcept {
		return std::is_same<TYPE, OTHERTYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Invocable
   *
   *   Is "type" invocable
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline _RTTI IsInvocable(const TYPE* type) const noexcept {
		return std::is_invocable<TYPE>::value;
	}

	// ------------------------------------------------------------------------
   /*! Is Layout Compatible
   *
   *   Is "type" Layout Compatible
   */ // ---------------------------------------------------------------------
	template<typename TYPE>
	constexpr bool inline RunTimeTypeInformation::IsLayoutCompatible(const TYPE* type) const noexcept {
		return std::is_standard_layout<TYPE>::value;
	}
#undef _RTTI
}

#define GRTTI (&Engine::RunTimeTypeInformation::Instance())

#endif