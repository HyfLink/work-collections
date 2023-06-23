#pragma once
#ifndef TMETA_NUMBER_DEF_H
#define TMETA_NUMBER_DEF_H

#include <cstdint>
#include "types.h"

namespace tmeta {

      template<int64_t N, int64_t D>  struct Number;

      namespace impl {

            template<int64_t N> struct abs_impl { static constexpr inline int64_t result = (N > 0 ? N : -N); };
            template<int64_t N> struct sign_impl { static constexpr inline int64_t result = (N ? (N > 0 ? 1 : -1) : 0); };

            template<int64_t N, int64_t D>
            struct gcd_impl_impl {
                  static constexpr inline int64_t result = gcd_impl_impl<D, N % D>::result;
            };

            template<int64_t N>
            struct gcd_impl_impl<N, 0> {
                  static constexpr inline int64_t result = N;
            };

            template<int64_t N, int64_t D>
            struct gcd_impl {
                  static constexpr inline int64_t result = sign_impl<D>::result * gcd_impl_impl<abs_impl<N>::result, abs_impl<D>::result>::result;
            };

            template<bool isZero, int64_t N, int64_t D>
            struct reduction {
                  static constexpr inline int64_t numer = 0;
                  static constexpr inline int64_t denom = 1;
            };

            template<int64_t N, int64_t D>
            struct reduction<false, N, D> {
                  static constexpr inline int64_t numer = N / gcd_impl_impl<N, D>::result;
                  static constexpr inline int64_t denom = D / gcd_impl_impl<N, D>::result;
            };

      } // namespace impl

      template<int64_t N, int64_t D>
      struct Number {
            using __tags__ = impl::__number_tags;
            static constexpr inline int64_t numer = (N / impl::gcd_impl<N, D>::result);
            static constexpr inline int64_t denom = (D / impl::gcd_impl<N, D>::result);
            static constexpr inline double instance = static_cast<double>(N) / static_cast<double>(D);
            using __value__ = Number<numer, denom>;
      };

      template<int64_t N>
      struct Number<N, 1> {
            using __tags__ = impl::__number_tags;
            using __value__ = Number<N, 1>;
            using inc = Number<N + 1, 1>;
            using dec = Number<N - 1, 1>;
            static constexpr inline int64_t numer = N;
            static constexpr inline int64_t denom = 1;
            static constexpr inline double instance = static_cast<double>(N);
      };

      template<int64_t N>
      struct Number<N, 0> {
            using __tags__ = impl::__number_tags;
            using __value__ = Number<0, 0>;
            static constexpr inline int64_t numer = 0;
            static constexpr inline int64_t denom = 0;
            static constexpr inline double instance = 0.0;
      };

} // namespace tmeta

#endif // TMETA_NUMBER_DEF_H
