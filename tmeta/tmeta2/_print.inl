#ifdef TMETA_USE_IOSTREAM
#   include <iostream>
#   define __pstr(s)    std::cout << (s)
#   define __pchar(c)   std::cout << (c)
#   define __pllint(i)  std::cout << (i)
#   define __pfloat(f)  std::cout << (f)
#   define __pends()   std::cout.put(' ') 
#   define __pendl()    std::cout << std::endl
#else // NOT TMETA_USE_IOSTREAM
#   include <cstdio>
#   define __pstr(s)    std::fputs((s), stdout)
#   define __pchar(c)   std::putchar(c)
#   define __pllint(i)  std::printf("%lld", (i))
#   define __pfloat(f)  std::printf("%lf", (f))
#   define __pends()   std::putchar(' ')
#   define __pendl()    std::putchar('\n')
#endif // TMETA_USE_IOSTREAM

namespace tmeta
{
    $fn($para... Ts) __print;

    $def($para... Args) void print() { __print<value<Args>...>()(); }


    $fn($para T, $para U, $para... Ts) __print<T, U, Ts...>
    {
        void operator()()
        {
            __print<T>()();
            __print<U>()();
            __print<Ts...>()();
        }
    };

    $fn() __print<>       { void operator()() { } };
    $fn() __print<null>   { void operator()() { __pstr("null"); } };
    $fn() __print<$true>  { void operator()() { __pstr("true"); } };
    $fn() __print<$false> { void operator()() { __pstr("false"); } };
    $fn() __print<endl>   { void operator()() { __pendl(); } };
    $fn() __print<ends>   { void operator()() { __pends(); } };

    $fn(INT N)        __print<integer<N>>   { void operator()() { __pllint($int(N)::integer); } };
    $fn(INT N, INT D) __print<number<N, D>> { void operator()() { __pfloat((number<N, D>::instance)); } };

    $fn(CHAR... Cs) __print<string<Cs...>>
    {
        void operator()()
        {
            __pchar('"');
            __pstr(string<Cs...>::instance);
            __pchar('"');
        }
    };

    $fn($para A, $para B, $para C) __print<node<A, node<B, C>>>
    {
        void operator()(bool bracket = true)
        {
            if (bracket) __pchar('[');
            __print<A>()();
            __pstr(", ");
            __print<node<B, C>>()(false);
            if (bracket) __pchar(']');
        }
    };


    $fn($para A) __print<node<A, null>>
    {
        void operator()(bool bracket = true)
        {
            if (bracket) __pchar('[');
            __print<A>()();
            if (bracket) __pchar(']');
        }
    };

    $fn($para T, $para... Ts) __print<tuple<T, Ts...>>
    {
        void operator()(bool bracket = true)
        {
            if (bracket) __pchar('(');
            __print<T>()();
            __pstr(", ");
            __print<tuple<Ts...>>()(false);
            if (bracket) __pchar(')');
        }
    };

    $fn($para T) __print<tuple<T>>
    {
        void operator()(bool bracket = true)
        {
            if (bracket) __pchar('(');
            __print<T>()();
            if (bracket) __pchar(')');
        }
    };

    $fn() __print<tuple<>>
    {
        void operator()(bool bracket = true)
        {
            if (bracket) __pstr("()");
        }
    };
} // namespace tmeta

#undef __pstr
#undef __pchar
#undef __pllint
#undef __pfloat
#undef __pends
#undef __pendl