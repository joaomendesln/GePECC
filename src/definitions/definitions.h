/**
 * @file definitions.h
 * @author João Mendes
 * @brief Definitions of enums and structs that are used in the project
 * @version 0.0.1
 * @date 2025-06-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <vector>
#include <map>
#include <string>

using namespace std;

/**
 * @brief Polarity of a signed formula
 * 
 */
enum class polarity {
    plus, minus, none
};

/**
 * @brief Node of the abstract representation of a formula
 * 
 */
struct FmlaNode {
    /**
     * @brief Content of the node
     * 
     */
    string data;

    /**
     * @brief Index of the parent. If it is the root node, the parent is 0
     * 
     */
    int parent;

    /**
     * @brief Indexes of its children
     * 
     */
    vector<int> children;

    /**
     * @brief Construct with no arguments
     * 
     */
    FmlaNode() {}

    /**
     * @brief Constructor with all arguments
     * 
     * @param data 
     * @param parent 
     * @param children 
     */
    FmlaNode(string data, int parent, vector<int> children) : data(data), parent(parent), children(children) {}
};

/**
 * @brief A formula is implemented as a vector of `FmlaNode` objects
 * 
 */
typedef vector<FmlaNode> Fmla;

/**
 * @brief Node of the abstract representation of a term
 * 
 */
struct TermNode {
    /**
     * @brief Content of the node
     * 
     */
    string data;

    /**
     * @brief Index of the parent. If it is the root node, the parent is 0
     * 
     */
    int parent;

    /**
     * @brief Indexes of its children
     * 
     */
    vector<int> children;

    /**
     * @brief Constructor with no arguments
     * 
     */
    TermNode() {}

    /**
     * @brief Constructor with all arguments
     *  
     */
    TermNode(string data, int parent, vector<int> children) : data(data), parent(parent), children(children) {}
};

/**
 * @brief A term is implemented as a vector of `TermNode` objects
 * 
 */
typedef vector<TermNode> Term;

/**
 * @brief A substitution is implemented as map from string objects, representing the variable, to `Term` objects
 * 
 */
typedef map<string, Term> Subst;

/**
 * @brief Signed formula
 * 
 */
struct SignedFmla {
    /**
     * @brief Polarity of the signed formula
     * 
     */
    polarity sign;

    /**
     * @brief Formula of the signed formula
     * 
     */
    Fmla fmla;

    /**
     * @brief Constructor with no arguments
     * 
     */
    SignedFmla() {}

    /**
     * @brief Constructor with all arguments
     * 
     */
    SignedFmla(polarity sign, Fmla fmla) : sign(sign), fmla(fmla) {}
};

/**
 * @brief Tableau expansion rule
 * 
 */
struct TblRule {
    /**
     * @brief Premises of the rule
     * 
     */
    vector<SignedFmla> premises;

    /**
     * @brief Conclusions of the rule
     * 
     */
    vector<SignedFmla> conclusions;

    /**
     * @brief Constructor with no arguments
     * 
     */
    TblRule() {}

    /**
     * @brief Constructor with all arguments
     * 
     * @param premises 
     * @param conclusions 
     */
    TblRule(vector<SignedFmla> premises, vector<SignedFmla> conclusions) : premises(premises), conclusions(conclusions) {}
};

/**
 * @brief Node of a tableau tree
 * 
 */
struct TblNode {
    /**
     * @brief Signed formula
     * 
     */
    SignedFmla signed_fmla;

    /**
     * @brief Indexes of the nodes that justify the addition of this node in the tableau
     * -1 for nodes in the initial tableau segment
     * -2 for nodes added by an application of cut 
     * 
     */
    vector<int> justification_parents;

    /**
     * @brief Index of the parent in the tableau tree
     * 
     */
    int tbl_parent;

    /**
     * @brief Indexes of its children in the tableau tree
     * 
     */
    vector<int> tbl_children;

    /**
     * @brief Index of the rule that justifies the addition of this node in the tableau
     * 
     */
    int expansion_rule_idx;

    /**
     * @brief Index of the conclusion of rule `expansion_rule_idx` that is syntactically isomorphic to `signed_fmla`
     * 
     */
    int conclusion_idx;

    /**
     * @brief Constructor with no arguments
     * 
     */
    TblNode() {}

    /**
     * @brief Constructor with all arguments
     * 
     * @param signed_fmla 
     * @param justification_parents 
     * @param tbl_parent 
     * @param tbl_children 
     * @param expansion_rule_idx 
     * @param conclusion_idx 
     */
    TblNode(SignedFmla signed_fmla, vector<int> justification_parents, int tbl_parent, vector<int> tbl_children, int expansion_rule_idx, int conclusion_idx) : signed_fmla(signed_fmla), justification_parents(justification_parents), tbl_parent(tbl_parent), tbl_children(tbl_children), expansion_rule_idx(expansion_rule_idx), conclusion_idx(conclusion_idx) {}
};

/**
 * @brief A tableau tree is implemented as a vector `TblNode` objects
 * 
 */
typedef vector<TblNode> Tableau;


#endif // DEFINITIONS_H

