#pragma once
#if !defined(TMETA_LIST_BASIC_H)
#define TMETA_LIST_BASIC_H

#include "types.h"
#include "is_types.h"
#include "list_def.h"

namespace tmeta::impl {

      // [Boolean] judge if list L has sublist S
      template<typename L, typename S>  struct listSublistImpl_HeadAlign;
      template<typename U, typename LV> struct listSublistImpl_HeadAlign<ListNode<U, LV>, ListNode<U, Null>> : True { };
      template<typename U, typename SV> struct listSublistImpl_HeadAlign<ListNode<U, Null>, ListNode<U, SV>> : False { };
      template<typename U>              struct listSublistImpl_HeadAlign<ListNode<U, Null>, ListNode<U, Null>> : True { };
      template<typename LU, typename SU, typename LV, typename SV>
      struct listSublistImpl_HeadAlign<ListNode<LU, LV>, ListNode<SU, SV>> : False { };
      template<typename U, typename LV, typename SV>
      struct listSublistImpl_HeadAlign<ListNode<U, LV>, ListNode<U, SV>> : listSublistImpl_HeadAlign<LV, SV> { };

      template<typename L, typename S>  struct listSublistImpl;
      template<typename S>              struct listSublistImpl<Null, S> : False { };
      template<typename U, typename LV> struct listSublistImpl<ListNode<U, LV>, U> : True { };
      template<typename U, typename LV> struct listSublistImpl<ListNode<U, LV>, ListNode<U, Null>> : True { };
      template<typename LU, typename LV, typename S>
      struct listSublistImpl<ListNode<LU, LV>, S> : listSublistImpl<LV, S> { };
      template<typename U, typename LV, typename SV>
      struct listSublistImpl<ListNode<U, LV>, ListNode<U, SV>> : Or<value<listSublistImpl_HeadAlign<LV, SV>>, listSublistImpl<LV, ListNode<U, SV>>> { };

      // [Boolean] judge if L or any member list in L has element T
      template<typename L, typename T>             struct listOwnImpl;
      template<typename U, typename V, typename T> struct listOwnImpl<ListNode<U, V>, T> : listOwnImpl<V, T> { };
      template<typename U, typename V>             struct listOwnImpl<ListNode<U, V>, U> : True { };
      template<typename T>                         struct listOwnImpl<Null, T> : False { };
      template<typename U, typename V, typename W, typename T>
      struct listOwnImpl<ListNode<ListNode<U, V>, W>, T> : Or<listOwnImpl<W, T>, listOwnImpl<ListNode<U, V>, T>> { };

      // [Integer] get index of T in list, return NaN if not included
      template<typename L, typename T> struct listIndexOfImpl;
      template<typename L, typename T> struct listIndexOfImpl : Number<1 + listIndexOfImpl<cdr<L>, T>::numer, listIndexOfImpl<cdr<L>, T>::denom> { };
      template<typename U, typename V> struct listIndexOfImpl<ListNode<U, V>, U> : Zero { };
      template<typename T>             struct listIndexOfImpl<Null, T> : NaN { };

      // [List] insert T into list at Idx
      template<typename L, typename T, typename Idx> struct listInsertImpl;
      template<typename L, typename T, int64_t Idx>  struct listInsertImpl<L, T, Integer<Idx>> : ListNode<car<L>, listInsertImpl<cdr<L>, T, Integer<Idx - 1>>> { };
      template<typename U, typename V, typename T>   struct listInsertImpl<ListNode<U, V>, T, Zero> : ListNode<T, ListNode<U, V>> { };
      template<typename T, int64_t Idx>              struct listInsertImpl<Null, T, Integer<Idx>> : List<T> { };

      // [List] Replace the first U element in the list with element T
      template<typename L, typename U, typename T> struct listReplaceImpl;
      template<typename L, typename U, typename T> struct listReplaceImpl : ListNode<car<L>, listReplaceImpl<cdr<L>, U, T>> { };
      template<typename U, typename V, typename T> struct listReplaceImpl<ListNode<U, V>, U, T> : ListNode<T, V> { };
      template<typename L, typename U>             struct listReplaceImpl<L, U, U> : value<L> { };
      template<typename U, typename T>             struct listReplaceImpl<Null, U, T> : Null { };

      // [List] Replace all U elements in the list with element T
      template<typename L, typename U, typename T> struct listReplaceAllImpl;
      template<typename L, typename U, typename T> struct listReplaceAllImpl : ListNode<car<L>, listReplaceAllImpl<cdr<L>, U, T>> { };
      template<typename U, typename V, typename T> struct listReplaceAllImpl<ListNode<U, V>, U, T> : ListNode<T, listReplaceAllImpl<V, U, T>> { };
      template<typename L, typename U>             struct listReplaceAllImpl<L, U, U> : value<L> { };
      template<typename U, typename T>             struct listReplaceAllImpl<Null, U, T> : Null { };

      // [List] Reverse list
      template<typename RT, typename L> struct listReverseImpl : listReverseImpl<ListNode<car<L>, RT>, cdr<L>> { };
      template<typename RT> struct listReverseImpl<RT, Null> : RT { };

} // namespace tmeta::impl

namespace tmeta {
      template<typename L, typename S>                using listSublist = value<impl::listSublistImpl<L, S>>;
      template<typename L, typename T>                using listOwn = value<impl::listOwnImpl<L, T>>;
      template<typename L, typename T>                using listIndexOf = value<impl::listIndexOfImpl<L, T>>;
      template<typename L, typename T, typename Idx>  using listInsert = value<impl::listInsertImpl<L, T, Idx>>;
      template<typename L, typename U, typename T>    using listReplace = value<impl::listReplaceImpl<L, U, T>>;
      template<typename L, typename U, typename T>    using listReplaceAll = value<impl::listReplaceAllImpl<L, U, T>>;
      template<typename L>                            using listReverse = value<impl::listReverseImpl<Null, L>>;
} // namespace tmeta

#endif // TMETA_LIST_BASIC_H

