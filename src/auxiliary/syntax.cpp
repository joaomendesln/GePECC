#include "syntax.h"

using namespace std;

bool is_function_symb(string symb) {
    return false;
}

bool is_predicate_symb(string symb) {
    return true;
}

// vector<FmlaNode> get_subfmla(vector<FmlaNode> fmla, int subfmla_root) {
//     set<int> subfmla_nodes_idx;
//     // int root = fmla[0].children[0];

//     subfmla_nodes_idx.insert(subfmla_root);

//     // BFS in the left subformula of fmla
//     queue<int> q;
//     q.push(subfmla_root);

//     while (!q.empty()) {
//         int current = q.front();
//         q.pop();

//         for (int child : fmla[current].children) {
//             q.push(child);
//             subfmla_nodes_idx.insert(child);
//         }
//     }

//     // Map to make the correct attribution for the new ids of the left subfmla
//     map<int, int> new_idx;
//     int count = 0;
    
//     for(int i = 0; i < fmla.size(); i++){
//         if(subfmla_nodes_idx.find(i) != subfmla_nodes_idx.end()){
//             new_idx[i] = count;
//             count += 1;
//         }
//     }

//     // Structure left subformula tree
//     vector<FmlaNode> subfmla;
    
//     for(int i = 0; i < fmla.size(); i++){
//         if(subfmla_nodes_idx.find(i) != subfmla_nodes_idx.end()){
//             FmlaNode fmla_node;

//             fmla_node.data = fmla[i].data;
//             if (i == subfmla_root){
//                 fmla_node.parent = 0;
//             }
//             else {
//                 fmla_node.parent = new_idx[fmla[i].parent];
//             }
//             vector<int> children;
//             for (auto child : fmla[i].children){
//                 children.push_back(new_idx[child]);
//             }
//             fmla_node.children = children;

//             subfmla.push_back(fmla_node);
//         }
//     }

//     return subfmla;
// }

// vector<FmlaNode> left_gen_subfmla(vector<FmlaNode> fmla) {
//     vector<FmlaNode> subfmla1 = get_subfmla(fmla, fmla[0].children[0]);

//     int right_root = fmla[0].children[1];
//     vector<FmlaNode> subfmla2 = get_subfmla(fmla, fmla[right_root].children[0]);

//     vector<FmlaNode> resulting_fmla = {
//         {"∈", 0, {}}
//     };
//     resulting_fmla = join_subfmla(resulting_fmla, 0, 1, subfmla1);
//     resulting_fmla = join_subfmla(resulting_fmla, 0, 2, subfmla2);

//     return resulting_fmla;
// }

// vector<FmlaNode> right_gen_subfmla(vector<FmlaNode> fmla) {
//     vector<FmlaNode> subfmla1 = get_subfmla(fmla, fmla[0].children[0]);

//     int right_root = fmla[0].children[1];
//     vector<FmlaNode> subfmla2 = get_subfmla(fmla, fmla[right_root].children[1]);

//     vector<FmlaNode> resulting_fmla = {
//         {"∈", 0, {}}
//     };
//     resulting_fmla = join_subfmla(resulting_fmla, 0, 1, subfmla1);
//     resulting_fmla = join_subfmla(resulting_fmla, 0, 2, subfmla2);

//     return resulting_fmla;

// }

// vector<FmlaNode> join_subfmla(vector<FmlaNode> fmla, int parent_node, int child_position, vector<FmlaNode> subfmla) {
//     vector<FmlaNode> resulting_fmla = fmla;

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

// set<string> get_fmla_vars(vector<FmlaNode> fmla){
//     set<string> vars;

//     for (int i = 0; i < fmla.size(); i++) {
//         if (fmla[i].children.size() == 0 && fmla[i].data != "∅") {
//             vars.insert(fmla[i].data);
//         }
//     }

//     return vars;
// }

// bool fmla_equality(vector<FmlaNode> fmla1, vector<FmlaNode> fmla2) {
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

// bool is_predicate_symb(vector<FmlaNode> fmla) {
//     if (fmla[0].data == "⊆" || fmla[0].data == "≍"){
//         return true;
//     }
//     return false;
// }

// bool is_function_symb(vector<FmlaNode> fmla) {
//     if (fmla[0].data == "∈"){
//         return true;
//     }
//     return false;
// }

// bool is_unary_function_symb(vector<FmlaNode> fmla) {
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

// bool is_atomic(vector<FmlaNode> fmla) {
//     int right_root = fmla[0].children[1];
//     if (fmla[right_root].children.size() == 0){
//         return true;
//     }
//     else {
//         return false;
//     }
// }