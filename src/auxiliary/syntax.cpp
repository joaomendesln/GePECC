#include "syntax.h"

using namespace std;

bool is_function_symb(string symb, map<string, int> function_symbs) {
    return function_symbs.find(symb) != function_symbs.end();
}

bool is_function_symb(string symb) {
    map<string, int> function_symbs = pre_process_function_symbs();
    return function_symbs.find(symb) != function_symbs.end();
}

bool is_predicate_symb(string symb, map<string, int> predicate_symbs) {
    return predicate_symbs.find(symb) != predicate_symbs.end();
}

bool is_predicate_symb(string symb) {
    map<string, int> predicate_symbs = pre_process_predicate_symbs();
    return predicate_symbs.find(symb) != predicate_symbs.end();
}

bool is_language_symb(string symb, map<string, int> language_symbs) {
    return language_symbs.find(symb) != language_symbs.end();
}

bool is_not_skolem_symb(string symb) {
    map<string, int> no_skolem_symb = pre_process_no_skolem_symbs();

    return no_skolem_symb.find(symb) != no_skolem_symb.end();
}

bool is_a_parameter(FmlaNode fmla_node) {
    map<string, int> language_symbs = pre_process_language_symbs();

    return fmla_node.children.size() == 0 && !is_language_symb(fmla_node.data, language_symbs);
}

bool is_a_parameter(TermNode term_node) {
    map<string, int> language_symbs = pre_process_language_symbs();

    return term_node.children.size() == 0 && !is_language_symb(term_node.data, language_symbs);
}


bool parameter_in_fmla(Fmla fmla, string parameter) {
    for (FmlaNode fmla_node : fmla) {
        if (is_a_parameter(fmla_node) && fmla_node.data == parameter) return true;
    }
    return false;
}

Term get_term_of_fmla(Fmla fmla, int term_root) {
    set<int> subfmla_nodes_idx;
    Term term;

    // BFS in the term root
    queue<int> q;
    q.push(term_root);

    // Map to make the correct attribution for the new ids of the term
    map<int, int> new_idx;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        new_idx[current] = term.size(); 
        FmlaNode fmla_node = fmla[current];

        TermNode term_node;
        term_node.data = fmla_node.data;
        if (term.size() == 0) {
            term_node.parent = 0;
        }
        else {
            term_node.parent = new_idx[fmla_node.parent];
            term[term_node.parent].children.push_back(term.size());
        }
        term_node.children = {};

        term.push_back(term_node);

        for (int child : fmla[current].children) {
            q.push(child);
        }
    }

    return term;
}

vector<Term> get_all_terms_of_fmla(Fmla fmla) {
    map<string, int> predicate_symbs = pre_process_predicate_symbs();

    vector<Term> all_terms;

    for (int i = 0; i < fmla.size(); i++) {
        if (!is_predicate_symb(fmla[i].data, predicate_symbs)) {
            all_terms.push_back(get_term_of_fmla(fmla, i));
        }
    }

    return all_terms;
}

Fmla subst_parameter_by_term(Fmla fmla, int parameter_idx, Term term) {
    map<int, int> new_idx;

    new_idx[0] = parameter_idx;

    for (int i = 1; i < term.size(); i++) {
        new_idx[i] = fmla.size() + i - 1;
    }

    for (int i = 0; i < term.size(); i++) {
        FmlaNode fmla_node;
        fmla_node.data = term[i].data;
        fmla_node.children = {};
        if (i == 0) {
            fmla_node.parent = fmla[parameter_idx].parent;
            fmla[parameter_idx] = fmla_node;
        }
        else {
            fmla_node.parent = new_idx[term[i].parent];
            fmla[fmla_node.parent].children.push_back(fmla.size());
            fmla.push_back(fmla_node);
        }
    }
    return fmla;
}

set<int> get_parameters_idxs(Fmla fmla) {
    set<int> parameters_idx;

    for (int i = 0; i < fmla.size(); i++) {
        if (is_a_parameter(fmla[i])) parameters_idx.insert(i);
    }

    return parameters_idx;
}

set<string> get_parameters(Fmla fmla) {
    set<string> parameters;

    for (int i = 0; i < fmla.size(); i++) {
        if (is_a_parameter(fmla[i])) parameters.insert(fmla[i].data);
    }

    return parameters;

}

