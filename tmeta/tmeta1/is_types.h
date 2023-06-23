// functions :
//
//    - is[type]<typename T> :
//          type can be Null, boolean, Number, string, pair, list, nan, Integer
//          tparam T : type
//          return value : boolean<>
//
//    - is_same<typename U, typename V>
//          tparam U, V : any type
//          return value : boolean<>
//
#pragma once
#ifndef TMETA_IS_TYPES_H
#define TMETA_IS_TYPES_H

#include "types.h"
#include "number_def.h"

namespace tmeta {

      namespace impl {

            template<typename U, typename V> struct is_same_impl : False { };
            template<typename T> struct is_same_impl<T, T> : True { };

            template<typename T> struct is_integer_impl : False { };
            template<int64_t N> struct is_integer_impl<Integer<N>> : True { };

      } // namespace impl

      /* boolean */template<typename U, typename V> using isSame = value<impl::is_same_impl<value<U>, value<V>>>;
      /* boolean */template<typename T> using isNull = Boolean<T::__tags__::is_null>;
      /* boolean */template<typename T> using isBoolean = Boolean<T::__tags__::is_boolean>;
      /* boolean */template<typename T> using isNumber = Boolean<T::__tags__::is_number>;
      /* boolean */template<typename T> using isString = Boolean<T::__tags__::is_string>;
      /* boolean */template<typename T> using isPair = Boolean<T::__tags__::is_pair>;
      /* boolean */template<typename T> using isList = Boolean<T::__tags__::is_list>;
      /* boolean */template<typename T> using isInteger = value<impl::is_integer_impl<value<T>>>;
      /* boolean */template<typename T> using isNan = isSame<T, NaN>;

} // namespace tmeta

#endif // TMETA_IS_TYPES_H
