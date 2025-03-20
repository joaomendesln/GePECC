#include <set>
#include "definitions/definitions.h"
#include "auxiliary/print.h"
#include "auxiliary/pre_process.h"
#include "auxiliary/deduction.h"

using namespace std;

int main(int argc, char* argv[]) {
    cout << "===== Pre-processing signed formulas file\n";
    vector<SignedFmla> input_sf = pre_process_signed_fmla_input();
    for (SignedFmla signed_fmla : input_sf) {
        if (signed_fmla.sign == polarity::plus) cout << "+ ";
        if (signed_fmla.sign == polarity::minus) cout << "- ";
        pretty_printing_fmla(signed_fmla.fmla);
        cout << "\n";
    }

    cout << "\n===== Pre-processing expansion rules file\n";
    vector<TblRule> er = pre_process_expansion_rules_input();

    vector<Tableau> minimal_proofs = extract_minimal_proofs(input_sf, er);

    if (minimal_proofs.size() > 0) {
        cout << "\n===== Resulting minimal proofs: \n";
        cout << "Amount of proofs: " << minimal_proofs.size() << "\n";
        cout << "Size of proofs: " << get_size(minimal_proofs[0], er) << "\n";
        int i = 1;
        vector<vector<SignedFmla> > isomorphic_sets;
        for (Tableau m : minimal_proofs) {

            // print_tableau_as_list_fmla_prefix(m);
            cout << "\n>> Proof " << i << "\n";
            print_tableau(m);

            i += 1;

            // vector<vector<SignedFmla>> isomorphic_sets_to_m = proof_isomorphic_sf_sets(m, er);
            // cout << "\nAmount of proof-isomorphic sets:\n";
            // cout << isomorphic_sets_to_m.size() << "\n";
            // cout << ">>>>>>>>>>>>>>>>>>>>>>\n\n";
            // for (vector<SignedFmla> set_sf : isomorphic_sets_to_m) {
            //     if (!vec_sf_in_vec_vec_sf(isomorphic_sets, set_sf)) {
            //         isomorphic_sets.push_back(set_sf);
            //     }
            //     // cout << "Set " << i << "\n";
            //     // i += 1;
            //     // for (SignedFmla sf : set_sf) {
            //     //     if (sf.sign == polarity::plus) cout << "+ ";
            //     //     if (sf.sign == polarity::minus) cout << "- ";
            //     //     print_fmla_prefix(sf.fmla);
            //     //     cout << "\n";
            //     // }
            //     // cout << "\n";
            // }
        }

        // cout << "======\nPART 2\n\n";
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
    else {
        cout << "No proof has been found\n";
    }
    
    int i = 1;
    cout << "\n===== Proof-isomorphic sets of signed formulas\n";
    vector<vector<SignedFmla> > resulting_isomorphic_sets;
    for (Tableau m : minimal_proofs) {

        // print_tableau_as_list_fmla_prefix(m);
        // print_tableau(m);
        // cout << "\n";

        vector<vector<SignedFmla> > isomorphic_sets = proof_isomorphic_sf_sets(m, er);
        // cout << "\nAmount of proof-isomorphic sets:\n";
        // cout << isomorphic_sets.size() << "\n";
        // cout << ">>>>>>>>>>>>>>>>>>>>>>\n\n";
        for (vector<SignedFmla> set_sf : isomorphic_sets) {
            if (!vec_sf_in_vec_vec_sf(resulting_isomorphic_sets, set_sf)) {
                resulting_isomorphic_sets.push_back(set_sf);
            }
            // cout << "Set " << i << "\n";
            // i += 1;
            // for (SignedFmla sf : set_sf) {
            //     if (sf.sign == polarity::plus) cout << "+ ";
            //     if (sf.sign == polarity::minus) cout << "- ";
            //     print_fmla_prefix(sf.fmla);
            //     cout << "\n";
            // }
            // cout << "\n";
        }
    }

    for (vector<SignedFmla> set_sf : resulting_isomorphic_sets) {
        cout << ">> Set " << i << "\n";
        i += 1;
        for (SignedFmla sf : set_sf) {
            if (sf.sign == polarity::plus) cout << "+ ";
            if (sf.sign == polarity::minus) cout << "- ";
            pretty_printing_fmla(sf.fmla);
            cout << "\n";
        }
        cout << "\n";
    }
    return 0;
}