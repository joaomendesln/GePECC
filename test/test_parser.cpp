#include "test_parser.h"
#include <iostream>

using namespace std;

void test_parser() {

    string string_test = "∈ (x, ∪(a, b))";
    // string string_test = "aaa (x, bbb(a, b))";
    Fmla fmla = parse_fmla(string_test);

    // string_test = "(a ∩ (c ∆ d)) ≍ (a - b)";
    // parse_fmla(string_test);
}