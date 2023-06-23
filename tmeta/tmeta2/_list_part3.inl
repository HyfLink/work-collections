namespace tmeta
{
    $fn($para RT, $para List)         __reverse;   
    $fn($para RT, $para HD, $para TL) __reverse<RT, node<HD, TL>> $return(__reverse<node<HD, RT>, TL>);
    $fn($para RT)                     __reverse<RT, null        > $return(RT);
    $fn($para HD, $para TL)           reverse<node<HD, TL>>       $return(__reverse<null, node<HD, TL>>);

    $fn(BOOL F, INT B, INT E, INT S) __range                                   $return(null);
    $fn(INT B, INT E, INT S)         __range<true, B, E, S>                    $return(node<integer<B>, __range<(S > 0) ? (B + S < E) : (B + S > E), B + S, E, S>>);
    $fn(INT E)                       range<integer<E>                        > $return(__range<true, 0, E, (E < 0 ? -1 : 1)>);
    $fn(INT B, INT E)                range<integer<B>, integer<E>            > $return(__range<true, B, E, (E < B ? -1 : 1)>);
    $fn(INT B, INT E, INT S)         range<integer<B>, integer<E>, integer<S>> $return(__range<true, B, E, S>);

    $fn($para HD, $para TL)  unique<node<HD, TL>> $return(node<HD, unique<value<clearAll<TL, HD>>>>);
    $fn()                    unique<null        > $return(null);

    $fn($para List, INT N)   cycle<List, integer<N>> $return(extend<List, value<cycle<List, integer<(N - 1)>>>>);
    $fn($para List)          cycle<List, $0        > $return(null);
    
    $fn($para Elem, INT N)   repeat<Elem, integer<N>> $return(node<Elem, repeat<Elem, integer<(N - 1)>>>);
    $fn($para Elem)          repeat<Elem, $0        > $return(null);

    $fn($para List) combination<List, $0> $return(null);
    $fn($para List) combination<List, $1> $return(map<list, List>);
    $fn($para List) combination<List, integer<len<List>::integer>> $return(list<List>);
    $fn($para List, INT N) combination<List, integer<N>>
    {
    private:
        $use($para T) addhd = node<head<List>, T>;
        using part1 = value<combination<tail<List>, integer<N>>>;
        using part2 = value<map<addhd, combination<tail<List>, integer<(N - 1)>>>>;

    public:
        using value = value<extend<part1, part2>>;
    };

    $fn($para Last) permutation<node<Last, null>> $return(node<node<Last, null>, null>);
    $fn($para List) permutation<List>
    {
    private:
        $fn($para Elem) helper {
            $use($para U) addelem = node<Elem, U>;
            using value = value<map<addelem, value<permutation<value<clear<List, Elem>>>>>>;
        };

    public:
        using value = value<flatmap<helper, List>>;
    };

} // namespace tmeta
