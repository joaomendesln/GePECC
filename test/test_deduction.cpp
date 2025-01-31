#include "test_deduction.h"
#include <iostream>

using namespace std;

void test_get_initial_tableau() {
    Tableau tbl = get_initial_tableau();

    print_tableau_as_list(tbl);
}

void test_get_all_arranges() {

    vector<vector<int>> all_arranges = get_all_arranges(4, 3);

    for (int i = 0; i < all_arranges.size(); i++){
        print_vec_int(all_arranges[i]);
        cout << "\n";
    }

}

void test_is_a_match() {
    SignedFmla sf_tbl;
    sf_tbl.sign = polarity::plus;
    sf_tbl.fmla = parse_fmla("∈(p1,∩(p2,p3))");

    SignedFmla sf_premisse;
    sf_premisse.sign = polarity::plus;
    sf_premisse.fmla = parse_fmla("∈(p1,p2)");

    cout << is_a_match(sf_tbl, sf_premisse) << "\n";
}

void test_matching_parameters() {
    SignedFmla sf_tbl;
    sf_tbl.sign = polarity::plus;
    sf_tbl.fmla = parse_fmla("∈(p1,∩(p2,p3))");

    SignedFmla sf_premisse;
    sf_premisse.sign = polarity::plus;
    sf_premisse.fmla = parse_fmla("∈(p1,p2)");

    Subst matching_parameters_map = matching_parameters(sf_tbl, sf_premisse);

    for (const auto& pair : matching_parameters_map) {
        cout << pair.first << ": ";
        print_term_infix(pair.second);
        cout << "\n";
    }
}

void test_trying_apply_expansion_rule(){
    Tableau tbl = get_initial_tableau();

    vector<TblRule> expansion_rules = pre_process_expansion_rules();
    
    vector<Tableau> tableaux = trying_apply_expansion_rule(tbl, expansion_rules[9]);

    int i = 0;
    for (Tableau tableau : tableaux) {
        cout << "Tableau " << i + 1 << ":\n";
        i++;
        print_tableau_as_list_fmla_infix(tableau);
        cout << "\n";
    }
}

void test_apply_cut() {
    Tableau tbl = get_initial_tableau();

    vector<TblRule> expansion_rules = pre_process_expansion_rules();
    Fmla cut_fmla = parse_fmla("∈(g(p1,p2),p1)");

    vector<Tableau> tableaux = apply_cut(tbl, cut_fmla);

    int i = 0;
    for (Tableau tableau : tableaux) {
        cout << "Tableau " << i + 1 << ":\n";
        i++;
        print_tableau_as_list_fmla_infix(tableau);
        cout << "\n";
    }
    

}