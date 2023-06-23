#pragma once
#if !defined(TMETA_ARITHMETIC_H)
#define TMETA_ARITHMETIC_H

#include "types.h"
#include "is_types.h"
#include "if_else.h"
#include "number_def.h"

namespace tmeta {

      // arithmetic & comparation implementation
      // ---------------------------------------
      namespace impl {
            template<typename N1, typename N2> struct div_impl {
                  static constexpr inline int64_t N = (N1::numer * N2::denom);
                  static constexpr inline int64_t D = (N1::denom * N2::numer);
                  using value = Number<N, D>;
            };
            template<typename N1> struct div_impl<N1, Zero> { using value = NaN; };

            template<typename Base, int64_t Exp> struct power_impl {
                  static constexpr inline int64_t N = value<power_impl<Base, Exp - 1>>::numer * Base::numer;
                  static constexpr inline int64_t D = value<power_impl<Base, Exp - 1>>::denom * Base::denom;
                  using value = Number<N, D>;
            };
            template<typename Base> struct power_impl<Base, 0> { using value = One; };
            template<int64_t Exp> struct power_impl<NaN, Exp> { using value = NaN; };
            template<int64_t Exp> struct power_impl<Zero, Exp> { using value = Zero; };
            template<int64_t Exp> struct power_impl<One, Exp> { using value = One; };
            template<> struct power_impl<Zero, 0> { using value = NaN; };

            template<int64_t N> struct fact_impl { static constexpr inline int64_t result = N * fact_impl<N - 1>::result; };
            template<> struct fact_impl<0> { static constexpr inline int64_t result = 1; };
      } // namespace impl

      /* Integer */template<typename N> using ToInteger = Integer<static_cast<int64_t>(N::instance)>;
      /* Integer */template<typename N> using Celling = Integer<(value<N>::denom == 1 ? value<N>::numer : static_cast<int64_t>(value<N>::instance + (value<N>::instance < 0.0 ? +0.0 : +1.0)))>;
      /* Integer */template<typename N> using Round = Integer<(value<N>::denom == 1 ? value<N>::numer : static_cast<int64_t>(value<N>::instance + (value<N>::instance < 0.0 ? -0.5 : +0.5)))>;
      /* Integer */template<typename N> using Floor = Integer<(value<N>::denom == 1 ? value<N>::numer : static_cast<int64_t>(value<N>::instance + (value<N>::instance < 0.0 ? -1.0 : +0.0)))>;

      /* Number  */template<typename N1, typename N2> using Add = value<Number<(N1::numer * N2::denom + N1::denom * N2::numer), (N1::denom * N2::denom)>>;
      /* Number  */template<typename N1, typename N2> using Sub = value<Number<(N1::numer * N2::denom - N1::denom * N2::numer), (N1::denom * N2::denom)>>;
      /* Number  */template<typename N1, typename N2> using Mul = value<Number<(N1::numer * N2::numer), (N1::denom * N2::denom)>>;
      /* Number  */template<typename N1, typename N2> using Div = value<impl::div_impl<N1, N2>>;

      /* boolean */template<typename N1, typename N2> using EQ = Boolean<(value<N1>::numer == value<N2>::numer && value<N1>::denom == value<N2>::denom)>;
      /* boolean */template<typename N1, typename N2> using NEQ = Boolean<(value<N1>::numer != value<N2>::numer || value<N1>::denom != value<N2>::denom)>;
      /* boolean */template<typename N1, typename N2> using GT = Boolean<(value<N1>::instance > value<N2>::instance)>;
      /* boolean */template<typename N1, typename N2> using LT = Boolean<(value<N1>::instance < value<N2>::instance)>;
      /* boolean */template<typename N1, typename N2> using GTEQ = Boolean<(value<N1>::instance >= value<N2>::instance)>;
      /* boolean */template<typename N1, typename N2> using LTEQ = Boolean<(value<N1>::instance <= value<N2>::instance)>;

      /* Number  */template<typename N>               using ABS = value<Number<(N::numer > 0 ? N::numer : -N::numer), N::denom>>;
      /* Integer */template<typename N>               using SIGN = Integer<(N::numer ? (N::numer > 0 ? 1 : -1) : 0)>;
      /* Integer */template<typename N1, typename N2> using GCD = Integer<impl::gcd_impl<ToInteger<N1>::numer, ToInteger<N2>::numer>::result>;
      /* Integer */template<typename N1, typename N2> using LCM = Integer<ToInteger<N1>::numer * ToInteger<N2>::numer / impl::gcd_impl<ToInteger<N1>::numer, ToInteger<N2>::numer>::result>;
      /* Integer */template<typename N>               using FACT = Integer<impl::fact_impl<ToInteger<N>::numer>::result>;

      /* Number  */template<typename Base, typename Exp> using Power = value<impl::power_impl<Base, ToInteger<Exp>::numer>>;


} // namespace tmeta

#endif // TMETA_ARITHMETIC_H
