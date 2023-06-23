#pragma once
#ifndef TMETA_FUNCTION_H
#define TMETA_FUNCTION_H

#include "types.h"
#include "if_else.h"
#include "list_def.h"

namespace tmeta::impl {

      // [Boolean] all : return false if any FUNC returns false, else true
      // FUNC -> Boolean
      //    Param : each element in the list
      template<typename LIST, template<typename Param> typename FUNC> struct all_impl;

      template<typename U, typename V, template<typename Param> typename FUNC>
      struct all_impl<ListNode<U, V>, FUNC> : if_else<FUNC<U>, all_impl<V, FUNC>, False> { };

      template<typename U, template<typename Param> typename FUNC>
      struct all_impl<ListNode<U, Null>, FUNC> : FUNC<U> { };

      template<template<typename Param> typename FUNC>
      struct all_impl<Null, FUNC> : False { };

      // [Boolean] any : return true if any FUNC returns true, else false
      // FUNC -> Boolean
      //    Param : each element in the list
      template<typename LIST, template<typename Param> typename FUNC> struct any_impl;

      template<typename U, typename V, template<typename Param> typename FUNC>
      struct any_impl<ListNode<U, V>, FUNC> : if_else<FUNC<U>, True, any_impl<V, FUNC>> { };

      template<template<typename Param> typename FUNC>
      struct any_impl<Null, FUNC> : False { };


      // [List] map : make a new list using the return value of FUNC
      // FUNC -> any_impl tmeta types
      //    Param : each element in the list
      template<typename LIST, template<typename Param> typename FUNC> struct map_impl;

      template<typename U, typename V, template<typename Param> typename FUNC>
      struct map_impl<ListNode<U, V>, FUNC> : ListNode<FUNC<U>, map_impl<V, FUNC>> { };

      template<template<typename Param> typename FUNC>
      struct map_impl<Null, FUNC> : Null { };


      // [List] filter : make a new list using element that FUNC returns true
      // FUNC -> Boolean
      //    Param : each element in the list
      template<typename LIST, template<typename Param> typename FUNC> struct filter_impl;

      template<typename U, typename V, template<typename Param> typename FUNC>
      struct filter_impl<ListNode<U, V>, FUNC> : if_else<FUNC<U>, ListNode<U, filter_impl<V, FUNC>>, filter_impl<V, FUNC>> { };

      template<template<typename Param> typename FUNC>
      struct filter_impl<Null, FUNC> : Null { };

      // [List] transform : use FUNC on each pair of elements in list1 and list2
      //    and make a new list using the return value of FUNC
      // FUNC -> Any
      //    Param1 : element in list1
      //    Param2 : element in list2
      template<typename LIST1, typename LIST2, template<typename Param1, typename Param2> typename FUNC> struct transform_impl;

      template<typename T, typename U, typename V, typename W, template<typename Param1, typename Param2> typename FUNC>
      struct transform_impl<ListNode<T, U>, ListNode<V, W>, FUNC> : ListNode<FUNC<T, V>, transform_impl<U, W, FUNC>> { };

      template<typename LIST, template<typename Param1, typename Param2> typename FUNC>
      struct transform_impl<Null, LIST, FUNC> : Null { };

      template<typename LIST, template<typename Param1, typename Param2> typename FUNC>
      struct transform_impl<LIST, Null, FUNC> : Null { };

      template<template<typename Param1, typename Param2> typename FUNC>
      struct transform_impl<Null, Null, FUNC> : Null { };

      // [Any] fold : use FUNC on ACC and each element in the list
      // ACC : initial value and acceleration value
      // FUNC -> Any
      //    Param1 : ACC
      //    Param2 : each element in the list
      template<typename LIST, typename ACC, template<typename Param1, typename Param2> typename FUNC> struct fold_impl;

      template<typename U, typename V, typename ACC, template<typename Param1, typename Param2> typename FUNC>
      struct fold_impl<ListNode<U, V>, ACC, FUNC> : fold_impl<V, FUNC<ACC, U>, FUNC> { };

      template<typename U, typename ACC, template<typename Param1, typename Param2> typename FUNC>
      struct fold_impl<ListNode<U, Null>, ACC, FUNC> : FUNC<ACC, U> { };

      // [Function]  Bind
      template<template<typename...Params> typename Func, int64_t Index, typename Param>
      struct Bind;

      template<template<typename...Params> typename Func, typename Param>
      struct Bind<Func, 1, Param> {
            template<typename...Params>
            struct func : Func<Param, Params ...> { };
      };

      template<template<typename...Params> typename Func, typename Param>
      struct Bind<Func, 2, Param> {
            template<typename Param1, typename...Params>
            struct func : Func<Param1, Param, Params ...> { };
      };

      template<template<typename...Params> typename Func, typename Param>
      struct Bind<Func, 3, Param> {
            template<typename Param1, typename Param2, typename...Params>
            struct func : Func<Param1, Param2, Param, Params ...> { };
      };


} // namespace tmeta::impl

namespace tmeta {

      template<typename LIST, template<typename Param> typename FUNC> using All = value<impl::all_impl<LIST, FUNC>>;
      template<typename LIST, template<typename Param> typename FUNC> using Any = value<impl::any_impl<LIST, FUNC>>;
      template<typename LIST, template<typename Param> typename FUNC> using Map = value<impl::map_impl<LIST, FUNC>>;
      template<typename LIST, template<typename Param> typename FUNC> using Filter = value<impl::filter_impl<LIST, FUNC>>;
      template<typename LIST1, typename LIST2, template<typename Param1, typename Param2> typename FUNC> using Transform = value<impl::transform_impl<LIST1, LIST2, FUNC>>;
      template<typename LIST, typename ACCELE, template<typename Param1, typename Param2> typename FUNC> using Fold = value<impl::fold_impl<LIST, ACCELE, FUNC>>;
      template<template<typename...Params> typename Func, int64_t Index, typename Param> using Bind = impl::Bind<Func, Index, Param>;

} // namespace tmeta

#endif // TMETA_FUNCTION_H
