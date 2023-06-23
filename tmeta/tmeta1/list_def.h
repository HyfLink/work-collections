#pragma once
#if !defined(TMETA_LIST_DEF_H)
#define TMETA_LIST_DEF_H

#include "types.h"
#include "is_types.h"
#include "if_else.h"

namespace tmeta {

      // list type definition
      // --------------------

      template<typename U, typename V>
      struct ListNode {
            using __tags__ = impl::__list_tags;
            using __car__ = typename U::__value__;
            using __cdr__ = typename V::__value__;
            using __value__ = ListNode<__car__, __cdr__>;
            static constexpr int64_t __len__ = 1 + __cdr__::__len__;

            template<typename T> using appendLeft = ListNode<T, __value__>;
            template<typename T> using append = ListNode<__car__, typename __cdr__::append<T>>;
            template<typename L> using extend = ListNode<__car__, typename __cdr__::extend<L>>;
            template<typename N> using at = if_not_else<N, __car__, typename __cdr__::at<typename N::dec>>;
            template<typename T> using has = Or<isSame<U, __car__>, typename __cdr__::has<T>>;
            template<typename N> using erase = if_else<N, ListNode<__car__, typename __cdr__::erase<typename N::dec>>, __cdr__>;
            template<typename T> using remove = if_else<isSame<T, __car__>, __cdr__, ListNode<__car__, typename __cdr__::remove<T>>>;
            template<typename T> using removeAll = if_else<isSame<T, __car__>, typename __cdr__::removeAll<T>, ListNode<__car__, typename __cdr__::removeAll<T>>>;
            using popLeft = __cdr__;
            using pop = ListNode<__car__, typename __cdr__::pop>;
            using unique = ListNode<__car__, typename  __cdr__::removeAll<__car__>>;
      };

      template<typename U>
      struct ListNode<U, Null> {
            using __tags__ = impl::__list_tags;
            using __car__ = typename U::__value__;
            using __cdr__ = Null;
            using __value__ = ListNode<__car__, __cdr__>;
            static constexpr int64_t __len__ = 1;

            template<typename T> using appendLeft = ListNode<T, __value__>;
            template<typename T> using append = ListNode<__car__, ListNode<T, Null>>;
            template<typename L> using extend = ListNode<__car__, L>;
            template<typename N> using at = if_else<N, Null, __car__>;
            template<typename T> using has = isSame<T, __car__>;
            template<typename N> using erase = if_else<N, __value__, __cdr__>;
            template<typename T> using remove = if_else<isSame<T, __car__>, __cdr__, __value__>;
            template<typename T> using removeAll = if_else<isSame<T, __car__>, __cdr__, __value__>;
            using popLeft = Null;
            using pop = Null;
            using unique = Null;

      };

      namespace impl {

            template<typename...Ts>             struct list_impl;
            template<typename T>                struct list_impl<T> : ListNode<T, Null> { };
            template<typename T, typename...Ts> struct list_impl<T, Ts...> : ListNode<T, list_impl<Ts...>> { };

      } // namespace impl

      template<typename...Ts> using List = value<impl::list_impl<Ts...>>;
      template<int64_t...Ns>  using IntegerList = List<Integer<Ns>...>;
      template<bool...Bs>     using BooleanList = List<Boolean<Bs>...>;

} // namespace tmeta

#endif // TMETA_LIST_DEF_H

