#include "test_deduction.h"
#include <iostream>

using namespace std;

void test_get_initial_tableau() {
    vector<SignedFmla> sf = pre_process_signed_fmla_input();
    Tableau tbl = get_initial_tableau(sf);

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
        print_term_prefix(pair.second);
        cout << "\n";
    }
}

void test_apply_rule_with_premisse(){
    vector<SignedFmla> sf = pre_process_signed_fmla_input();
    Tableau tbl = get_initial_tableau(sf);

    vector<TblRule> expansion_rules = pre_process_expansion_rules_input();
    
    vector<Tableau> tableaux = apply_rule_with_premisse(tbl, expansion_rules[9]);

    int i = 0;
    for (Tableau tableau : tableaux) {
        cout << "Tableau " << i + 1 << ":\n";
        i++;
        print_tableau_as_list_fmla_prefix(tableau);
        cout << "\n";
    }
}

void test_apply_cut() {
    vector<SignedFmla> sf = pre_process_signed_fmla_input();
    Tableau tbl = get_initial_tableau(sf);

    vector<TblRule> expansion_rules = pre_process_expansion_rules_input();
    Fmla cut_fmla = parse_fmla("∈(g(p1,p2),p1)");

    vector<Tableau> tableaux = apply_cut(tbl, cut_fmla, expansion_rules);

    int i = 0;
    for (Tableau tableau : tableaux) {
        cout << "Tableau " << i + 1 << ":\n";
        i++;
        print_tableau_as_list_fmla_prefix(tableau);
        cout << "\n";
    }
}

void test_get_cut_fmlas() {
    vector<TblRule> expansion_rules = pre_process_expansion_rules_input();
    vector<Fmla> cut_fmlas = get_cut_fmlas(expansion_rules);

    for (Fmla fmla : cut_fmlas) {
        print_fmla_prefix(fmla);
        cout << "\n";
    }
}

void test_potential_premisse_nodes_rule() {

    vector<TblRule> expansion_rules = pre_process_expansion_rules_input();

    SignedFmla sf = pre_process_signed_fmla("(+, ∈(p1,∩(p2,p3)))");
    TblRule rule = expansion_rules[2];
    vector<SignedFmla> potential_nodes = potential_premisse_nodes_rule(sf, rule);

    for (SignedFmla node : potential_nodes) {
        if (node.sign == polarity::minus) cout << "- ";
        if (node.sign == polarity::plus) cout << "+ ";

        print_fmla_prefix(node.fmla);
        cout << "\n";
    }
}