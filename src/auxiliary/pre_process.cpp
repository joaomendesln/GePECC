#include "pre_process.h"

using namespace std;

vector<string> open_file(string file_path) {
    vector<string> lines;

    ifstream file(file_path);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << file_path << "\n";
        return lines;
    }

    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }

    file.close();
    if (lines.size() == 0) {
        cout << "Empty file\n";
    }
    return lines;
}

map<string, int> pre_process_symb_line(string symb_line) {
    map<string, int> resulting_symbs_map;

    // get arity and symbols in this line
    string arity_str = "";
    string symbols_str = "";
    bool symbols_init = false;
    
    for (char c : symb_line) {
        if (c == ':') {
            symbols_init = true;
        }
        else {
            if (symbols_init == false) arity_str += c;
            else symbols_str += c;
        }
    }

    // convert the arity of the symbols in this line to integer
    int arity_int = stoi(arity_str);

    // split the rest of the line by commas
    stringstream stream_func(symbols_str);
    string symbol;
    vector<string> symbs_with_space;

    while (getline(stream_func, symbol, ',')) {
        symbs_with_space.push_back(symbol);
    }

    // remove white spaces and adding function symbol to the map
    for (int j = 0; j < symbs_with_space.size(); j++) {
        symbol = "";
        for (char c : symbs_with_space[j]) {
            if (!isspace(c)) {
                symbol += c;
            }
        }
        resulting_symbs_map[symbol] = arity_int;
    }

    return resulting_symbs_map;
}

set<string> pre_process_skolem_symbs() {
    string line = "f, g";

    if (line.size() == 0){
        string file_path = "../src/inputs/skolem_symbols";
        line = open_file(file_path)[0];
    }

    string skolem_symbs_no_space = "";

    for (char c : line) {
        if(!isspace(c)) {
            skolem_symbs_no_space += c;
        }
    }

    vector<string> skolem_symbs_vec;
    stringstream stream_func(skolem_symbs_no_space);
    string symb;

    while (getline(stream_func, symb, ',')) {
        skolem_symbs_vec.push_back(symb);
    }

    set<string> skolem_symbs;

    for (string symb : skolem_symbs_vec) {
        skolem_symbs.insert(symb);
    }

    return skolem_symbs;
}

map<string, int> pre_process_function_symbs() {

    map <string, int> resulting_function_symbs;

    vector<string> lines = {"function", "0: ∅", "1: fst, snd", "2: f, g, ∩, ∪, ×, △, -", "predicate", "2: ∈, ⊆, ⪥"};
    // vector<string> lines = {};
    
    if (lines.size() == 0){
        string file_path = "../src/inputs/symbols";
        lines = open_file(file_path);
    }

    if (lines[0] != "function") {
        cout << "Wrong file fomatting\n";
        return resulting_function_symbs;
    }

    for (int i = 1; i < lines.size(); i++) {
        if (lines[i] == "predicate") break;

        map<string, int> line_map = pre_process_symb_line(lines[i]);
        for (const auto& pair : line_map) {
            resulting_function_symbs[pair.first] = pair.second;
        }

    }
    return resulting_function_symbs;

}

map<string, int> pre_process_predicate_symbs() {
    map <string, int> resulting_predicate_symbs;

    vector<string> lines = {"function", "0: ∅", "1: fst, snd", "2: f, g, ∩, ∪, ×, △, -", "predicate", "2: ∈, ⊆, ⪥"};
    // vector<string> lines = {};

    if (lines.size() == 0){
        string file_path = "../src/inputs/symbols";
        lines = open_file(file_path);
    }

    if (lines[0] != "function") {
        cout << "Wrong file fomatting\n";
        return resulting_predicate_symbs;
    }

    int predicate_lines_init = -1;

    for (int i = 1; i < lines.size(); i++) {
        if (lines[i] == "predicate") predicate_lines_init = i;
    }

    if (predicate_lines_init == -1) {
        cout << "Wrong file fomatting\n";
        return resulting_predicate_symbs;
    }

    for (int i = predicate_lines_init + 1; i < lines.size(); i++) {
        map<string, int> line_map = pre_process_symb_line(lines[i]);
        for (const auto& pair : line_map) {
            resulting_predicate_symbs[pair.first] = pair.second;
        }
    }
    return resulting_predicate_symbs;

}