Fmla subst_extension(Fmla fmla, Subst subs) {
    int fmla_initial_size = fmla.size();

    set<int> parameters_idxs = get_parameters_idxs(fmla);

    for (int i = 0; i < fmla_initial_size; i++) {
        if (parameters_idxs.find(i) != parameters_idxs.end()) {
            fmla = subst_parameter_by_term(fmla, i, subs[fmla[i].data]);
        }
    }

   return fmla;
}

Fmla subst_extension_potential(Fmla fmla, Subst subs) {

    set<string> parameters_subs, parameters_subs_domain;

    for (const auto& pair : subs) {
        parameters_subs.insert(pair.first);
        parameters_subs_domain.insert(pair.first);

        vector<string> parameters = get_all_parameters_of_term(pair.second);

        for (string parameter : parameters) {
            parameters_subs.insert(parameter);
        }

    }

    int fmla_initial_size = fmla.size();
    
    set<string> fmla_parameters_set = get_all_parameters_of_fmla(fmla);
    
    set<int> parameters_idxs = get_parameters_idxs(fmla);

    for (int parameter_idx : parameters_idxs) {
        string parameter_str = fmla[parameter_idx].data;
        if (parameters_subs_domain.find(parameter_str) == parameters_subs_domain.end()) {
            string new_parameter = get_new_parameter(parameters_subs);

            fmla_parameters_set.insert(new_parameter);

            TermNode term_node;
            term_node.data = new_parameter;
            term_node.parent = 0;
            term_node.children = {};

            subs[parameter_str] = {term_node};
            parameters_subs.insert(new_parameter);
        }
    }

    for (int i = 0; i < fmla_initial_size; i++) {
        if (parameters_idxs.find(i) != parameters_idxs.end()) {
            fmla = subst_parameter_by_term(fmla, i, subs[fmla[i].data]);
        }
    }

   return fmla;
}

string get_new_parameter(set<string> parameters) {
    for (int i = 1; i <= parameters.size(); i++) {
        string parameter_test = "p" + to_string(i);
        if (parameters.find(parameter_test) == parameters.end()) {
            return parameter_test;
        }
    }
    return "p" + to_string(parameters.size() + 1);
}

set<string> get_all_parameters_of_fmla(Fmla fmla) {
    set<string> parameters;

    for (int i = 0; i < fmla.size(); i++) {
        FmlaNode fmla_node = fmla[i];
        if (is_a_parameter(fmla_node)) parameters.insert(fmla_node.data);

    }

    return parameters;
}

bool term_in_vector_of_terms(Term term, vector<Term> terms) {
    bool result = false;
    for (Term t : terms) {
        if (term_equality(term, t)) result = true;
    }
    return result;
}

vector<string> get_all_parameters_of_term(Term term) {
    vector<string> parameters;

    for (int i = 0; i < term.size(); i++) {
        TermNode term_node = term[i];
        if (is_a_parameter(term_node)) parameters.push_back(term_node.data);

    }

    return parameters;
}

set<string> get_all_parameters(vector<Fmla> fmlas) {
    set<string> parameters;

    for (Fmla fmla : fmlas){
        for (int i = 0; i < fmla.size(); i++) {
            FmlaNode fmla_node = fmla[i];
            if (is_a_parameter(fmla_node)) parameters.insert(fmla_node.data);

        }
    }

    return parameters;
}

bool fmla_equality(Fmla fmla1, Fmla fmla2) {
    if (fmla1.size() != fmla2.size()) return false;

    queue<FmlaNode> q1, q2;
    q1.push(fmla1[0]);
    q2.push(fmla2[0]);

    while (!q1.empty()) {
        FmlaNode current1 = q1.front();
        FmlaNode current2 = q2.front();

        if (current1.data != current2.data) return false;

        q1.pop(); q2.pop();


        if (current1.children.size() != current2.children.size()) {
            return false;
        }
        else {
            for (int i = 0; i < current1.children.size(); i++) {
                q1.push(fmla1[current1.children[i]]);
                q2.push(fmla2[current2.children[i]]);
            }
        }
    }

    return true;
}

