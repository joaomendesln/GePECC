#include "test_pre_process.h"
#include <iostream>

using namespace std;

void test_pre_process_function_symbs() {

    map <string, int> function_symbs = pre_process_function_symbs();

    for (const auto& pair : function_symbs) {
        cout << pair.first << ": " << pair.second << "\n";
    }

}

void test_pre_process_predicate_symbs() {

    map <string, int> predicate_symbs = pre_process_predicate_symbs();

    for (const auto& pair : predicate_symbs) {
        cout << pair.first << ": " << pair.second << "\n";
    }

}

void test_pre_process_expension_rules() {

    vector<TblRule> expansion_rules = pre_process_expansion_rules_input();

    for (int i = 0; i < expansion_rules.size(); i++) {
        cout << "++++ Rule "<< i << " ++++\n\n";

        TblRule single_expansion_rule = expansion_rules[i];

        vector<SignedFmla> premisses = single_expansion_rule.premisses;
        vector<SignedFmla> conclusions = single_expansion_rule.conclusions;

        cout << ">>> Premisses\n";
        for (int j = 0; j < premisses.size(); j++) {
            SignedFmla premisse = premisses[j];
            string s = "";
            if (premisse.sign == polarity::minus) s = "- ";
            if (premisse.sign == polarity::plus) s = "+ ";
            cout << s;
            print_fmla_prefix(premisse.fmla);
            cout << "\n";

        }
        cout << ">>> Conclusion\n";
        for (int j = 0; j < conclusions.size(); j++) {
            SignedFmla conclusion = conclusions[j];
            string s = "";
            if (conclusion.sign == polarity::minus) s = "- ";
            if (conclusion.sign == polarity::plus) s = "+ ";
            cout << s;
            print_fmla_prefix(conclusion.fmla);
            cout << "\n";

        }
    }

}

void test_pre_process_signed_fmlas_input() {
    vector<SignedFmla> sf_input = pre_process_signed_fmla_input();

    for (SignedFmla sf : sf_input) {
        string s = "";
        if (sf.sign == polarity::minus) s = "- ";
        if (sf.sign == polarity::plus) s = "+ ";
        cout << s;
        print_fmla_prefix(sf.fmla);
        cout << "\n";
    }
}