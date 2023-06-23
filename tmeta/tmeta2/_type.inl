namespace tmeta
{
    CONST INT __gcd(INT a, INT b)
    {
        while (b ^= a ^= b ^= a %= b);
        return a;
    }

    $fn(BOOL Flag) boolean
    {
        CONST BOOL instance = Flag;

        using boolize = boolean<Flag>;
        using value = boolean<Flag>;
    };

    struct null
    {
        using boolize = $false;
        using value = null;
    };

    $fn(INT N, INT D) number
    {
        CONST INT numer = N / __gcd(N, D);
        CONST INT denom = D / __gcd(N, D);
        CONST INT integer = numer / denom;
        CONST FLOAT instance = FLOAT(numer) / FLOAT(denom);

        using boolize = boolean<numer != 0>;
        using value = number<numer, denom>;
    };

    $fn(INT N) number<N, 1>
    {
        CONST INT numer = N;
        CONST INT denom = 1;
        CONST INT integer = N;
        CONST FLOAT instance = FLOAT(N);

        using boolize = boolean<numer != 0>;
        using value = number<numer, 1>;
    };

    $fn(CHAR... Chars) string
    {
        CONST INT length = sizeof...(Chars);
        CONST CHAR instance[length + 1] = {Chars..., '\0'};

        using boolize = boolean<length != 0>;
        using value = string<Chars...>;
    };

    $fn($para Head, $para Tail) node
    {
        using head = value<Head>;
        using tail = value<Tail>;
        using value = node<head, tail>;
    };

    $fn($para H, $para... Ts) list<H, Ts...> $return(node<H, list<Ts...>>);
    $fn($para H)              list<H>        $return(node<H, null>);

    $fn($para T, $para... Ts) tuple<T, Ts...>
    {
        using head = value<T>;
        using tail = tuple<value<Ts>...>;
        using value = tuple<value<T>, value<Ts>...>;
    };

    $fn() tuple<> { using value = tuple<>; };

    $fn($para X         ) isnull<X>               $return($false);
    $fn($para X         ) isbool<X>               $return($false);
    $fn($para X         ) islist<X>               $return($false);
    $fn($para X         ) istuple<X>              $return($false);
    $fn($para X         ) isstring<X>             $return($false);
    $fn($para X         ) isnumber<X>             $return($false);
    $fn($para X         ) isinteger<X>            $return($false);
    
    $fn(                ) isnull<null>            $return($true);
    $fn(BOOL F          ) isbool<boolean<F>>      $return($true);
    $fn($para A, $para B) islist<node<A, B>>      $return($true);
    $fn($para... Ts     ) istuple<tuple<Ts...>>   $return($true);
    $fn(CHAR... Cs      ) isstring<string<Cs...>> $return($true);
    $fn(INT N, INT D    ) isnumber<number<N, D>>  $return($true);
    $fn(INT N           ) isinteger<integer<N>>   $return($true);

} // namespace tmeta
