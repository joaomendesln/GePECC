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

    // // ++ Parser
    // test_parser();

    // ++ Deduction
    // test_get_initial_tableau();
    // test_get_all_arranges();
    // test_is_a_match();
    // test_matching_parameters();
    // test_trying_apply_expansion_rule();
    test_apply_cut();

    // ++ Syntax
    // test_get_term_of_fmla(); 
    // test_subst_parameter_by_term(); 

    return 0;
}