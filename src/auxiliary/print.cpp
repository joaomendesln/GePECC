#include "print.h"

using namespace std;

void pretty_printing_fmla_aux(Fmla fmla, FmlaNode node, int level) {
    if (node.children.size() == 0) {
        cout << node.data;
    }
    set<string> skolem_symbs = pre_process_skolem_symbs();

    if (skolem_symbs.find(node.data) == skolem_symbs.end() && (node.children.size() == 1 || node.children.size() == 2) ) {
        if (node.children.size() == 1) {
            cout << "(";
            pretty_printing_fmla_aux(fmla, fmla[node.children[0]], level + 1);
            cout << ")";
        }
        if (node.children.size() == 2) {
            if (level == 0) {
                pretty_printing_fmla_aux(fmla, fmla[node.children[0]], level + 1);
                cout << " " << node.data << " ";
                pretty_printing_fmla_aux(fmla, fmla[node.children[1]], level + 1);
            }
            else {
                cout << "(";
                pretty_printing_fmla_aux(fmla, fmla[node.children[0]], level + 1);
                cout << " " << node.data << " ";
                pretty_printing_fmla_aux(fmla, fmla[node.children[1]], level + 1);
                cout << ")";
            }
        }
    }
    else {
        if (node.children.size() > 0) {
            cout << "" << node.data << "";
            cout << "(";
            for (int i = 0; i < node.children.size(); i++) {
                if (i < node.children.size() - 1) {
                    pretty_printing_fmla_aux(fmla, fmla[node.children[i]], level + 1);
                    cout << ", ";
                }
                else {
                    pretty_printing_fmla_aux(fmla, fmla[node.children[i]], level + 1);
                }
            }
            cout << ")";
        }
    }
    
}

void pretty_printing_fmla(Fmla fmla) {
    pretty_printing_fmla_aux(fmla, fmla[0], 0);
}

void print_vec_int(vector<int> vec) {
    cout << "[";
    for (int i = 0; i < vec.size(); i++){
        if (i < vec.size() - 1) {
            cout << vec[i] << ", ";
        }
        else {
            cout << vec[i];
        }
    }
    cout << "]";
}

void print_fmla_prefix(Fmla fmla) {
    print_fmla_prefix_aux(fmla, 0);
}

void print_fmla_prefix_aux(Fmla fmla, int idx) {
    FmlaNode fmla_node = fmla[idx];

    cout << fmla_node.data;

    if (fmla_node.children.size() > 0) {
        cout << "(";
        for (int i = 0; i < fmla_node.children.size(); i++) {
            print_fmla_prefix_aux(fmla, fmla_node.children[i]);
            if (i < fmla_node.children.size() - 1) cout << ",";
        }
        cout << ")";
    }

}

void print_term_prefix(Term term) {
    print_term_prefix_aux(term, 0);
}

void print_term_prefix_aux(Term term, int idx) {
    TermNode term_node = term[idx];

    cout << term_node.data;

    if (term_node.children.size() > 0) {
        cout << "(";
        for (int i = 0; i < term_node.children.size(); i++) {
            print_term_prefix_aux(term, term_node.children[i]);
            if (i < term_node.children.size() - 1) cout << ",";
        }
        cout << ")";
    }

}

void print_tableau(Tableau tbl) {

    vector< vector<int>> branches = get_tbl_branches(tbl);

    // for (vector<int> branch : branches) {
    //     cout << "branch: ";
    //     print_vec_int(branch);
    //     cout << "\n";
    // }

    // BFS: getting the level of the nodes in tableau tree
    vector<int> tbl_level = get_tbl_levels(tbl);

    // DFS: pritting the tree
    stack<int> s;
    s.push(0);

    while (!s.empty()) {
        int current = s.top();

        for (int i = 0; i < tbl_level[current] + 1; i++) cout << "=";
        cout << " " << current << ": ";
        TblNode node = tbl[current];
        polarity sign = node.signed_fmla.sign;
        Fmla fmla = node.signed_fmla.fmla;
        if (sign == polarity::plus) cout << "+ ";
        if (sign == polarity::minus) cout << "- ";
        if (sign == polarity::plus || sign == polarity::minus) pretty_printing_fmla(fmla);
        else cout << fmla[0].data;
        if (node.justification_parents[0] != -1 && node.justification_parents[0] != -2) {
            cout << ", ";
            print_vec_int(node.justification_parents);
        }
        cout << "\n";

        s.pop();
        for (int i = tbl[current].tbl_children.size() - 1; i >= 0; i--) {
            int child = tbl[current].tbl_children[i]; 
            s.push(child);
        }
    }

    // for (int i = 0; i < branches.size(); i++){
    //     cout << "Branch " << i + 1 << "\n";
    //     vector<int> branch = branches[i];
    //     sort(branch.begin(), branch.end());
    //     for (int j = 0; j < branch.size(); j++) {
    //         TblNode node = tbl[branch[j]];
    //         cout << i << ": ";
    //         polarity sign = node.signed_fmla.sign;
    //         Fmla fmla = node.signed_fmla.fmla;
    //         if (sign == polarity::plus) cout << "+ ";
    //         if (sign == polarity::minus) cout << "- ";
    //         // if (node.sign == polarity::plus || node.sign == polarity::minus) pretty_printing_fmla(node.fmla);
    //         if (sign == polarity::plus || sign == polarity::minus) print_fmla_prefix(fmla);
    //         else cout << fmla[0].data;
    //         cout << ", ";
    //         print_vec_int(node.justification_parents);
    //         cout << "\n";

    //     }
    // }
}

