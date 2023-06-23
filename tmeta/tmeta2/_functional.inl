namespace tmeta
{
    $fn($parafn Fn, $para...Ts)       map<Fn, tuple<Ts...>> $return(tuple<value<Fn<Ts>>...>);
    $fn($parafn Fn, $para H, $para T) map<Fn, node<H, T>  > $return(node<value<Fn<H>>, map<Fn, T>>);
    $fn($parafn Fn)                   map<Fn, null        > $return(null);

    $fn($parafn Fn, $para... Ts)      allmap<Fn, tuple<Ts...>> $return(AND<boolize<Ts>...>);
    $fn($parafn Fn, $para H, $para T) allmap<Fn, node<H, T>>   $return(all<map<Fn, node<H, T>>>);
    $fn($parafn Fn, $para... Ts)      anymap<Fn, tuple<Ts...>> $return(OR<boolize<Ts>...>);
    $fn($parafn Fn, $para H, $para T) anymap<Fn, node<H, T>>   $return(any<map<Fn, node<H, T>>>);

    $fn($parafn Fn, $para... Ts)      flatmap<Fn, tuple<Ts...>> $return(extend<value<Fn<Ts>>...>);
    $fn($parafn Fn, $para H, $para T) flatmap<Fn, node<H, T>  > $return(extend<value<Fn<H>>, value<flatmap<Fn, T>>>);
    $fn($parafn Fn)                   flatmap<Fn, null        > $return(null);

    $fn($parafn Fn, $para Helper, $para List)   __filter;
    $fn($parafn Fn, $para HD, $para H, $para T) __filter<Fn, $true,  node<HD, node<H, T>>> $return(node<HD, __filter<Fn, value<Fn<H>>, node<H, T>>>);
    $fn($parafn Fn, $para HD, $para H, $para T) __filter<Fn, $false, node<HD, node<H, T>>> $return(__filter<Fn, value<Fn<H>>, node<H, T>>);
    $fn($parafn Fn, $para HD)                   __filter<Fn, $true,  node<HD, null>      > $return(node<HD, null>);
    $fn($parafn Fn, $para HD)                   __filter<Fn, $false, node<HD, null>      > $return(null);
    $fn($parafn Fn, $para H, $para T)           filter<Fn, node<H, T>>                     $return(__filter<Fn, value<Fn<H>>, node<H, T>>);
    
    $fn($parafn Fn, $para T1, $para T2, $para... Ts) reduce<Fn, tuple<T1, T2, Ts...>> $return(reduce<Fn, tuple<Ts...>, value<Fn<T1, T2>>>);
    $fn($parafn Fn, $para T, $para... Ts, $para Acc) reduce<Fn, tuple<T, Ts...>, Acc> $return(reduce<Fn, tuple<Ts...>, value<Fn<Acc, T>>>);
    $fn($parafn Fn, $para Acc)                       reduce<Fn, tuple<>,         Acc> $return(Acc);
    $fn($parafn Fn, $para HD, $para H, $para T)      reduce<Fn, node<HD, node<H, T>>> $return(reduce<Fn, T, value<Fn<HD, H>>>);
    $fn($parafn Fn, $para H, $para T, $para Acc)     reduce<Fn, node<H, T>,      Acc> $return(reduce<Fn, T, value<Fn<Acc, H>>>);
    $fn($parafn Fn, $para Acc)                       reduce<Fn, null,            Acc> $return(Acc);

    $fn($parafn Fn, $para... Ts, $para... Us)               zip<Fn, tuple<Ts...>, tuple<Us...>> $return(tuple<value<Fn<Ts, Us>>...>);
    $fn($parafn Fn, $para H1, $para T1, $para H2, $para T2) zip<Fn, node<H1, T1>, node<H2, T2>> $return(node<value<Fn<H1, H2>>, zip<Fn, T1, T2>>);
    $fn($parafn Fn, $para H1, $para T1)                     zip<Fn, node<H1, T1>, null        > $return(null);
    $fn($parafn Fn, $para H2, $para T2)                     zip<Fn, null,         node<H2, T2>> $return(null);
    $fn($parafn Fn)                                         zip<Fn, null,         null        > $return(null);

    $fn($parafn Fn, $para HD) sort<Fn, node<HD, null>> $return(node<HD, null>);
    $fn($parafn Fn, $para HD, $para TL) sort<Fn, node<HD, TL>>
    {
        $fn($para A, $para List)       ins;
        $fn($para A)                   ins<A, null>       $return(node<A, null>);
        $fn($para A, $para H, $para T) ins<A, node<H, T>> $return(ifelse<Fn<A, H>, node<A, node<H, T>>, node<H, value<ins<A, T>>>>);
        using value = value<ins<HD, value<sort<Fn, TL>>>>;
    };

    $fn($parafn Fn, $para... Ts) apply<Fn, tuple<Ts...>> $return(Fn<value<Ts>...>);

    $fn($parafn Fn)                 compose<Fn>         { $use($para... Args) func = value<Fn<Args...>>; };
    $fn($parafn Fn, $parafn... Fns) compose<Fn, Fns...> { $use($para... Args) func = value<Fn<typename compose<Fns...>::func<Args...>>>; };

    $fn($parafn Fn, $para Arg) bind1st { $use($para... Args)                         func = value<Fn<Arg, Args...>>; };
    $fn($parafn Fn, $para Arg) bind2nd { $use($para Arg1, $para... Args)             func = value<Fn<Arg1, Arg, Args...>>; };
    $fn($parafn Fn, $para Arg) bind3rd { $use($para Arg1, $para Arg2, $para... Args) func = value<Fn<Arg1, Arg2, Arg, Args...>>; };

} // namespace tmeta
