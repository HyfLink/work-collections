/*
 * C++ template meta programing library
 */

#pragma once
#include "_macro_define.inl"

namespace tmeta
{
    /******************** types ********************/

    struct null;
    $fn(BOOL F)           boolean;
    $fn(CHAR... Cs)       string;
    $fn(INT N, INT D)     number;
    $fn($para A, $para B) node;
    $fn($para... Ts)      list;
    $fn($para... Ts)      tuple;
    $use(INT N)           integer = number<N, 1>;

    /******************** constants ********************/

    typedef boolean<true> $true;
    typedef boolean<false> $false;
    typedef string<' '> ends;
    typedef string<'\n'> endl;
    typedef integer<0> $0;
    typedef integer<1> $1;
    typedef integer<2> $2;
    typedef integer<3> $3;
    typedef integer<4> $4;
    typedef integer<5> $5;
    typedef integer<6> $6;
    typedef integer<7> $7;
    typedef integer<8> $8;
    typedef integer<9> $9;

    /******************** basic ********************/

    $use($para X) value = typename X::value;
    $use($para X) boolize = typename X::value::boolize;
    $use($para X) head = typename X::head;
    $use($para X) tail = typename X::tail;

    $declfn(ifelse); // bool, any, any -> any
    $declfn(cond);   // [bool, any], ..., any -> any

    $declfn(__and); // bool, ... -> bool
    $declfn(__or);  // bool, ... -> bool
    $use($para... Args) NOT = boolean<!boolize<Args>::instance...>;
    $use($para... Args) AND = value<__and<boolize<Args>...>>;
    $use($para... Args) OR = value<__or<boolize<Args>...>>;

    $declfn(__eq); // any, any -> bool
    $use($para... Args) eq = value<__eq<value<Args>...>>;

    $declfn(gt);  // [override] any, any -> bool
    $declfn(lt);  // [override] any, any -> bool
    $use($para... Args) gteq = OR<eq<Args...>, gt<Args...>>;
    $use($para... Args) lteq = OR<eq<Args...>, lt<Args...>>;

    $declfn(max); // [override] any, any -> any
    $declfn(min); // [override] any, any -> any
    $fn($para A, $para B) max<A, B> $return(ifelse<gt<A, B>, A, B>);
    $fn($para A, $para B) min<A, B> $return(ifelse<lt<A, B>, A, B>);

    /******************** print ********************/

    $def($para... Args) void print();

    /******************** istypes ********************/

    $declfn(isnull);    // any -> bool
    $declfn(isbool);    // any -> bool
    $declfn(islist);    // any -> bool
    $declfn(istuple);   // any -> bool
    $declfn(isnumber);  // any -> bool
    $declfn(isstring);  // any -> bool
    $declfn(isinteger); // any -> bool

    /******************** sequence ********************/

    $declfn(len);     // [override] seq -> int : get length of the sequence
    $declfn(maximum); // [override] seq -> any : max<elem, max<elem...>>
    $declfn(minimum); // [override] seq -> any : min<elem, min<elem...>>

    $declfn(last);    // [override] seq -> any : get last elem
    $declfn(extend);  // [override] seq, ... -> seq : merge several sequences
    $declfn(cycle);   // [override] seq, int -> seq : repeat sequence N times

    $declfn(set);     // [override] seq, int, any -> seq : set N-th elem
    $declfn(get);     // [override] seq, int -> any : get N-th elem
    $declfn(find);    // [override] seq, elem -> bool : is elem in the sequence
    $declfn(indexof); // [override] seq, elem -> int : find first elem equals to E
    $declfn(count);   // [override] seq, elem -> int : count E in sequence


    /******************** string ********************/

    $declfn(isdigit);  // char -> bool
    $declfn(isupper);  // char -> bool
    $declfn(islower);  // char -> bool
    $declfn(isalpha);  // char -> bool
    $declfn(isalnum);  // char -> bool
    $declfn(toupper);  // str -> str
    $declfn(tolower);  // str -> str

    /******************** arithmetic ********************/