void print_tableau_as_list(Tableau tbl) {
    for (int i = 0; i < tbl.size(); i++){
        TblNode node = tbl[i];
        cout << i << ": ";
        SignedFmla sf = node.signed_fmla;
        if (sf.sign == polarity::plus) cout << "+ ";
        if (sf.sign == polarity::minus) cout << "- ";
        if (sf.sign == polarity::plus || sf.sign == polarity::minus) pretty_printing_fmla(sf.fmla);
        else cout << sf.fmla[0].data;
        cout << ", ";
        print_vec_int(node.justification_parents);
        cout << ", " << node.tbl_parent << ", ";
        print_vec_int(node.tbl_children);
        cout << "\n";
    }
}

void print_tableau_as_list_fmla_prefix(Tableau tbl) {
    for (int i = 0; i < tbl.size(); i++){
        TblNode node = tbl[i];
        cout << i << ": ";
        SignedFmla sf = node.signed_fmla;
        if (sf.sign == polarity::plus) cout << "+ ";
        if (sf.sign == polarity::minus) cout << "- ";
        if (sf.sign == polarity::plus || sf.sign == polarity::minus) print_fmla_prefix(sf.fmla);
        else cout << sf.fmla[0].data;
        cout << ", ";
        print_vec_int(node.justification_parents);
        cout << ", " << node.tbl_parent << ", ";
        print_vec_int(node.tbl_children);
        cout << "\n";
    }
}

// void print_tableau_with_closure(Tableau tbl) {
//     vector< vector<int>> branches = get_tbl_branches(tbl);

//     // get closure nodes
//     map<int, pair<int, int>> closure_map;
//     set<int> max_closure_nodes;

//     for (vector<int> branch : branches) {
//         vector<int> closure_nodes = branch_closure_nodes(branch, tbl);

//         int max_closure_node = -1;

//         if (closure_nodes.size() == 2) max_closure_node = max(closure_nodes[0], closure_nodes[1]);

//         max_closure_nodes.insert(max_closure_node);
//         closure_map[max_closure_node] = make_pair(closure_nodes[0], closure_nodes[1]);
//     }

//     vector<int> tbl_level = get_tbl_levels(tbl);

//     // DFS: pritting the tree
//     stack<int> s;
//     s.push(0);

//     while (!s.empty()) {
//         int current = s.top();

//         for (int i = 0; i < tbl_level[current] + 1; i++) cout << "=";
//         cout << " " << current << ": ";
//         TblNode node = tbl[current];
//         if (node.sign == polarity::plus) cout << "+ ";
//         if (node.sign == polarity::minus) cout << "- ";
//         if (node.sign == polarity::plus || node.sign == polarity::minus) pretty_printing_fmla(node.fmla);
//         else cout << node.fmla[0].data;
//         cout << ", ";
//         print_vec_int(node.proof_parents);
//         cout << "\n";

//         if (max_closure_nodes.find(current) != max_closure_nodes.end()) {
//             int closure_node_1 = min(closure_map[current].first, closure_map[current].second);
//             for (int i = 0; i < tbl_level[current] + 2; i++) cout << "=";
//             cout << " * : ";
//             cout << "[" << closure_node_1 << ", " << current << "]\n";
//         }

//         s.pop();
//         for (int child : tbl[current].tbl_children) {
//             s.push(child);
//         }
//     }

// }