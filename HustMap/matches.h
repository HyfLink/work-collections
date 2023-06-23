#pragma once

template <typename FrIter1, typename FrIter2>
bool matches(FrIter1 first1, FrIter1 last1, FrIter2 first2, FrIter2 last2) {
    for (; first2 != last2; ++first2) {
        while (first1 < last1 && *first1 != *first2) ++first1;
        if (first1 == last1) return false;
    }
    return true;
}
