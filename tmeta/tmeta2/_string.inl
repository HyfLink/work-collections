namespace tmeta
{
    $def(int N, int M)
    constexpr CHAR __tygrab(const CHAR (&c)[M]) noexcept { return N < M ? c[N] : '\0'; }

    $def(CHAR... X)
    auto __typoke(string<X...>) -> string<X...>;

    $def(CHAR... X, CHAR... Y)
    auto __typoke(string<X...>, string<'\0'>, string<Y>...) -> string<X...>;

    $def(CHAR A, CHAR... X, CHAR... Y)
    auto __typoke(string<X...>, string<A>, string<Y>...) -> decltype(__typoke(string<X..., A>(), string<Y>()...));

    $def(CHAR... C)
    auto __typeek(string<C...>) -> decltype(__typoke(string<C>()...));

    CONST BOOL __isdigit(CHAR C) { return ('0' <= C && C <= '9'); }
    CONST BOOL __islower(CHAR C) { return ('a' <= C && C <= 'z'); }
    CONST BOOL __isupper(CHAR C) { return ('A' <= C && C <= 'Z'); }
    CONST BOOL __isalpha(CHAR C) { return __islower(C) || __isupper(C); }
    CONST BOOL __isalnum(CHAR C) { return __islower(C) || __isupper(C) || __isdigit(C); }
    CONST CHAR __tolower(CHAR C) { return __isupper(C) ? (C + 'a' - 'A') : (C); }
    CONST CHAR __toupper(CHAR C) { return __islower(C) ? (C + 'A' - 'a') : (C); }
}

namespace tmeta
{
    /********** char **********/

    $fn(CHAR C) isdigit<string<C>> $return(boolean<__isdigit(C)>);
    $fn(CHAR C) islower<string<C>> $return(boolean<__islower(C)>);
    $fn(CHAR C) isupper<string<C>> $return(boolean<__isupper(C)>);
    $fn(CHAR C) isalpha<string<C>> $return(boolean<__isalpha(C)>);
    $fn(CHAR C) isalnum<string<C>> $return(boolean<__isalnum(C)>);
    $fn(CHAR C) tolower<string<C>> $return(string<__tolower(C)>);
    $fn(CHAR C) toupper<string<C>> $return(string<__toupper(C)>);

    /********** string **********/

    $fn(CHAR... As, CHAR... Bs) extend<string<As...>, string<Bs...>> $return(string<As..., Bs...>);

    $fn(CHAR A, CHAR... As, CHAR B, CHAR... Bs) gt<string<A, As...>, string<B, Bs...>> $return(boolean<(A > B)>);
    $fn(CHAR A, CHAR... As, CHAR... Bs)         gt<string<A, As...>, string<A, Bs...>> $return(gt<string<As ...>, string<Bs ...>>);
    $fn(CHAR... Bs)                             gt<string<>, string<Bs...>>            $return($false);
    $fn(CHAR... As)                             gt<string<As...>, string<>>            $return($true);
    $fn()                                       gt<string<>, string<>>                 $return($false);

    $fn(CHAR A, CHAR... As, CHAR B, CHAR... Bs) lt<string<A, As...>, string<B, Bs...>> $return(boolean<(A < B)>);
    $fn(CHAR A, CHAR... As, CHAR... Bs)         lt<string<A, As...>, string<A, Bs...>> $return(gt<string<As ...>, string<Bs ...>>);
    $fn(CHAR... Bs)                             lt<string<>, string<Bs...>>            $return($true);
    $fn(CHAR... As)                             lt<string<As...>, string<>>            $return($false);
    $fn()                                       lt<string<>, string<>>                 $return($false);

    $fn(CHAR... Cs) len<string<Cs...>> $return(integer<sizeof...(Cs)>);

    $fn(CHAR... Cs, INT N) get<string<Cs...>, integer<N>> $return(string<(N < sizeof...(Cs) ? string<Cs...>::instance[N] : '\0')>);

    $fn(CHAR... Cs, INT N) cycle<string<Cs...>, integer<N>> $return(extend<string<Cs...>, value<cycle<string<Cs...>, integer<(N - 1)>>>>);
    $fn(CHAR... Cs)        cycle<string<Cs...>, $0        > $return(string<>);

    $fn(CHAR... Cs) tolower<string<Cs...>> $return(string<__tolower(Cs)...>);
    $fn(CHAR... Cs) toupper<string<Cs...>> $return(string<__toupper(Cs)...>);

} // namespace tmeta

#define __TYPESTRING002(n, x) tmeta::__tygrab<0b##n##0>(x), tmeta::__tygrab<0b##n##1>(x)
#define __TYPESTRING004(n, x) __TYPESTRING002(n##0, x), __TYPESTRING002(n##1, x)
#define __TYPESTRING008(n, x) __TYPESTRING004(n##0, x), __TYPESTRING004(n##1, x)
#define __TYPESTRING016(n, x) __TYPESTRING008(n##0, x), __TYPESTRING008(n##1, x)
#define __TYPESTRING032(n, x) __TYPESTRING016(n##0, x), __TYPESTRING016(n##1, x)
#define __TYPESTRING064(n, x) __TYPESTRING032(n##0, x), __TYPESTRING032(n##1, x)
#define __TYPESTRING128(n, x) __TYPESTRING064(n##0, x), __TYPESTRING064(n##1, x)
#define __TYPESTRING256(n, x) __TYPESTRING128(n##0, x), __TYPESTRING128(n##1, x)
#define __TYPESTRING512(n, x) __TYPESTRING256(n##0, x), __TYPESTRING256(n##1, x)

#define $str2(x)   decltype(tmeta::__typeek(string<__TYPESTRING002(, x)>()))
#define $str4(x)   decltype(tmeta::__typeek(string<__TYPESTRING004(, x)>()))
#define $str8(x)   decltype(tmeta::__typeek(string<__TYPESTRING008(, x)>()))
#define $str16(x)  decltype(tmeta::__typeek(string<__TYPESTRING016(, x)>()))
#define $str32(x)  decltype(tmeta::__typeek(string<__TYPESTRING032(, x)>()))
#define $str64(x)  decltype(tmeta::__typeek(string<__TYPESTRING064(, x)>()))
#define $str128(x) decltype(tmeta::__typeek(string<__TYPESTRING128(, x)>()))
#define $str256(x) decltype(tmeta::__typeek(string<__TYPESTRING128(, x)>()))
#define $str512(x) decltype(tmeta::__typeek(string<__TYPESTRING128(, x)>()))
