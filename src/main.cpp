#include <set>
#include "definitions/definitions.h"
#include "auxiliary/print.h"
#include "auxiliary/pre_process.h"
#include "auxiliary/deduction.h"

using namespace std;

int main(int argc, char* argv[]) {
    vector<SignedFmla> sf = pre_process_signed_fmla_input();
    vector<TblRule> er = pre_process_expansion_rules_input();

    cout << "ER size before: " << er.size() << "\n";
    er = add_cut_rule(er);
    cout << "ER size after cut: " << er.size() << "\n";

    er = remove_unnecessary_rules(sf, er);

    vector<Tableau> minimal_proofs = extract_minimal_proofs(sf, er);

    cout << "\nResulting minimal proofs: \n";
    cout << "Size: " << get_size(minimal_proofs[0]) << "\n";
    for (Tableau m : minimal_proofs) {
        print_tableau_as_list_fmla_prefix(m);
        cout << "\n";
    }
    return 0;
}