bool fmla_equality_under_subst(Fmla fmla1, Fmla fmla2) {
    if (fmla1.size() != fmla2.size()) return false;

    map<string, string> subst;
    set<string> subst_img;

    queue<FmlaNode> q1, q2;
    q1.push(fmla1[0]);
    q2.push(fmla2[0]);

    while (!q1.empty()) {
        FmlaNode current1 = q1.front();
        FmlaNode current2 = q2.front();
        q1.pop(); q2.pop();

        if (is_a_parameter(current1) && is_a_parameter(current2)) {
            if (subst.find(current1.data) != subst.end()) {
                if (subst[current1.data] != current2.data) return false;
            }
            else {
                if (subst_img.find(current2.data) != subst_img.end()) return false;
                else {
                    subst[current1.data] = current2.data;
                    subst_img.insert(current2.data);
                }
            }
        }
        else if (current1.data != current2.data) return false;

        if (current1.children.size() != current2.children.size()) {
            return false;
        }
        else {
            for (int i = 0; i < current1.children.size(); i++) {
                q1.push(fmla1[current1.children[i]]);
                q2.push(fmla2[current2.children[i]]);
            }
        }
    }

    return true;

}

bool are_syntactically_isomorphic(Fmla fmla1, Fmla fmla2) {
    if (fmla1.size() != fmla2.size()) return false;

    map<string, string> subst;
    set<string> subst_img;

    queue<FmlaNode> q1, q2;
    q1.push(fmla1[0]);
    q2.push(fmla2[0]);

    while (!q1.empty()) {
        FmlaNode current1 = q1.front();
        FmlaNode current2 = q2.front();
        q1.pop(); q2.pop();

        if (is_a_parameter(current1) && is_a_parameter(current2)) {
            if (subst.find(current1.data) != subst.end()) {
                if (subst[current1.data] != current2.data) return false;
            }
            else {
                if (subst_img.find(current2.data) != subst_img.end()) return false;
                else {
                    subst[current1.data] = current2.data;
                    subst_img.insert(current2.data);
                }
            }
        }

        if (current1.children.size() != current2.children.size()) {
            return false;
        }
        else {
            for (int i = 0; i < current1.children.size(); i++) {
                q1.push(fmla1[current1.children[i]]);
                q2.push(fmla2[current2.children[i]]);
            }
        }
    }

    return true;

}

bool are_isomorphic_with_equal_parameters(Fmla fmla1, Fmla fmla2) {
    if (fmla1.size() != fmla2.size()) return false;

    queue<FmlaNode> q1, q2;
    q1.push(fmla1[0]);
    q2.push(fmla2[0]);

    while (!q1.empty()) {
        FmlaNode current1 = q1.front();
        FmlaNode current2 = q2.front();
        q1.pop(); q2.pop();

        if (is_a_parameter(current1) && is_a_parameter(current2)) {
            if (current1.data != current2.data) {
                return false;
            }            
        }

        if (current1.children.size() != current2.children.size()) {
            return false;
        }
        else {
            for (int i = 0; i < current1.children.size(); i++) {
                q1.push(fmla1[current1.children[i]]);
                q2.push(fmla2[current2.children[i]]);
            }
        }
    }

    return true;

}

bool term_equality(Term term1, Term term2) {
    if (term1.size() != term2.size()) return false;

    queue<TermNode> q1, q2;
    q1.push(term1[0]);
    q2.push(term2[0]);

    while (!q1.empty()) {
        TermNode current1 = q1.front();
        TermNode current2 = q2.front();

        if (current1.data != current2.data) return false;

        q1.pop(); q2.pop();

        if (current1.children.size() != current2.children.size()) {
            return false;
        }
        else {
            for (int i = 0; i < current1.children.size(); i++) {
                q1.push(term1[current1.children[i]]);
                q2.push(term2[current2.children[i]]);
            }
        }
    }

    return true;
}

int get_term_idx_img_subst(Fmla fmla1, int parameter_idx, Fmla fmla2) {
    // Assume subst(fmla1) = fmla2. We want to find where the parameter in the position of parameter_idx is substituted in fmla2 

    queue<int> q1, q2;
    q1.push(0);
    q2.push(0);

    while(!q1.empty()) {
        int curr1 = q1.front();
        int curr2 = q2.front();

        q1.pop(); q2.pop();

        if (curr1 == parameter_idx) {
            return curr2;
        }
        for (int i = 0; i < fmla1[curr1].children.size(); i++) {
            q1.push(fmla1[curr1].children[i]);
            q2.push(fmla2[curr2].children[i]);
        }
    }

    return -1;
}

// Fmla left_gen_subfmla(Fmla fmla) {
//     Fmla subfmla1 = get_subfmla(fmla, fmla[0].children[0]);

//     int right_root = fmla[0].children[1];
//     Fmla subfmla2 = get_subfmla(fmla, fmla[right_root].children[0]);

