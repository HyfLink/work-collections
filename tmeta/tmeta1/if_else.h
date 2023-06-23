// flow control :
//
//    - if_else_impl<typename If, typename Then, typename Else>
//          if 'If' is True, if_else_impl<...> equals 'Then',
//          if 'If' is False, if_else_impl<...> equals 'Else',
//          else if 'If' is not boolean, error occurs.
//
//    - conds_impl<typename...Conds>
//          sizeof...(Conds) must odd, and Conds can be expanded into condition1,
//          statement1, condition2, statement2, ..., else_statement.
//          if condition-i is True, return statement-i, i will be the smallest.
//          if all the conditions are not True, return else_statement.
//
//    - Not/AND/OR/Xor :
//          Logical Operations.
//          return value : Boolean<>
//
#pragma once
#ifndef TMETA_IF_ELSE_H
#define TMETA_IF_ELSE_H

#include "types.h"

namespace tmeta {

      namespace impl {

            template<typename If, typename Then, typename Else>
            struct if_else_impl;

            template<typename Then, typename Else>
            struct if_else_impl<True, Then, Else> : value<Then> { };

            template<typename Then, typename Else>
            struct if_else_impl<False, Then, Else> : value<Else> { };

            template<int64_t N, typename Then, typename Else>
            struct if_else_impl<Integer<N>, Then, Else> : value<Then> { };

            template<typename Then, typename Else>
            struct if_else_impl<Zero, Then, Else> : value<Else> { };

            template<typename...Conds>
            struct conds_impl;

            template<typename Cond, typename Then, typename...Conds>
            struct conds_impl<Cond, Then, Conds...> : if_else_impl<value<Cond>, value<Then>, conds_impl<Conds...>> { };

            template<typename Cond, typename Then>
            struct conds_impl<Cond, Then> : if_else_impl<value<Cond>, value<Then>, Null> { };

            template<typename Else>
            struct conds_impl<Else> : value<Else> { };

      } // namespace impl

      template<typename If, typename Then, typename Else>
      using if_else = value<impl::if_else_impl<If, Then, Else>>;

      template<typename Ifnot, typename Then, typename Else>
      using if_not_else = value<impl::if_else_impl<Ifnot, Else, Then>>;

      template<typename...Conds>
      using conds = value<impl::conds_impl<Conds...>>;

      template<typename P>             using Logic = if_else<value<P>, True, False>;
      template<typename P>             using Not = if_else<value<P>, False, True>;
      template<typename P, typename Q> using Xor = if_else<value<P>, Not<Q>, value<Q>>;

      namespace impl {

            template<typename...Ps>             struct or_impl;
            template<typename P, typename...Qs> struct or_impl<P, Qs ...> : if_else<P, True, or_impl<Qs ...>> { };
            template<typename P>                struct or_impl<P> : Logic<P> { };

            template<typename...Ps>             struct and_impl;
            template<typename P, typename...Qs> struct and_impl<P, Qs ...> : if_not_else<P, False, and_impl<Qs ...>> { };
            template<typename P>                struct and_impl<P> : Logic<P> { };

      } // namespace impl

      template<typename...Ps>          using And = value<impl::and_impl<value<Ps> ...>>;
      template<typename...Ps>          using Or = value<impl::or_impl<value<Ps> ...>>;

} // namespace tmeta

#endif // TMETA_IF_ELSE_H