    $declfn(nag);  // num -> num
    $declfn(add);  // num, num -> num
    $declfn(sub);  // num, num -> num
    $declfn(mul);  // num, num -> num
    $declfn(div);  // num, num -> num
    $declfn(mod);  // int, int -> int
    $declfn(sqrt); // num -> num
    $declfn(succ); // int -> int
    $declfn(pred); // int -> int

    $use($para N) denom = integer<value<N>::denom>;
    $use($para N) numer = integer<value<N>::numer>;

    /******************** tuple ********************/


    /******************** list ********************/

    $declfn(all);         // list -> bool : AND<elem1, elem2, ...>
    $declfn(any);         // list -> bool : OR <elem1, elem2, ...>
    $declfn(take);        // list, int  -> list : get first N elems int the list
    $declfn(drop);        // list, int  -> list : get list without first N elems
    $declfn(insert);      // list, any  -> list : add elem to the begining of list
                          // list, any, int -> list : add elem to the pos N of list
    $declfn(earse);       // list       -> list : delete the first elem
                          // list, int  -> list : delete the N-th elem
    $declfn(append);      // list, any  -> list : append elem to end of list
    $declfn(clear);       // list, any  -> list : clear first elem equals to E
    $declfn(clearAll);    // list, any  -> list : clear all elems equal to E
    $declfn(replace);     // list, any, any -> list : replace first elem A with B
    $declfn(replaceAll);  // list, any, any -> list : replace all elems A with B
    $declfn(reverse);     // list -> list : reverse the list
    $declfn(unique);      // list -> list : de-duplicate the list
    $declfn(range);       // int           -> list[int] : [  0  ...end), step = 1
                          // int, int      -> list[int] : [begin...end), step = 1
                          // int, int, int -> list[int] : [begin...end), step = step
    $declfn(repeat);      // any, int -> list : repeat elem N times
    $declfn(combination); // list, int -> list[list] : get all combinations of list for N
    $declfn(permutation); // list -> list[list] : generate full permutation

    /******************** functional ********************/

    // func[any->any], seq -> seq : use func on each elem in the seq
    $fn($parafn Fn, $para... Args) map $return(map<Fn, value<Args>...>);

    // func[any->seq], seq -> seq : reduce<map<func, seq>, extend, null>
    $fn($parafn Fn, $para... Args) flatmap $return(flatmap<Fn, value<Args>...>);

    // func[any->bool], seq -> bool : all<map<seq, fn>>
    $fn($parafn Fn, $para... Args) allmap $return(allmap<Fn, value<Args>...>);

    // func[any->bool], seq -> bool : any<map<seq, fn>>
    $fn($parafn Fn, $para... Args) anymap $return(anymap<Fn, value<Args>...>);

    // func[any->bool], seq -> seq : remove elem if func(elem) is false
    $fn($parafn Fn, $para... Args) filter $return(filter<Fn, value<Args>...>);

    // func[any, any->any], seq, [any] -> any : fold algorithm
    $fn($parafn Fn, $para... Args) reduce $return(reduce<Fn, value<Args>...>);

    // func[any, any->any], seq, seq -> seq : use func on each pair of elems in two seqs
    $fn($parafn Fn, $para... Args) zip $return(zip<Fn, value<Args>...>);

    // func, seq[any, any->bool] -> seq : sort seq by func
    $fn($parafn Fn, $para... Args) sort $return(sort<Fn, value<Args>...>);

    // func, tuple -> any : apply elems to func
    $fn($parafn Fn, $para Tuple) apply $return(apply<Fn, value<Tuple>>);

    // func... -> func : compose functions
    $fn($parafn... Fn) compose;

    // func, any -> func : bind function with an argument
    $fn($parafn Fn, $para Arg) bind1st;
    $fn($parafn Fn, $para Arg) bind2nd;
    $fn($parafn Fn, $para Arg) bind3rd;
}

#include "_type.inl"
#include "_print.inl"
#include "_bool.inl"
#include "_string.inl"
#include "_arithmetic.inl"
#include "_tuple.inl"
#include "_list_part1.inl"
#include "_list_part2.inl"
#include "_list_part3.inl"
#include "_functional.inl"

#include "_macro_undef.inl"
