namespace tmeta
{
    $fn(INT N, INT D) nag<number<N, D>> $return(number<(-N), D>);
    $fn(INT N)        succ<integer<N>>  $return(integer<N + 1>);
    $fn(INT N)        pred<integer<N>>  $return(integer<N - 1>);

    $fn(INT N1, INT N2) add<integer<N1>, integer<N2>> $return(integer<N1 + N2>);
    $fn(INT N1, INT N2) sub<integer<N1>, integer<N2>> $return(integer<N1 - N2>);
    $fn(INT N1, INT N2) mul<integer<N1>, integer<N2>> $return(integer<N1 *N2>);
    $fn(INT N1, INT N2) div<integer<N1>, integer<N2>> $return(number<N1, N2>);
    $fn(INT N1, INT N2) mod<integer<N1>, integer<N2>> $return(integer<N1 % N2>);
    $fn(INT N1, INT N2) max<integer<N1>, integer<N2>> $return(integer<(N1 > N2 ? N1 : N2)>);
    $fn(INT N1, INT N2) min<integer<N1>, integer<N2>> $return(integer<(N1 < N2 ? N1 : N2)>);
    $fn(INT N1, INT N2) gt <integer<N1>, integer<N2>> $return(boolean<(N1 > N2)>);
    $fn(INT N1, INT N2) lt <integer<N1>, integer<N2>> $return(boolean<(N1 < N2)>);

    $fn(INT N1, INT D1, INT N2, INT D2) add<number<N1, D1>, number<N2, D2>> $return(number<N1 * D2 + N2 * D1, D1 * D2>);
    $fn(INT N1, INT D1, INT N2, INT D2) sub<number<N1, D1>, number<N2, D2>> $return(number<N1 * D2 - N2 * D1, D1 * D2>);
    $fn(INT N1, INT D1, INT N2, INT D2) mul<number<N1, D1>, number<N2, D2>> $return(number<N1 * N2, D1 * D2>);
    $fn(INT N1, INT D1, INT N2, INT D2) div<number<N1, D1>, number<N2, D2>> $return(number<N1 * D2, D1 * N2>);
    $fn(INT N1, INT D1, INT N2, INT D2) max<number<N1, D1>, number<N2, D2>> $return(number<(N1 * D2 > N2 * D1) ? N1 : N2, (N1 * D2 > N2 * D1) ? D1 : D2>);
    $fn(INT N1, INT D1, INT N2, INT D2) min<number<N1, D1>, number<N2, D2>> $return(number<(N1 * D2 < N2 * D1) ? N1 : N2, (N1 * D2 < N2 * D1) ? D1 : D2>);
    $fn(INT N1, INT D1, INT N2, INT D2) gt <number<N1, D1>, number<N2, D2>> $return(boolean<(N1 * D2 > N2 * D1)>);
    $fn(INT N1, INT D1, INT N2, INT D2) lt <number<N1, D1>, number<N2, D2>> $return(boolean<(N1 * D2 < N2 * D1)>);


    static constexpr INT __highestbit(INT x)
    {
        INT n = 1;
        if (!(x >> 32)) { n += 32; x <<= 32; }
        if (!(x >> 48)) { n += 16; x <<= 16; }
        if (!(x >> 56)) { n += 8;  x <<= 8;  }
        if (!(x >> 60)) { n += 4;  x <<= 4;  }
        if (!(x >> 62)) { n += 2;  x <<= 2;  }
        if (!(x >> 63)) { n += 1;  x <<= 1;  }
        return 65 - n; 
    }

    $fn($para N, $para R, INT I) __sqrt
    {
        static constexpr inline INT a = (N::denom * R::numer * R::numer + N::numer * R::denom * R::denom);
        static constexpr inline INT b = (2 * N::denom * R::numer * R::denom);
        static constexpr inline INT d = __highestbit(a < b ? a : b) - 20;
        static constexpr inline INT RN = (d > 0 ? a >> d : a);
        static constexpr inline INT RD = (d > 0 ? b >> d : b);
        using value = value<__sqrt<N, number<RN, RD>, (I - 1)>>;
    };
    $fn($para N, $para R) __sqrt<N, R, 0> $return(R);
    $fn($para R, INT I) __sqrt<$0, R, I> $return($0);
    $fn(INT N, INT D) sqrt<number<N, D>> $return(__sqrt<number<N, D>, number<N, D>, 50>);

} // namespace tmeta
