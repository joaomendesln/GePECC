#include <set>
#include "definitions/definitions.h"
#include "auxiliary/print.h"
#include "auxiliary/pre_process.h"
#include "auxiliary/deduction.h"

using namespace std;

int main(int argc, char* argv[]) {
    cout << "===== Pre-processing signed formulas file\n";
    vector<SignedFmla> sf = pre_process_signed_fmla_input();

    cout << "===== Pre-processing expansion rules file\n";
    vector<TblRule> er = pre_process_expansion_rules_input();

    // cout << "ER size before: " << er.size() << "\n";
    // er = add_cut_rule(er);
    // cout << "ER size after cut: " << er.size() << "\n";

    // er = remove_unnecessary_rules(sf, er);

    vector<Tableau> minimal_proofs = extract_minimal_proofs(sf, er);

    cout << "\nResulting minimal proofs: \n";
    cout << "Size: " << get_size(minimal_proofs[0]) << "\n";
    int i = 1;
    for (Tableau m : minimal_proofs) {

        // print_tableau_as_list_fmla_prefix(m);
        print_tableau(m);
        cout << "\n";

        // vector<vector<SignedFmla>> isomorphic_sets = proof_isomorphic_sf_sets(m, er);
        // cout << "\nAmount of proof-isomorphic sets:\n";
        // cout << isomorphic_sets.size() << "\n";
        // cout << ">>>>>>>>>>>>>>>>>>>>>>\n\n";
        // for (vector<SignedFmla> set_sf : isomorphic_sets) {
        //     cout << "Set " << i << "\n";
        //     i += 1;
        //     for (SignedFmla sf : set_sf) {
        //         if (sf.sign == polarity::plus) cout << "+ ";
        //         if (sf.sign == polarity::minus) cout << "- ";
        //         print_fmla_prefix(sf.fmla);
        //         cout << "\n";
        //     }
        //     cout << "\n";
        // }
    }

    // cout << "======\nPART 2\n\n";
    // for (Tableau m : minimal_proofs) {

    //     // print_tableau_as_list_fmla_prefix(m);
    //     print_tableau(m);
    //     cout << "\n";

    //     vector<vector<SignedFmla>> isomorphic_sets = proof_isomorphic_sf_sets(m, er);
    //     cout << "\nAmount of proof-isomorphic sets:\n";
    //     cout << isomorphic_sets.size() << "\n";
    //     cout << ">>>>>>>>>>>>>>>>>>>>>>\n\n";
    //     for (vector<SignedFmla> set_sf : isomorphic_sets) {
    //         cout << "Set " << i << "\n";
    //         i += 1;
    //         for (SignedFmla sf : set_sf) {
    //             if (sf.sign == polarity::plus) cout << "+ ";
    //             if (sf.sign == polarity::minus) cout << "- ";
    //             print_fmla_prefix(sf.fmla);
    //             cout << "\n";
    //         }
    //         cout << "\n";
    //     }
    // }
    return 0;
}