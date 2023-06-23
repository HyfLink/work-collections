namespace tmeta
{
    $fn($para H, $para T) len<node<H, T>> $return(succ<value<len<T>>>);
    $fn()                 len<null>       $return($0);

    $fn($para H, $para T) last<node<H,    T   >> $return(last<T>);
    $fn($para Last)       last<node<Last, null>> $return(Last);

    $fn($para H, $para T) all<node<H,      T  >> $return(all<node<boolize<H>, T>>);
    $fn($para T)          all<node<$true,  T  >> $return(all<T>);
    $fn($para T)          all<node<$false, T  >> $return($false);
    $fn()                 all<node<$true, null>> $return($true);
    $fn()                 all<null>              $return($false);
    $fn($para H, $para T) any<node<H,      T>> $return(any<node<boolize<H>, T>>);
    $fn($para T)          any<node<$false, T>> $return(any<T>);
    $fn($para T)          any<node<$true,  T>> $return($true);
    $fn()                 any<null>            $return($false);

    $fn($para H, $para T) maximum<node<H, T      >> $return(max<H, value<maximum<T>>>);
    $fn($para Last)       maximum<node<Last, null>> $return(Last);
    $fn($para H, $para T) minimum<node<H, T      >> $return(min<H, value<minimum<T>>>);
    $fn($para Last)       minimum<node<Last, null>> $return(Last);

    $fn($para H1, $para T1, $para H2, $para T2) gt<node<H1, T1>, node<H2, T2>> $return(gt<H1, H2>);
    $fn($para H, $para T1, $para T2)            gt<node<H,  T1>, node<H,  T2>> $return(gt<T1, T2>);
    $fn($para H1, $para T1, $para H2, $para T2) lt<node<H1, T1>, node<H2, T2>> $return(lt<H1, H2>);
    $fn($para H, $para T1, $para T2)            lt<node<H,  T1>, node<H,  T2>> $return(lt<T1, T2>);

    $fn($para H, $para T, $para E) find<node<H, T>, E> $return(find<T, E>);
    $fn($para H, $para T)          find<node<H, T>, H> $return($true);
    $fn($para E)                   find<null,       E> $return($false);

    $fn(INT RT, $para List, $para E)       __indexof;
    $fn(INT RT, $para H, $para T, $para E) __indexof<RT, node<H, T>, E> $return(__indexof<RT + 1, T, E>);
    $fn(INT RT, $para H, $para T)          __indexof<RT, node<H, T>, H> $return(integer<RT>);
    $fn(INT RT, $para E)                   __indexof<RT, null,       E> $return(integer<(-1)>);
    $fn($para List, $para E)               indexof<List, E>             $return(__indexof<0, List, E>);

    $fn(INT RT, $para List, $para E)       __count;
    $fn(INT RT, $para H, $para T, $para E) __count<RT, node<H, T>, E> $return(__count<RT, T, E>);
    $fn(INT RT, $para H, $para T)          __count<RT, node<H, T>, H> $return(__count<(RT + 1), T, H>);
    $fn(INT RT, $para E)                   __count<RT, null,       E> $return(integer<RT>);
    $fn($para H, $para T, $para E)         count<node<H, T>, E>       $return(__count<0, node<H, T>, E>);

    $fn($para H, $para T, INT N)          get<node<H, T>, integer<N>>    $return(get<T, integer<N - 1>>);
    $fn($para H, $para T)                 get<node<H, T>, $0        >    $return(H);
    $fn(INT N)                            get<null,       integer<N>>    $return(null);
    $fn($para H, $para T, INT N, $para E) set<node<H, T>, integer<N>, E> $return(node<H, value<set<T, integer<(N - 1)>, E>>>);
    $fn($para H, $para T, $para E)        set<node<H, T>, $0,         E> $return(node<E, T>);
    $fn(INT N, $para E)                   set<null,       integer<N>, E> $return(null);


} // namespace tmeta
