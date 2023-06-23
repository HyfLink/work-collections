#include "tmeta/tmeta.hpp"
using namespace tmeta;

$fn($para N) multiplication
{
private:
    $use($para A) rng = range<$1, succ<A>>;
    $use($para A) rpt = repeat<A, A>;
    $use($para A, $para B) term = tuple<A, B, mul<A, B>, ifelse<eq<A, B>, endl, ends>>;

public:
    using value = value<zip<
        term,
        flatmap<rng, rng<N>>,
        flatmap<rpt, rng<N>>
    >>;
};

int main(void)
{
    $println(multiplication<$9>);
    return 0;
}