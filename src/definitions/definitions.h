#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <vector>
#include <map>
#include <string>


using namespace std;

enum class polarity { // TODO: check if "class" is necessary
    plus, minus, none
};

enum class side { // TODO: delete this class if it is not necessary
    left, right
};

struct FmlaNode {
    string data;
    int parent;
    vector<int> children;

    // TODO: operator for equality of formulas

    // void print() const { // TODO: check if const is necessary
    //     cout << "(" << data << ", " << parent << ", ";
    //     print_vec_int(children);
    //     cout << ")";
    // }
};

typedef vector<FmlaNode> Fmla;

struct TermNode {
    string data;
    int parent;
    vector<int> children;
};

typedef vector<TermNode> Term;
typedef map<string, Term> Subst;

struct SignedFmla {
    polarity sign;
    Fmla fmla;
};

struct TblNode {
    SignedFmla signed_fmla;
    vector<int> justification_parents; // -1 for initial tableau, -2 for 0-premisse rules, -3 for cut formulas
    int tbl_parent;
    vector<int> tbl_children;

    bool operator<(const TblNode& other) const {
        if (justification_parents.size() < other.justification_parents.size()) { // this is not correct actually, though it won't affect the procedure
            return true;
        }
        return false;
    }

    // void print() const { // check if const is necessary
    //     char s;
    //     if (sign == polarity::plus) s = '+';
    //     if (sign == polarity::minus) s = '-';
    //     if (sign == polarity::none) s = '_';

    //     cout << "(" << s << ", ";
    //     pretty_printing_fmla(fmla);
    //     cout << ", ";
    //     print_vec_int(proof_parents);
    //     cout << ", ";
    //     print_vec_int(hypotheses);
    //     cout << "[" << exaustion[0] << ", " << exaustion[1] << "])";
    // }

    // void print_tbl() const {
    //     char s;
    //     if (sign == polarity::plus) s = '+';
    //     if (sign == polarity::minus) s = '-';

    //     cout << s << " ";
    //     pretty_printing_fmla(fmla);
    //     cout << ", ";
    //     print_vec_int(proof_parents);
    // }
};

typedef vector<TblNode> Tableau;

struct TblRule {
    vector<SignedFmla> premisses;
    SignedFmla conclusion;
};

#endif // DEFINITIONS_H