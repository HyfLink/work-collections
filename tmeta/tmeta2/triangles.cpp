#include "tmeta/tmeta.hpp"
using namespace tmeta;

$fn($para Points, $para Lines) countTriangles
{
    $fn($para P1, $para P2) is2connected
    {
        $use($para Line) helper = AND<value<find<Line, P1>>, value<find<Line, P2>>>;
        using value = value<anymap<Lines, helper>>;
    };

    $fn($para P1, $para P2, $para P3) not3connected
    {
        $use($para Line) helper = AND<value<find<Line, P1>>, value<find<Line, P2>>, value<find<Line, P3>>>;
        using value = NOT<anymap<Lines, helper>>;
    };

    $fn($para Triple) isTriangle
    {
        using P1 = value<get<Triple, $0>>;
        using P2 = value<get<Triple, $1>>;
        using P3 = value<get<Triple, $2>>;

        using value = AND<
            is2connected<P1, P2>,
            is2connected<P1, P3>,
            is2connected<P2, P3>,
            not3connected<P1, P2, P3>
        >;
    };

    using value = value<count<map<combination<Points, $3>, isTriangle>, $true>>;
};

#define $10 integer<10>
#define $11 integer<11>
#define $12 integer<12>

int main(void)
{
    using points = value<range<$1, $12>>;
    using lines = list<
        list<$1, $2, $8>,
        list<$1, $3, $7, $9>,
        list<$1, $4, $6, $10>,
        list<$1, $5, $11>,
        list<$2, $3, $4, $5>,
        list<$5, $6, $7, $8>,
        list<$8, $9, $10, $11>>;

    $println(countTriangles<points, lines>);
    return 0;
}