//     Fmla resulting_fmla = {
//         {"∈", 0, {}}
//     };
//     resulting_fmla = join_subfmla(resulting_fmla, 0, 1, subfmla1);
//     resulting_fmla = join_subfmla(resulting_fmla, 0, 2, subfmla2);

//     return resulting_fmla;
// }

// Fmla right_gen_subfmla(Fmla fmla) {
//     Fmla subfmla1 = get_subfmla(fmla, fmla[0].children[0]);

//     int right_root = fmla[0].children[1];
//     Fmla subfmla2 = get_subfmla(fmla, fmla[right_root].children[1]);

//     Fmla resulting_fmla = {
//         {"∈", 0, {}}
//     };
//     resulting_fmla = join_subfmla(resulting_fmla, 0, 1, subfmla1);
//     resulting_fmla = join_subfmla(resulting_fmla, 0, 2, subfmla2);

//     return resulting_fmla;

// }

// Fmla join_subfmla(Fmla fmla, int parent_node, int child_position, Fmla subfmla) {
//     Fmla resulting_fmla = fmla;

//     int size = resulting_fmla.size();

//     // Add subfmla index in the children of the new parent node of submla
//     resulting_fmla[parent_node].children.insert(resulting_fmla[parent_node].children.begin() + child_position - 1, size);

//     // Join subfmla in fmla
//     for(int i = 0; i < subfmla.size(); i++){
//         FmlaNode fmla_node;

//         fmla_node.data = subfmla[i].data;
//         if (i == 0){
//             fmla_node.parent = parent_node;
//         }
//         else {
//             fmla_node.parent = subfmla[i].parent + size;
//         }
//         vector<int> children;
//         for (auto child : subfmla[i].children){
//             children.push_back(child + size);
//         }
//         fmla_node.children = children;

//         resulting_fmla.push_back(fmla_node);
//     }
//     return resulting_fmla;
// }

// set<string> get_fmla_vars(Fmla fmla){
//     set<string> vars;

//     for (int i = 0; i < fmla.size(); i++) {
//         if (fmla[i].children.size() == 0 && fmla[i].data != "∅") {
//             vars.insert(fmla[i].data);
//         }
//     }

//     return vars;
// }

// bool fmla_equality(Fmla fmla1, Fmla fmla2) {
//     if (fmla1.size() != fmla2.size()) return false;

//     queue<FmlaNode> q1, q2;
//     q1.push(fmla1[0]);
//     q2.push(fmla2[0]);

//     while (!q1.empty()) {
//         FmlaNode current1 = q1.front();
//         FmlaNode current2 = q2.front();

//         if (current1.data != current2.data) return false;

//         q1.pop(); q2.pop();


//         if (current1.children.size() != current2.children.size()) {
//             return false;
//         }
//         else {
//             for (int i = 0; i < current1.children.size(); i++) {
//                 q1.push(fmla1[current1.children[i]]);
//                 q2.push(fmla2[current2.children[i]]);
//             }
//         }
//     }

//     return true;
// }

// string get_new_var(set<string> vars) {
//     int int_new_var = 97; // 97 corresponds to "a" in ascii table

//     while (int_new_var <= 122) {
//         char c = static_cast<char>(int_new_var);
//         string string_c = string() + c;
//         if (vars.find(string_c) == vars.end()) {
//             return string_c;
//         }
//         int_new_var += 1;
//     }
//     return ""; // the case in which there are more variables in the tableau than letters in the alphabet is not covered
// }

// bool is_predicate_symb(Fmla fmla) {
//     if (fmla[0].data == "⊆" || fmla[0].data == "≍"){
//         return true;
//     }
//     return false;
// }

// bool is_function_symb(Fmla fmla) {
//     if (fmla[0].data == "∈"){
//         return true;
//     }
//     return false;
// }

// bool is_unary_function_symb(Fmla fmla) {
//     if (fmla[0].data == "∈"){
//         if (fmla[0].children.size() > 0){
//             FmlaNode right_root = fmla[fmla[0].children[1]];
//             if (right_root.data == "∁"){
//                 return true;
//             }
//         }
//     }
//     return false;
// }

// bool is_atomic(Fmla fmla) {
//     int right_root = fmla[0].children[1];
//     if (fmla[right_root].children.size() == 0){
//         return true;
//     }
//     else {
//         return false;
//     }
// }