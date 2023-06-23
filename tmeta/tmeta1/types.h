#pragma once
#ifndef TMETA_TYPES_H
#define TMETA_TYPES_H

#include <cstdint>

namespace tmeta {

      // forward declaration
      // -------------------

      struct Null;
      template<bool flag>                 struct Boolean;
      template<char...Cs>                 struct String;
      template<typename U, typename V>    struct Pair;
      template<int64_t N, int64_t D>      struct Number;

      template<int64_t N>            using Integer = Number<N, 1>;
      template<int64_t N, int64_t D> using Fraction = Number<N, D>;

      /* any     */template<typename T> using value = typename T::__value__;
      /* any     */template<typename P> using car = typename P::__car__::__value__;
      /* any     */template<typename P> using cdr = typename P::__cdr__::__value__;
      /* Integer */template<typename T> using len = Integer<value<T>::__len__>;

      using NaN = Fraction<0, 0>;
      using Zero = Integer<0>;
      using One = Integer<1>;
      using True = Boolean<true>;
      using False = Boolean<false>;

      // tmeta types implementation
      // --------------------------

      namespace impl {

            struct __tags {
                  static inline constexpr bool is_null = false;
                  static inline constexpr bool is_boolean = false;
                  static inline constexpr bool is_number = false;
                  static inline constexpr bool is_string = false;
                  static inline constexpr bool is_pair = false;
                  static inline constexpr bool is_list = false;
            };

            struct __null_tags : __tags { static inline constexpr bool is_null = true; };
            struct __boolean_tags : __tags { static inline constexpr bool is_boolean = true; };
            struct __number_tags : __tags { static inline constexpr bool is_number = true; };
            struct __string_tags : __tags { static inline constexpr bool is_string = true; };
            struct __pair_tags : __tags { static inline constexpr bool is_pair = true; };
            struct __list_tags : __tags { static inline constexpr bool is_list = true; };

      } // namespace impl


      struct Null {
            using __tags__ = impl::__null_tags;
            using __value__ = Null;
      };

      template<bool flag>
      struct Boolean {
            using __tags__ = impl::__boolean_tags;
            using __value__ = Boolean<flag>;
            static constexpr inline bool instance = flag;
      };


      template<char...Cs>
      struct String {
            using __tags__ = impl::__string_tags;
            static constexpr int64_t __len__ = sizeof...(Cs);
            static constexpr inline char c_str[__len__ + 1] = { Cs..., '\0' };
            using __value__ = String<Cs...>;
      };

      template<typename U, typename V>
      struct Pair {
            using __tags__ = impl::__pair_tags;
            using __car__ = typename U::value;
            using __cdr__ = typename V::value;
            using __value__ = Pair<U, V>;
      };

} // namespace tmeta

#endif // TMETA_TYPES_H