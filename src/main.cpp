#include <set>
#include "definitions/definitions.h"
#include "auxiliary/print.h"
#include "auxiliary/pre_process.h"
#include "auxiliary/deduction.h"

using namespace std;

int main(int argc, char* argv[]) {
    vector<SignedFmla> input_sf;
    vector<TblRule> er;
    if (argc > 1) {
        cout << "===== Pre-processing signed formulas file\n";
        input_sf = converts_str_to_signed_fmla_input(argv[2]);
        for (SignedFmla signed_fmla : input_sf) {
            if (signed_fmla.sign == polarity::plus) cout << "+ ";
            if (signed_fmla.sign == polarity::minus) cout << "- ";
            pretty_printing_fmla(signed_fmla.fmla);
            cout << "\n";
        }

        cout << "\n===== Pre-processing expansion rules file\n";
        er = pre_process_expansion_rules_input(argv[1]);
    }
    else {
        cout << "===== Pre-processing signed formulas file\n";
        input_sf = converts_str_to_signed_fmla_input("");
        for (SignedFmla signed_fmla : input_sf) {
            if (signed_fmla.sign == polarity::plus) cout << "+ ";
            if (signed_fmla.sign == polarity::minus) cout << "- ";
            pretty_printing_fmla(signed_fmla.fmla);
            cout << "\n";
        }

        cout << "\n===== Pre-processing expansion rules file\n";
        er = pre_process_expansion_rules_input("");
    }

    cout << "\n===== Searching for minimal proofs\n";
    vector<Tableau> minimal_proofs = search_minimal_proofs(input_sf, er);

    if (minimal_proofs.size() > 0) {
        cout << "Amount of proofs: " << minimal_proofs.size() << "\n";
        cout << "Size of proofs: " << get_size(minimal_proofs[0], er) << "\n";
        int i = 1;
        vector<vector<SignedFmla> > isomorphic_sets;
        for (Tableau m : minimal_proofs) {
            cout << "\n>> Proof " << i << "\n";
            print_proof(m, er);

            i += 1;
        }
    }
    else {
        cout << "No proof has been found\n";
    }
    
    int i = 1;

    cout << "\n===== Searching for proof-isomorphic sets of signed formulas\n";
    vector<vector<SignedFmla> > resulting_isomorphic_sets;
    for (Tableau m : minimal_proofs) {
        vector<vector<SignedFmla> > isomorphic_sets = search_proof_isomorphic_sf_sets(m, er);
        for (vector<SignedFmla> set_sf : isomorphic_sets) {
            if (!vec_sf_in_vec_vec_sf(resulting_isomorphic_sets, set_sf)) {
                resulting_isomorphic_sets.push_back(set_sf);
            }
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