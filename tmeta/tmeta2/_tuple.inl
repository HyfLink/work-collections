namespace tmeta
{
    $fn($para... Ts) len<tuple<Ts...>> $return(integer<sizeof...(Ts)>);

    $fn($para... Ts, $para... Us)              extend<tuple<Ts...>, tuple<Us...>>        $return(tuple<Ts..., Us...>);
    $fn($para... Ts, $para... Us, $para... Vs) extend<tuple<Ts...>, tuple<Us...>, Vs...> $return(extend<tuple<Ts..., Us...>, Vs...>);

    $fn($para T, $para... Ts) last<tuple<T, Ts...>> $return(last<tuple<Ts...>>);
    $fn($para T)              last<tuple<T>>        $return(T);
    
    $fn(INT N, $para... Ts) cycle<tuple<Ts...>, integer<N>> $return(extend<tuple<Ts...>, value<cycle<tuple<Ts...>, integer<(N - 1)>>>>);
    $fn($para... Ts)        cycle<tuple<Ts...>, $2>         $return(tuple<Ts..., Ts...>);
    $fn($para... Ts)        cycle<tuple<Ts...>, $1>         $return(tuple<Ts...>);
    $fn($para... Ts)        cycle<tuple<Ts...>, $0>         $return(tuple<>);

    $fn($para T, $para... Ts) maximum<tuple<T, Ts...>> $return(max<T, maximum<tuple<Ts...>>>);
    $fn($para T)              maximum<tuple<T>>        $return(T);
    $fn($para T, $para... Ts) minimum<tuple<T, Ts...>> $return(min<T, minimum<tuple<Ts...>>>);
    $fn($para T)              minimum<tuple<T>>        $return(T);

    $fn($para A, $para B, $para... As, $para... Bs) gt<tuple<A, As...>, tuple<B, Bs...>> $return(gt<A, B>);
    $fn($para T,          $para... As, $para... Bs) gt<tuple<T, As...>, tuple<T, Bs...>> $return(gt<tuple<As...>, tuple<Bs...>>);
    $fn()                                           gt<tuple<>,         tuple<>>         $return($false);
    $fn($para A, $para B, $para... As, $para... Bs) lt<tuple<A, As...>, tuple<B, Bs...>> $return(lt<A, B>);
    $fn($para T, $para... As, $para... Bs)          lt<tuple<T, As...>, tuple<T, Bs...>> $return(lt<tuple<As...>, tuple<Bs...>>);
    $fn()                                           lt<tuple<>,         tuple<>>         $return($false);

    $fn($para T, $para... Ts, $para E) find<tuple<T, Ts...>, E> $return(find<tuple<Ts...>, E>);
    $fn($para T, $para... Ts)          find<tuple<T, Ts...>, T> $return($true);
    $fn($para E)                       find<tuple<>,         E> $return($false);

    $fn(INT RT, $para Tuple, $para E)          __indexof;
    $fn(INT RT, $para T, $para... Ts, $para E) __indexof<RT, tuple<T, Ts...>, E> $return(__indexof<(RT + 1), tuple<Ts...>, E>);
    $fn(INT RT, $para T, $para... Ts)          __indexof<RT, tuple<T, Ts...>, T> $return(integer<RT>);
    $fn(INT RT, $para E)                       __indexof<RT, tuple<>,         E> $return(integer<(-1)>);
    $fn($para... Ts, $para E)                  indexof<tuple<Ts...>, E>          $return(__indexof<0, tuple<Ts...>, E>);

    $fn(INT RT, $para Tuple, $para E)          __count;
    $fn(INT RT, $para T, $para... Ts, $para E) __count<RT, tuple<T, Ts...>, E> $return(__count<RT, tuple<Ts...>, E>);
    $fn(INT RT, $para T, $para... Ts)          __count<RT, tuple<T, Ts...>, T> $return(__count<(RT + 1), tuple<Ts...>, T>);
    $fn(INT RT, $para E)                       __count<RT, tuple<>,         E> $return(integer<RT>);
    $fn($para...Ts, $para E)                   count<tuple<Ts...>, E>          $return(__count<0, tuple<Ts...>, E>);

    $fn(INT N, $para T, $para... Ts)          get<tuple<T, Ts...>, integer<N>>    $return(get<tuple<Ts...>, integer<(N - 1)>>);
    $fn($para T, $para... Ts)                 get<tuple<T, Ts...>, $0>            $return(T);
    $fn(INT N, $para T, $para... Ts, $para E) set<tuple<T, Ts...>, integer<N>, E> $return(extend<tuple<T>, value<set<tuple<Ts...>, integer<(N - 1)>, E>>>);
    $fn($para T, $para... Ts, $para E)        set<tuple<T, Ts...>, $0,         E> $return(tuple<E, Ts...>);

} // namespace tmeta
