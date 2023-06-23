namespace tmeta
{
    $fn($para HD, $para TL, INT N) take<node<HD, TL>, integer<N>> $return(node<HD, value<take<TL, integer<(N - 1)>>>>);
    $fn($para HD, $para TL)        take<node<HD, TL>, $0        > $return(null);
    $fn(INT N)                     take<null,         integer<N>> $return(null);

    $fn($para HD, $para TL, INT N) drop<node<HD, TL>, integer<N>> $return(drop<TL, integer<(N - 1)>>);
    $fn($para HD, $para TL)        drop<node<HD, TL>, $0        > $return(node<HD, TL>);
    $fn(INT N)                     drop<null,         integer<N>> $return(null);

    $fn($para HD, $para TL)        earse<node<HD, TL>            > $return(TL);
    $fn($para HD, $para TL, INT N) earse<node<HD, TL>, integer<N>> $return(node<HD, value<earse<TL, integer<(N - 1)>>>>);
    $fn($para HD, $para TL)        earse<node<HD, TL>, $0        > $return(TL);
    $fn(INT N)                     earse<null,         integer<N>> $return(null);

    $fn($para HD, $para TL, $para Elem) clear<node<HD, TL>, Elem> $return(node<HD, value<clear<TL, Elem>>>);
    $fn($para HD, $para TL)             clear<node<HD, TL>, HD  > $return(TL);
    $fn($para Elem)                     clear<null,         Elem> $return(null);

    $fn($para HD, $para TL, $para Elem) clearAll<node<HD, TL>, Elem> $return(node<HD, value<clearAll<TL, Elem>>>);
    $fn($para HD, $para TL)             clearAll<node<HD, TL>, HD  > $return(clearAll<TL, HD>);
    $fn($para Elem)                     clearAll<null,         Elem> $return(null);

    $fn($para HD, $para TL, $para Elem)     append<node<HD, TL>, Elem> $return(node<HD, value<append<TL, Elem>>>);
    $fn($para Elem)                         append<null,         Elem> $return(node<Elem, null>);

    $fn($para H1, $para T1, $para H2, $para T2)              extend<node<H1, T1>, node<H2, T2>>        $return(node<H1, value<extend<T1, node<H2, T2>>>>);
    $fn($para H1, $para T1)                                  extend<node<H1, T1>, null        >        $return(node<H1, T1>);
    $fn($para H2, $para T2)                                  extend<null,         node<H2, T2>>        $return(node<H2, T2>);
    $fn()                                                    extend<null,         null        >        $return(null);
    $fn($para H1, $para T1, $para H2, $para T2, $para... Ls) extend<node<H1, T1>, node<H2, T2>, Ls...> $return(extend<extend<node<H1, T1>, node<H2, T2>>, Ls...>);

    $fn($para HD, $para TL, $para A, $para B) replace<node<HD, TL>, A, B> $return(node<HD, value<replace<TL, A, B>>>);
    $fn($para A, $para TL, $para B)           replace<node<A,  TL>, A, B> $return(node<B, TL>);
    $fn($para A, $para B)                     replace<null,         A, B> $return(null);

    $fn($para HD, $para TL, $para A, $para B) replaceAll<node<HD, TL>, A, B> $return(node<HD, value<replaceAll<TL, A, B>>>);
    $fn($para A, $para TL, $para B)           replaceAll<node<A,  TL>, A, B> $return(node<B, value<replaceAll<TL, A, B>>>);
    $fn($para A, $para B)                     replaceAll<null,         A, B> $return(null);
    
    $fn($para HD, $para TL, $para Elem)        insert<node<HD, TL>, Elem         > $return(node<Elem, node<HD, TL>>);
    $fn($para HD, $para TL, $para Elem, INT N) insert<node<HD, TL>, Elem, integer<N>> $return(node<HD, value<insert<TL, Elem, integer<(N - 1)>>>>);
    $fn($para HD, $para TL, $para Elem)        insert<node<HD, TL>, Elem, $0     > $return(node<Elem, node<HD, TL>>);
    $fn($para Elem, INT N)                     insert<null,         Elem, integer<N>> $return(node<Elem, null>);


} // namespace tmeta