map<string, int> pre_process_language_symbs() {
    map<string, int> function_symbs = pre_process_function_symbs();
    map<string, int> predicate_symbs = pre_process_predicate_symbs();

    map<string, int> language_symbs;

    for (const auto& pair : function_symbs) {
        language_symbs[pair.first] = pair.second;
    }

    for (const auto& pair : predicate_symbs) {
        language_symbs[pair.first] = pair.second;
    }

    return language_symbs;
}

map<string, int> pre_process_no_skolem_symbs() {
    map<string, int> function_symbs = pre_process_function_symbs();
    map<string, int> predicate_symbs = pre_process_predicate_symbs();
    set<string> skolem_symbs = pre_process_skolem_symbs();

    map<string, int> no_skolem_symbs;

    for (const auto& pair : function_symbs) {
        if (skolem_symbs.find(pair.first) == skolem_symbs.end()) no_skolem_symbs[pair.first] = pair.second;
    }

    for (const auto& pair : predicate_symbs) {
        no_skolem_symbs[pair.first] = pair.second;
    }

    return no_skolem_symbs;

}

TblRule pre_process_single_expansion_rule(string line) {

    string str_premisses;
    string str_conclusions;
    bool premisses_finished = false;
    for (int i = 0; i < line.size(); i++) {
        char c = line[i];

        if (c == ';') premisses_finished = true;

        if (premisses_finished == false) {
            str_premisses += c;
        }
        if (premisses_finished == true && c != ';') {
            str_conclusions += c;
        }
    }

    vector<SignedFmla> premisses = pre_process_signed_fmla_list(str_premisses);
    vector<SignedFmla> conclusions = pre_process_signed_fmla_list(str_conclusions);

    TblRule expansion_rule;
    expansion_rule.premisses = premisses;
    expansion_rule.conclusions = conclusions;
    expansion_rule.is_cut = false;

    return expansion_rule;
}

vector<SignedFmla> pre_process_signed_fmla_list(string list) {
    // cout << list << "\n";
    if (list.size() == 0) return {};

    vector<SignedFmla> signed_fmla_vec;

    
    int parentheses_counter = 0;
    string signed_fmla_str = "";

    for (int i = 0; i < list.size(); i++) {
        char c = list[i];

        // Control opening/closing parenthesis
        if (c == '(') parentheses_counter++;
        if (c == ')') parentheses_counter--;

        // Verifications regarding comma
        if (c != ',') {
            signed_fmla_str += c;
        }
        else {
            if (parentheses_counter == 0) {
                signed_fmla_vec.push_back(pre_process_signed_fmla(signed_fmla_str));
                signed_fmla_str = "";
            }
            else {
                signed_fmla_str += c;
            }
        }

        // End of the string
        if (i == list.size() - 1) {
            signed_fmla_vec.push_back(pre_process_signed_fmla(signed_fmla_str));
        }
    }

    return signed_fmla_vec;
}

SignedFmla pre_process_signed_fmla(string signed_fmla_str) {
    SignedFmla signed_fmla;

    string signed_fmla_str_no_space = "";

    for (char c : signed_fmla_str) {
        if(!isspace(c)) {
            signed_fmla_str_no_space += c;
        }
    }

    if (signed_fmla_str_no_space[0] != '(') {
        cerr << "Error pre-processing the signed formula " << signed_fmla_str << "\n";
        cerr << "Missing opening parenthesis\n";
        return signed_fmla;
    }
    if (signed_fmla_str_no_space[signed_fmla_str_no_space.size() - 1] != ')') {
        cerr << "Error pre-processing the signed formula " << signed_fmla_str << "\n";
        cerr << "Missing closing parenthesis\n";
        return signed_fmla;
    }


    // set polarity
    if (signed_fmla_str_no_space[1] == ',') signed_fmla.sign = polarity::none;
    else if (signed_fmla_str_no_space[1] == '-') signed_fmla.sign = polarity::minus;
    else if (signed_fmla_str_no_space[1] == '+') signed_fmla.sign = polarity::plus;
    else {
        cerr << "Error pre-processing the signed formula " << signed_fmla_str << "\n";
        cerr << "Wrong polarity\n";
        return signed_fmla;
    }

    // set formula
    int init_fmla = 0;
    char comma = signed_fmla_str_no_space[init_fmla];
    while (comma != ',') {
        init_fmla++;
        comma = signed_fmla_str_no_space[init_fmla];
    }

    string fmla_str;
    for (int i = init_fmla + 1; i < signed_fmla_str_no_space.size() - 1; i++) {
        char c = signed_fmla_str_no_space[i];
        fmla_str += c;
    }

    signed_fmla.fmla = parse_fmla(fmla_str);

    return signed_fmla;
}

