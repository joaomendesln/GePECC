#include "test_pre_process.h"
#include "test_parser.h"
#include "test_deduction.h"
#include "test_syntax.h"

using namespace std;

int main() {

    // // +++ Pre-process
    // test_pre_process_function_symbs();
    // test_pre_process_predicate_symbs();
    // test_pre_process_expansion_rules();
    // test_pre_process_signed_fmlas_input();
    // test_pre_process_cut_fmlas();
    // test_potential_premisse_nodes_rule();

    // // ++ Parser
    // test_parser();
    // test_fmla_equality_under_subst();

    // // ++ Deduction
    // test_get_initial_tableau();
    // test_get_all_arranges();
    // test_is_a_match();
    // test_matching_parameters();
    // test_apply_rule_with_premisse();
    // test_apply_cut();
    // test_get_cut_fmlas();
    // test_check_rule_application();
    // test_matching_premises();
    // test_get_pattern_matching_premisse_symb();
    test_get_ps_potential_symbols();

    // // ++ Syntax
    // test_get_term_of_fmla(); 
    // test_subst_parameter_by_term(); 

    return 0;
}