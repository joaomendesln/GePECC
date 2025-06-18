#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <vector>
#include <map>
#include <string>

using namespace std;


enum class polarity { // TODO: check if "class" is necessary
    plus, minus, none
};


enum class symbType {
    func, pred
};


struct FmlaNode {
    string data;
    int parent;
    vector<int> children;

    FmlaNode() {}
    FmlaNode(string data, int parent, vector<int> children) : data(data), parent(parent), children(children) {}
};


typedef vector<FmlaNode> Fmla;


struct TermNode {
    string data;
    int parent;
    vector<int> children;

    TermNode() {}
    TermNode(string data, int parent, vector<int> children) : data(data), parent(parent), children(children) {}
};


typedef vector<TermNode> Term;
typedef map<string, Term> Subst;


struct SignedFmla {
    polarity sign;
    Fmla fmla;

    SignedFmla() {}
    SignedFmla(polarity sign, Fmla fmla) : sign(sign), fmla(fmla) {}
};


struct TblNode {
    SignedFmla signed_fmla;
    vector<int> justification_parents; // -1 for initial tableau, -2 for cut formulas
    int tbl_parent;
    vector<int> tbl_children;
    int expansion_rule_idx;
    int conclusion_idx;

    TblNode() {}
    TblNode(SignedFmla signed_fmla, vector<int> justification_parents, int tbl_parent, vector<int> tbl_children, int expansion_rule_idx, int conclusion_idx) : signed_fmla(signed_fmla), justification_parents(justification_parents), tbl_parent(tbl_parent), tbl_children(tbl_children), expansion_rule_idx(expansion_rule_idx), conclusion_idx(conclusion_idx) {}
};


typedef vector<TblNode> Tableau;


struct TblRule {
    vector<SignedFmla> premises;
    vector<SignedFmla> conclusions;

    TblRule() {}
    TblRule(vector<SignedFmla> premises, vector<SignedFmla> conclusions) : premises(premises), conclusions(conclusions) {}
};


#endif // DEFINITIONS_H

