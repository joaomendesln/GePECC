#include <set>
#include "definitions/definitions.h"
#include "auxiliary/print.h"
#include "auxiliary/pre_process.h"
#include "auxiliary/deduction.h"

using namespace std;

int main(int argc, char* argv[]) {
    vector<SignedFmla> input_sf;
    vector<TblRule> er;
    if (argc == 3) {
        cout << "===== Pre-processing symbols of the language\n";

        map<string, int> function_symbols = pre_process_function_symbs();
        map<int, vector<string>> function_by_arity;

        for (const auto& [symbol, arity] : function_symbols) {
            function_by_arity[arity].push_back(symbol);
        }

        cout << ">> Function symbols\n";
        for (const auto& [arity, symbols] : function_by_arity) {
            if (arity == 0) cout << "Constants: ";
            if (arity > 0) cout << "Arity " << arity << ": ";
            for (int j = 0; j < symbols.size(); j++){
                if (j < symbols.size() - 1) {
                    cout << symbols[j] << ", ";
                }
                else {
                    cout << symbols[j] << "";
                }
            }
            cout << "\n";
        }
        set<string> skolem_set = pre_process_skolem_symbs();
        vector<string> skolem_vec(skolem_set.begin(), skolem_set.end());

        cout << "Skolemization: ";
        for (int i = 0; i < skolem_vec.size(); i++) {
            if (i < skolem_vec.size() - 1) cout << skolem_vec[i] << ", ";
            else  cout << skolem_vec[i] << "\n\n";
        }

        map<string, int> predicate_symbols = pre_process_predicate_symbs();
        map<int, vector<string>> predicate_by_arity;

        for (const auto& [symbol, arity] : predicate_symbols) {
            predicate_by_arity[arity].push_back(symbol);
        }

        cout << ">> Predicate symbols\n";
        for (const auto& [arity, symbols] : predicate_by_arity) {
            cout << "Arity " << arity << ": ";
            for (int j = 0; j < symbols.size(); j++){
                if (j < symbols.size() - 1) {
                    cout << symbols[j] << ", ";
                }
                else {
                    cout << symbols[j] << "";
                }
            }
            cout << "\n";
        }

        cout << "\n===== Pre-processing expansion rules file\n";
        er = pre_process_expansion_rules_input(argv[1]);

        cout << "===== Pre-processing signed formulas file\n";
        input_sf = converts_str_to_signed_fmla_input(argv[2]);
        for (SignedFmla signed_fmla : input_sf) {
            if (signed_fmla.sign == polarity::plus) cout << "+ ";
            if (signed_fmla.sign == polarity::minus) cout << "- ";
            pretty_printing_fmla(signed_fmla.fmla);
            cout << "\n";
        }
    }
    else {
        cerr << "===== Two files paths must be passed as input\n";
        return 0;
    }

    if (input_sf.size() > 0 && er.size() > 0) {
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
    }
}