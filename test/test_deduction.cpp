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

    SignedFmla sf_premise;
    sf_premise.sign = polarity::plus;
    sf_premise.fmla = parse_fmla("∈(p1,p2)");

    cout << is_a_match(sf_tbl, sf_premise) << "\n";
}

void test_matching_parameters() {
    SignedFmla sf_tbl;
    sf_tbl.sign = polarity::plus;
    sf_tbl.fmla = parse_fmla("∈(p1,∩(p2,p3))");

    SignedFmla sf_premise;
    sf_premise.sign = polarity::plus;
    sf_premise.fmla = parse_fmla("∈(p1,p2)");

    Subst matching_parameters_map = matching_parameters(sf_tbl, sf_premise);

    for (const auto& pair : matching_parameters_map) {
        cout << pair.first << ": ";
        print_term_prefix(pair.second);
        cout << "\n";
    }
}

void test_apply_rule_with_premise(){
    vector<SignedFmla> sf = pre_process_signed_fmla_input();
    Tableau tbl = get_initial_tableau(sf);

    vector<TblRule> expansion_rules = pre_process_expansion_rules_input();
    
    vector<Tableau> tableaux = apply_rule_with_premise(tbl, expansion_rules[9], 9);

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

    vector<Tableau> tableaux = apply_cut(tbl, expansion_rules);

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

void test_potential_premise_nodes_rule() {

    vector<TblRule> expansion_rules = pre_process_expansion_rules_input();

    SignedFmla sf = pre_process_signed_fmla("(+, ∈(p1,∩(p2,p3)))");
    TblRule rule = expansion_rules[2];
    vector<SignedFmla> potential_nodes = potential_premise_nodes_rule(sf, rule);

    for (SignedFmla node : potential_nodes) {
        if (node.sign == polarity::minus) cout << "- ";
        if (node.sign == polarity::plus) cout << "+ ";

        print_fmla_prefix(node.fmla);
        cout << "\n";
    }
}

void test_check_rule_application() {

    vector<TblRule> expansion_rules = pre_process_expansion_rules_input();

    SignedFmla sf1 = pre_process_signed_fmla("(+, ∈(p1,∩(∩(p2,p4),p3)))");
    SignedFmla sf2 = pre_process_signed_fmla("(+, ∈(p1,∩(p2,p4)))");

    vector<SignedFmla> justifictions = {sf1};
    // justifictions.push_back(sf1);

    cout << "aaa\n";
    
    cout << check_rule_application(justifictions, sf2, expansion_rules) << "\n";


}

void test_matching_premises() {

    cout << "Test 1\n";

    vector<TblRule> expansion_rules = pre_process_expansion_rules_input();
    TblRule rule = expansion_rules[0];
    int prem_idx = 0;
    int conc_idx = 1;
    vector<SignedFmla> result = pattern_matching_premises(expansion_rules, rule, prem_idx, conc_idx);
    
    for (SignedFmla sf : result) {
        if (sf.sign == polarity::plus) cout << "+ ";
        if (sf.sign == polarity::minus) cout << "- ";
        pretty_printing_fmla(sf.fmla);
        cout << "\n";
    }

    cout << "\nTest 2\n";

    rule = expansion_rules[1];
    prem_idx = 0;
    conc_idx = 0;

    result = pattern_matching_premises(expansion_rules, rule, prem_idx, conc_idx);
    
    for (SignedFmla sf : result) {
        if (sf.sign == polarity::plus) cout << "+ ";
        if (sf.sign == polarity::minus) cout << "- ";
        pretty_printing_fmla(sf.fmla);
        cout << "\n";
    }

    cout << "\nTest 3\n";

    rule = expansion_rules[1];
    prem_idx = 1;
    conc_idx = 0;

    result = pattern_matching_premises(expansion_rules, rule, prem_idx, conc_idx);
    
    for (SignedFmla sf : result) {
        if (sf.sign == polarity::plus) cout << "+ ";
        if (sf.sign == polarity::minus) cout << "- ";
        pretty_printing_fmla(sf.fmla);
        cout << "\n";
    }

    cout << "\nTest 4\n";

    rule = expansion_rules[17];
    prem_idx = 0;
    conc_idx = 0;

    result = pattern_matching_premises(expansion_rules, rule, prem_idx, conc_idx);
    
    for (SignedFmla sf : result) {
        if (sf.sign == polarity::plus) cout << "+ ";
        if (sf.sign == polarity::minus) cout << "- ";
        pretty_printing_fmla(sf.fmla);
        cout << "\n";
    }

    cout << "\nTest 5\n";

    rule = expansion_rules[22];
    prem_idx = 0;
    conc_idx = 0;

    result = pattern_matching_premises(expansion_rules, rule, prem_idx, conc_idx);
    
    for (SignedFmla sf : result) {
        if (sf.sign == polarity::plus) cout << "+ ";
        if (sf.sign == polarity::minus) cout << "- ";
        pretty_printing_fmla(sf.fmla);
        cout << "\n";
    }

    cout << "\nTest 6\n";

    rule = expansion_rules[21];
    prem_idx = 0;
    conc_idx = 0;

    result = pattern_matching_premises(expansion_rules, rule, prem_idx, conc_idx);
    
    for (SignedFmla sf : result) {
        if (sf.sign == polarity::plus) cout << "+ ";
        if (sf.sign == polarity::minus) cout << "- ";
        pretty_printing_fmla(sf.fmla);
        cout << "\n";
    }
}

void test_get_pattern_matching_premise_symb() {
    Fmla fmla1 = parse_fmla("∈(p1,∩(p2,∪(p3,p4)))");
    Fmla fmla2 = parse_fmla("∈(p1,-(p2,p3))");
    cout << get_pattern_matching_premise_symb(fmla1, 4, fmla2) << "\n";
}

void test_get_ps_potential_symbols() {
    cout << "===== Pre-processing signed formulas file\n";
    vector<SignedFmla> sf = pre_process_signed_fmla_input();

    cout << "===== Pre-processing expansion rules file\n";
    vector<TblRule> er = pre_process_expansion_rules_input();

    vector<Tableau> minimal_proofs = extract_minimal_proofs(sf, er);

    cout << "\nResulting minimal proofs: \n";
    int i = 1;
    for (Tableau m : minimal_proofs) {
        // print_tableau_as_list_fmla_prefix(m);
        print_tableau(m);
        get_ps_potential_symbols(m, er);
        cout << "\n";

    }
}