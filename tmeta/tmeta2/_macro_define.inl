// keywords
#define $def(...)       template<__VA_ARGS__>
#define $return(...)    : public __VA_ARGS__ { }
#define $para           typename
#define $parafn         $def($para... Args) $para
#define $fn(...)        $def(__VA_ARGS__) struct
#define $declfn(X)      $fn($para... Args) X $return(X<value<Args>...>)
#define $use(...)       $def(__VA_ARGS__) using

// types
#define $int(X)         tmeta::integer<(X)>
#define $str(x)         $str64(x)

// functions
#define $print(...)     tmeta::print<__VA_ARGS__>()
#define $println(...)   tmeta::print<__VA_ARGS__, endl>()
#define $printsp(...)   tmeta::print<__VA_ARGS__, ends>()

// tmeta macros
#define CONST           static constexpr inline
#define BOOL            bool
#define CHAR            char
#define FLOAT           double
#define INT             long long