vector<TblRule> pre_process_expansion_rules_input() {

    vector<TblRule> resulting_expansion_rules;

    // vector<string> lines = {
    //     "[ax∩]",
    //     "(+, ∈(p1, ∩(p2, p3))); (+, ∈(p1, p2))",
    //     "(+, ∈(p1, ∩(p2, p3))); (+, ∈(p1, p3))",
    //     "(-, ∈(p1, ∩(p2, p3))), (+, ∈(p1, p2)); (-, ∈(p1, p3))",
    //     "(-, ∈(p1, ∩(p2, p3))), (+, ∈(p1, p3)); (-, ∈(p1, p2))",
    //     "[ax∪]",
    //     "(-, ∈(p1, ∪(p2, p3))); (-, ∈(p1, p2))",
    //     "(-, ∈(p1, ∪(p2, p3))); (-, ∈(p1, p3))",
    //     "(+, ∈(p1, ∪(p2, p3))), (-, ∈(p1, p2)); (+, ∈(p1, p3))",
    //     "(+, ∈(p1, ∪(p2, p3))), (-, ∈(p1, p3)); (+, ∈(p1, p2))",
    //     "; (-, ∈(p1,∅))"};

    // vector<string> lines = 
    // {
    //     "[ax∩]",
    //     "(+, ∈(p1, ∩(p2, p3))); (+, ∈(p1, p2))",
    //     "(+, ∈(p1, ∩(p2, p3))); (+, ∈(p1, p3))",
    //     "(-, ∈(p1, ∩(p2, p3))), (+, ∈(p1, p2)); (-, ∈(p1, p3))",
    //     "(-, ∈(p1, ∩(p2, p3))), (+, ∈(p1, p3)); (-, ∈(p1, p2))",
    //     "(+, ∈(p1, p2)), (+, ∈(p1, p3)); (+, ∈(p1, ∩(p2, p3)))",
    //     "(-, ∈(p1, p2)); (-, ∈(p1, ∩(p2, p3)))",
    //     "(-, ∈(p1, p3)); (-, ∈(p1, ∩(p2, p3)))",
    //     "[ax∪]",
    //     "(-, ∈(p1, ∪(p2, p3))); (-, ∈(p1, p2))",
    //     "(-, ∈(p1, ∪(p2, p3))); (-, ∈(p1, p3))",
    //     "(+, ∈(p1, ∪(p2, p3))), (-, ∈(p1, p2)); (+, ∈(p1, p3))",
    //     "(+, ∈(p1, ∪(p2, p3))), (-, ∈(p1, p3)); (+, ∈(p1, p2))",
    //     "(+, ∈(p1, p2)); (+, ∈(p1, ∪(p2, p3)))",
    //     "(+, ∈(p1, p3)); (+, ∈(p1, ∪(p2, p3)))",
    //     "(-, ∈(p1, p2)), (-, ∈(p1, p3)); (-, ∈(p1, ∪(p2, p3)))",
    //     "[ax∅]",
    //     ";(-, ∈(p1,∅))",
    //     "[ax×]",
    //     "(+, ∈(p1, ×(p2,p3))); (+, ∈(fst(p1), p2))",
    //     "(+, ∈(p1, ×(p2,p3))); (+, ∈(snd(p1), p3))",
    //     "(+, ∈(fst(p1), p2)), (-, ∈(p1, ×(p2,p3))); (-, ∈(snd(p1), p3))",
    //     "(+, ∈(snd(p1), p3)), (-, ∈(p1, ×(p2,p3))); (-, ∈(fst(p1), p2))",
    //     "(+, ∈(fst(p1), p2)), (+, ∈(snd(p1), p3)); (+, ∈(p1, ×(p2,p3)))",
    //     "(-, ∈(fst(p1), p2)); (-, ∈(p1, ×(p2,p3)))",
    //     "(-, ∈(snd(p1), p3)); (-, ∈(p1, ×(p2,p3)))",
    //     "[ax⊆]",
    //     "(+, ∈(p3, p1)), (+, ⊆(p1,p2)); (+, ∈(p3,p2))",
    //     "(-, ∈(p3,p2)), (+, ⊆(p1,p2)); (-, ∈(p3, p1))",
    //     // "(+, ∈(p3, p1)), (-, ∈(p3,p2)); (-, ⊆(p1,p2))",
    //     // "(-, ∈(f(p1,p2), p1)); (+, ⊆(p1, p2))",
    //     // "(+, ∈(f(p1,p2), p2)); (+, ⊆(p1, p2))",
    //     "(-, ⊆(p1, p2)); (+, ∈(f(p1,p2), p1))",
    //     "(-, ⊆(p1, p2)); (-, ∈(f(p1,p2), p2))",
    // };

    vector<string> lines = 
    {
        "[ax∩]",
        "(+, ∈(p1, ∩(p2, p3))); (+, ∈(p1, p2)), (+, ∈(p1, p3))",
        "(-, ∈(p1, ∩(p2, p3))), (+, ∈(p1, p2)); (-, ∈(p1, p3))",
        "(-, ∈(p1, ∩(p2, p3))), (+, ∈(p1, p3)); (-, ∈(p1, p2))",
        // "(+, ∈(p1, p2)), (+, ∈(p1, p3)); (+, ∈(p1, ∩(p2, p3)))",
        // "(-, ∈(p1, p2)); (-, ∈(p1, ∩(p2, p3)))",
        // "(-, ∈(p1, p3)); (-, ∈(p1, ∩(p2, p3)))",
        "[ax∪]",
        "(-, ∈(p1, ∪(p2, p3))); (-, ∈(p1, p2)), (-, ∈(p1, p3))",
        "(+, ∈(p1, ∪(p2, p3))), (-, ∈(p1, p2)); (+, ∈(p1, p3))",
        "(+, ∈(p1, ∪(p2, p3))), (-, ∈(p1, p3)); (+, ∈(p1, p2))",
        // "(+, ∈(p1, p2)); (+, ∈(p1, ∪(p2, p3)))",
        // "(+, ∈(p1, p3)); (+, ∈(p1, ∪(p2, p3)))",
        // "(-, ∈(p1, p2)), (-, ∈(p1, p3)); (-, ∈(p1, ∪(p2, p3)))",
        "[ax-]",
        "(+, ∈(p1, -(p2, p3))); (+, ∈(p1, p2)), (-, ∈(p1, p3))",
        "(-, ∈(p1, -(p2, p3))), (+, ∈(p1, p2)); (+, ∈(p1, p3))",
        "(-, ∈(p1, -(p2, p3))), (-, ∈(p1, p3)); (-, ∈(p1, p2))",
        // "(+, ∈(p1, p2)); (+, ∈(p1, ∪(p2, p3)))",
        // "(+, ∈(p1, p3)); (+, ∈(p1, ∪(p2, p3)))",
        // "(-, ∈(p1, p2)), (-, ∈(p1, p3)); (-, ∈(p1, ∪(p2, p3)))",
        "[ax△]",
        "(+, ∈(p1, △(p2, p3))), (-, ∈(p1, p2)); (+, ∈(p1, p3))",
        "(+, ∈(p1, △(p2, p3))), (-, ∈(p1, p3)); (+, ∈(p1, p2))",
        "(+, ∈(p1, △(p2, p3))), (+, ∈(p1, p2)); (-, ∈(p1, p3))",
        "(+, ∈(p1, △(p2, p3))), (+, ∈(p1, p3)); (-, ∈(p1, p2))",
        "(-, ∈(p1, △(p2, p3))), (-, ∈(p1, p2)); (-, ∈(p1, p3))",
        "(-, ∈(p1, △(p2, p3))), (-, ∈(p1, p3)); (-, ∈(p1, p2))",
        "(-, ∈(p1, △(p2, p3))), (+, ∈(p1, p2)); (+, ∈(p1, p3))",
        "(-, ∈(p1, △(p2, p3))), (+, ∈(p1, p3)); (+, ∈(p1, p2))",
        "[ax∅]",
        "(+, ∈(p1,∅));",
        "[ax×]",
        "(+, ∈(p1, ×(p2,p3))); (+, ∈(fst(p1), p2)), (+, ∈(snd(p1), p3))",
        "(+, ∈(fst(p1), p2)), (-, ∈(p1, ×(p2,p3))); (-, ∈(snd(p1), p3))",
        "(+, ∈(snd(p1), p3)), (-, ∈(p1, ×(p2,p3))); (-, ∈(fst(p1), p2))",
        // "(+, ∈(fst(p1), p2)), (+, ∈(snd(p1), p3)); (+, ∈(p1, ×(p2,p3)))",
        // "(-, ∈(fst(p1), p2)); (-, ∈(p1, ×(p2,p3)))",
        // "(-, ∈(snd(p1), p3)); (-, ∈(p1, ×(p2,p3)))",
        "[ax⊆]",
        "(+, ∈(p3,p1)), (+, ⊆(p1,p2)); (+, ∈(p3,p2))",
        "(-, ∈(p3,p2)), (+, ⊆(p1,p2)); (-, ∈(p3,p1))",
        // "(+, ∈(p3, p1)), (-, ∈(p3,p2)); (-, ⊆(p1,p2))",
        // "(-, ∈(f(p1,p2), p1)); (+, ⊆(p1, p2))",
        // "(+, ∈(f(p1,p2), p2)); (+, ⊆(p1, p2))",
        "(-, ⊆(p1, p2)); (+, ∈(f(p1,p2), p1)), (-, ∈(f(p1,p2), p2))",
        "[ax⪥]",
        "(+, ∈(p3,p1)), (+, ⪥(p1,p2)); (-, ∈(p3,p2))",
        "(+, ∈(p3,p2)), (+, ⪥(p1,p2)); (-, ∈(p3,p1))",
        // "(+, ∈(p3, p1)), (+, ∈(p3,p2)); (-, ⪥(p1,p2))",
        "(-, ∈(g(p1,p2), p1)); (+, ⪥(p1, p2))",
        "(-, ∈(g(p1,p2), p2)); (+, ⪥(p1, p2))",
        "(-, ⪥(p1, p2)); (+, ∈(g(p1,p2), p1)), (+, ∈(g(p1,p2), p2))",
    };

    if (lines.size() == 0){
        string file_path = "../src/inputs/expansion_rules";
        lines = open_file(file_path);
    }

    for (int i = 0; i < lines.size(); i++) {
        string line = lines[i];
        if (line.size() > 0 && line[0] != '[') {
            // cout << line << "\n";
            resulting_expansion_rules.push_back(pre_process_single_expansion_rule(lines[i]));
        }
    }

    return resulting_expansion_rules;

}

