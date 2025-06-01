#pragma once
#include <type_traits>
#include <tuple>

namespace std
{
    template <class Ty>
    class function;
}

template <bool Value>
struct valid_constant
{
    static constexpr bool is_valid = Value;
};


template <bool Enabled, class Ty>
struct basic_function_traits : valid_constant<false> { };

template <class Return_ty, class... Args_ty> // for default function ptrs
struct basic_function_traits<true, Return_ty(*)(Args_ty...)> : valid_constant<true>
{
    using result_t       = Return_ty;
    using function_ptr_t = Return_ty(*)(Args_ty...);
    using arguments_t    = std::tuple<Args_ty...>;

    template <size_t Index>
    using get_argument_t = std::tuple_element_t<Index, arguments_t>;

    static constexpr size_t arguments_count = sizeof...(Args_ty);
    static constexpr bool is_nothrow = false;
};

template <class Return_ty, class Object_ty, class... Args_ty> // for non-static functions
struct basic_function_traits<true, Return_ty(Object_ty::*)(Args_ty...)> : valid_constant<true>
{
    using result_t       = Return_ty;
    using object_t       = Object_ty;
    using function_ptr_t = Return_ty(Object_ty::*)(Args_ty...);
    using arguments_t    = std::tuple<Args_ty...>;

    template <size_t Index>
    using get_argument_t = std::tuple_element_t<Index, arguments_t>;

    static constexpr size_t arguments_count = sizeof...(Args_ty);
    static constexpr bool is_nothrow = false;
    static constexpr bool is_const   = false;
};

template <class Return_ty, class... Args_ty> // for default noexcept function ptrs
struct basic_function_traits<true, Return_ty(*)(Args_ty...) noexcept> :
    basic_function_traits<true, Return_ty(*)(Args_ty...)>
{
    using function_ptr_t = Return_ty(*)(Args_ty...) noexcept;
    static constexpr bool is_nothrow = true;
};

template <class Return_ty, class... Args_ty> // for default noexcept function (non ptr)
struct basic_function_traits<true, Return_ty(Args_ty...) noexcept> :
    basic_function_traits<true, Return_ty(*)(Args_ty...)>
{
    using function_ptr_t = Return_ty(*)(Args_ty...) noexcept;
    static constexpr bool is_nothrow = true;
};

template <class Return_ty, class Object_ty, class... Args_ty> // for objects noexcept function (only non-static)
struct basic_function_traits<true, Return_ty(Object_ty::*)(Args_ty...) noexcept> :
    basic_function_traits<true, Return_ty(Object_ty::*)(Args_ty...)>
{
    using function_ptr_t = Return_ty(Object_ty::*)(Args_ty...) noexcept;
    static constexpr bool is_nothrow = true;
};

template <class Return_ty, class Object_ty, class... Args_ty> // for objects const function (only non-static)
struct basic_function_traits<true, Return_ty(Object_ty::*)(Args_ty...) const> :
    basic_function_traits<true, Return_ty(Object_ty::*)(Args_ty...)>
{
    using function_ptr_t = Return_ty(Object_ty::*)(Args_ty...) const;
    static constexpr bool is_const = true;
};

template <class Return_ty, class Object_ty, class... Args_ty> // for objects const noexcept function (only non-static)
struct basic_function_traits<true, Return_ty(Object_ty::*)(Args_ty...) const noexcept> :
    basic_function_traits<true, Return_ty(Object_ty::*)(Args_ty...)>
{
    using function_ptr_t = Return_ty(Object_ty::*)(Args_ty...) const noexcept;
    static constexpr bool is_nothrow = true;
    static constexpr bool is_const = true;
};


template <class Ty>
struct function_traits : valid_constant<false> { };

template <class Return_ty, class... Args_ty> // function ptr
struct function_traits<Return_ty(*)(Args_ty...)> : basic_function_traits<true, Return_ty(*)(Args_ty...)> { };

template <class Return_ty, class... Args_ty> // noexcept function ptr
struct function_traits<Return_ty(*)(Args_ty...) noexcept> : basic_function_traits<true, Return_ty(*)(Args_ty...) noexcept> { };

template <class Return_ty, class... Args_ty> // pure function (non ptr)
struct function_traits<Return_ty(Args_ty...)> : basic_function_traits<true, Return_ty(*)(Args_ty...)> { };

template <class Return_ty, class... Args_ty> // pure noexcpet function (non ptr)
struct function_traits<Return_ty(Args_ty...) noexcept> : basic_function_traits<true, Return_ty(*)(Args_ty...) noexcept> { };

template <class Return_ty, class Object_ty, class... Args_ty> // object non-static function ptr
struct function_traits<Return_ty(Object_ty::*)(Args_ty...)> :
    basic_function_traits<true, Return_ty(Object_ty::*)(Args_ty...)> { };

template <class Return_ty, class Object_ty, class... Args_ty> // noexcept object non-function ptr
struct function_traits<Return_ty(Object_ty::*)(Args_ty...) noexcept> :
    basic_function_traits<true, Return_ty(Object_ty::*)(Args_ty...) noexcept> { };

template <class Return_ty, class Object_ty, class... Args_ty> // object const non-static function ptr
struct function_traits<Return_ty(Object_ty::*)(Args_ty...) const> :
    basic_function_traits<true, Return_ty(Object_ty::*)(Args_ty...) const> { };

template <class Return_ty, class Object_ty, class... Args_ty> // object const noexcept non-static function ptr
struct function_traits<Return_ty(Object_ty::*)(Args_ty...) const noexcept> :
    basic_function_traits<true, Return_ty(Object_ty::*)(Args_ty...) const noexcept> { };

template <class Return_ty, class... Args_ty> // 'std::function'
struct function_traits<std::function<Return_ty(Args_ty...)>> : basic_function_traits<true, Return_ty(*)(Args_ty...)> { };