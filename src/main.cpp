#include <set>
#include "definitions/definitions.h"
#include "auxiliary/print.h"
#include "auxiliary/pre_process.h"
#include "auxiliary/deduction.h"

using namespace std;

int main(int argc, char* argv[]) {
    vector<SignedFmla> sf = pre_process_signed_fmla_input();
    vector<TblRule> er = pre_process_expansion_rules_input();

    vector<Tableau> minimal_proofs = extract_minimal_proofs(sf, er);

    for (Tableau m : minimal_proofs) {
        print_tableau_as_list_fmla_prefix(m);
    }
    return 0;
}