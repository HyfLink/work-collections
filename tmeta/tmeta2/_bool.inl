namespace tmeta
{
    $fn($para X, $para Y) __eq<X, Y>  $return($false);
    $fn($para X)          __eq<X, X>  $return($true);
    
    $fn(BOOL... Qs) __and<boolean<Qs>...> $return(boolean<(Qs && ...)>);
    $fn(BOOL... Qs) __or<boolean<Qs>...>  $return(boolean<(Qs || ...)>);

    $fn($para C, $para T, $para E) ifelse<C,      T, E> $return(ifelse<boolize<C>, T, E>);
    $fn($para T, $para E)          ifelse<$true,  T, E> $return(T);
    $fn($para T, $para E)          ifelse<$false, T, E> $return(E);

    $fn($para C, $para T, $para... Cs) cond<C,      T, Cs...> $return(cond<boolize<C>, T, Cs...>);
    $fn($para T, $para... Cs)          cond<$false, T, Cs...> $return(cond<Cs...>);
    $fn($para T, $para... Cs)          cond<$true,  T, Cs...> $return(T);
    $fn($para T, $para E)              cond<$false, T, E>     $return(E);

} // namespace tmeta
