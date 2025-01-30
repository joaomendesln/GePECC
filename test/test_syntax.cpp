#include "test_syntax.h"
#include <iostream>

using namespace std;

// void test_fmla_equality() {

//     vector<FmlaNode> fmla1 = {
//         { "⊆", 0, {1, 2}},                    
//         { "∩", 0, {3, 4}},
//         { "∪", 0, {5, 6}},
//         { "c", 1, {}},
//         { "d", 1, {}},
//         { "a", 2, {}},
//         { "b", 2, {}} 
//     };

//     vector<FmlaNode> fmla2 = {
//         { "⊆", 0, {1, 2}},   
//         { "∩", 0, {5, 6}},                 
//         { "∪", 0, {3, 4}},
//         { "a", 2, {}},
//         { "b", 2, {}},
//         { "c", 1, {}},
//         { "d", 1, {}} 
//     };

//     vector<FmlaNode> fmla3 = {
//         { "⊆", 0, {1, 2}},   
//         { "∪", 0, {5, 6}},                 
//         { "∩", 0, {3, 4}},
//         { "c", 1, {}},
//         { "d", 1, {}},
//         { "a", 2, {}},
//         { "b", 2, {}}
//     };

//     // test1
//     pretty_printing_fmla(fmla1);
//     cout << " and ";
//     pretty_printing_fmla(fmla2);
//     cout << " are equal? ";
//     if (fmla_equality(fmla1, fmla2) == 1) cout << "yes\n"; else cout << "no\n";

//     // test1
//     pretty_printing_fmla(fmla1);
//     cout << " and ";
//     pretty_printing_fmla(fmla3);
//     cout << " are equal? ";
//     if (fmla_equality(fmla1, fmla3) == 1) cout << "yes\n"; else cout << "no\n";

//     // test1
//     pretty_printing_fmla(fmla2);
//     cout << " and ";
//     pretty_printing_fmla(fmla3);
//     cout << " are equal? ";
//     if (fmla_equality(fmla2, fmla3) == 1) cout << "yes\n"; else cout << "no\n";
    
// }

void test_get_term_of_fmla() {
    vector<FmlaNode> fmla = parse_fmla("∈(p1,∪(p2, ∩(p3, p4)))");

    vector<TermNode> term1 = get_term_of_fmla(fmla, 2);
    vector<TermNode> term2 = get_term_of_fmla(fmla, 4);
}

void test_subst_parameter_by_term() {

    vector<FmlaNode> fmla = parse_fmla("∈(p1,∪(p2, ∩(p3, p4)))");
    vector<TermNode> term = get_term_of_fmla(fmla, 2);

    vector<FmlaNode> resulting_fmla = subst_parameter_by_term(fmla, 1, term);
    print_fmla_infix(resulting_fmla);

    cout << "\n";

    fmla = parse_fmla("∈(p1,∪(p2, ∩(p3, p4)))");
    term = get_term_of_fmla(fmla, 5);

    resulting_fmla = subst_parameter_by_term(fmla, 1, term);
    print_fmla_infix(resulting_fmla);

    cout << "\n";
}