vector<SignedFmla> pre_process_signed_fmla_input() {
    vector<SignedFmla> resulting_signed_fmlas;

    // // p1 ∈ (p2 ∩ p3) ⊢ p1 ∈ p2
    // vector<string> lines = {
    //     "(+, ∈(p1, ∩(p2,p3)))",
    //     "(-, ∈(p1, p2))"
    // };

    // // ⊢ ∅ ⊆ p1 ∩ p2
    // vector<string> lines = {
    //     "(-, ⊆(∅, ∩(p1,p2)))"
    // };

    // // p1 ∈ (p2 ∪ p3) ⊢ p1 ∈ p2
    // vector<string> lines = {
    //     "(+, ∈(p1, ∪(p2,p3)))",
    //     "(-, ∈(p1, p2))"
    // };

    // // p1 ∈ (p2 ∩ (p3 ∪ p4)) ⊢ p1 ∈ ((p2 ∩ p3) ∪ p4)
    // vector<string> lines = {
    //     "(+, ∈(p1, ∩(p2, ∪(p3, p4))))", 
    //     "(-, ∈(p1, ∪(∩(p2, p3), p4)))"
    // }; 

    // // ⊢ (p2 ∩ (p3 ∪ p4)) ⪥ ((p2 △ p3) - p4)
    // vector<string> lines = {
    //     "(-, ⪥(∩(p2, ∪(p3, p4)), -(△(p2, p3), p4)))"
    // };

    // // ⊢ p1 ∩ (p2 ∪ p3) ⊆ (p1 ∩ p2) ∪ p3
    // vector<string> lines = {
    //     "(-, ⊆(∩(p1, ∪(p2, p3)), ∪(∩(p1, p2), p3)))"
    // };

    // // ⊢ p1 ⊆ p1
    // vector<string> lines = {
    //     "(-, ⊆(p1, p1))"
    // }; 

    // // p1 ⊆ p2, p2 ⊆ p3 ⊢ p1 ⊆ p3
    // vector<string> lines = {
    //     "(+, ⊆(p1, p2))",
    //     "(+, ⊆(p2, p3))",
    //     "(-, ⊆(p1, p3))"
    // };

    // // p1 ⪥ p2 ⊢ p2 ⪥ p1
    // vector<string> lines = {
    //     "(+, ⪥(p1, p2))",
    //     "(-, ⪥(p2, p1))"
    // };

    // CUT

    // p1 ∈ (p2 ∩ p3) ⊢ p1 ∈ ((p2 ∪ p4) ∩ (p3 ∪ p5))
    vector<string> lines = {
        "(+, ∈(p1, ∩(p2, p3)))", 
        "(-, ∈(p1, ∩(∪(p2, p4), ∪(p3, p5))))"
    }; 

    // // p1 ∈ (p2 ∪ (p3 ∩ p4)) ⊢ p1 ∈ ((p2 ∪ p3) ∩ (p2 ∪ p4))
    // vector<string> lines = {
    //     "(+, ∈(p1, ∪(p2, ∩(p3, p4))))", 
    //     "(-, ∈(p1, ∩(∪(p2, p3), ∪(p2, p4))))"
    // };

    // // p5 ∈ ((p1 × p2) ∩ (p3 × p4)) ⊢ p5 ∈ ((p1 ∩ p3) × (p2 ∩ p4))
    // vector<string> lines = {
    //     "(+, ∈(p5,∩(×(p1,p2),×(p3,p4))))",
    //     "(-, ∈(p5,×(∩(p1,p3),∩(p2,p4))))"
    // };

    // // ⊢ ((p1 × p2) ∩ (p3 × p4)) ⊆ ((p1 ∩ p3) × (p2 ∩ p4))
    // vector<string> lines = {
    //     "(-, ⊆(∩(×(p1,p2),×(p3,p4)),×(∩(p1,p3),∩(p2,p4))))" 
    // }; 
    
    // CONTAINS DISPENSABLE NODES

    // // ⊢ (p1 ∩ (p2 ∩ p3)) ⊆ p2
    // vector<string> lines = {
    //     "(-, ⊆(∩(p1, ∩(p2, p3)), p2))"
    // }; 

    // // p1 ∈ (p2 ∩ (p3 ∩ p4)) ⊢ p1 ∈ p2
    // vector<string> lines = {
    //     "(+, ∈(p1, ∩(p2, ∩(p3, p4))))", 
    //     "(-, ∈(p1, p4))"
    // };

    // vector<string> lines = {};

    if (lines.size() == 0){
        string file_path = "../src/inputs/signed_fmlas";
        lines = open_file(file_path);
    }

    for (int i = 0; i < lines.size(); i++) {
        SignedFmla sf = pre_process_signed_fmla(lines[i]);
        resulting_signed_fmlas.push_back(sf);
    }

    return resulting_signed_fmlas;

}