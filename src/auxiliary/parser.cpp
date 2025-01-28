#include "parser.h"

using namespace std;

vector<FmlaNode> parse_fmla(string fmla) {

    map<string, int> function_symbs = process_function_symbs();
    map<string, int> predicate_symbs = process_predicate_symbs();

    vector<FmlaNode> resulting_fmla;

    // remove white spaces
    string fmla_no_space = "";
    for (char c : fmla) {
        if (!isspace(c)) {
            fmla_no_space += c;
        }
    }

    // process first characters
     
    // get the first symb which can be found until the first opening parentheses
    // get the outer commas

    // char blank_space = ' ';
    // fmla.erase(remove(fmla.begin(), fmla.end(), blank_space), fmla.end());

    vector<string> tokens_fmla = get_tokens(fmla);

    // check if parentheses are correct
    // check if amount of predicate symbols is correct

    pair<int, string> symb = get_pred_symb(tokens_fmla);
    FmlaNode fmla_node;

    fmla_node.data = symb.second;
    fmla_node.parent = 0;
    fmla_node.children = {};

    resulting_fmla.push_back(fmla_node);
    
    queue<tuple<int, vector<string>, int>> queue; // current_parent, text to be splitted, index of root symbol

    queue.push(make_tuple(0, tokens_fmla, symb.first));
    int curr_result_size;

    while (!queue.empty()) {

        tuple<int, vector<string>, int> current = queue.front();
        int curr_parent = get<0>(current);
        vector< vector<string>> children = split_by_symb_idx(get<1>(current), get<2>(current));
        
        queue.pop();

        for (vector<string> term : children) {
            pair<int, string> parsed_term = parse_term(term);

            curr_result_size = resulting_fmla.size();
            resulting_fmla[curr_parent].children.push_back(curr_result_size);
            
            queue.push(make_tuple(curr_result_size, term, parsed_term.first));
            
            if (parsed_term.second != "(" && parsed_term.second != ")"){
                FmlaNode child_node;

                child_node.data = parsed_term.second;
                child_node.parent = curr_parent;
                child_node.children = {};

                resulting_fmla.push_back(child_node);
            }
        }
    }    
    
    return resulting_fmla;

}

vector<string> split(string fmla, char delimiter) {
    vector<string> splitted;
    size_t start = 0;
    size_t end = fmla.find(delimiter);

    while (end != string::npos) {
        splitted.push_back(fmla.substr(start, end - start));
        start = end + 1;
        end = fmla.find(delimiter, start);
    }
    
    splitted.push_back(fmla.substr(start, end)); // Add the last token
    return splitted;
}

vector<string> get_tokens(string fmla) {

    vector<string> splitted_fmla = split(fmla, ' ');
    vector<string> tokens;

    for (int i = 0; i < splitted_fmla.size(); i++) {
        if (has_non_unicode(splitted_fmla[i]) == true) {
            tokens.push_back(splitted_fmla[i]);
        }
        else {
            for (char c : splitted_fmla[i]) {
                string string_token = "";
                string_token.push_back(c);
                tokens.push_back(string_token);
            }
        }
    }

    return tokens;
}

bool has_non_unicode(string str) {
    for (int i = 0; i < str.size(); i++) {
        char a = str[i];
        int a_int = static_cast<int>(a);
        if (a_int < 0) {
            return true;
        }
    }
    return false;
}

pair<int, string> get_pred_symb(vector<string> tokens_fmla) {
    for (int i = 0; i < tokens_fmla.size(); i++) {
        if (tokens_fmla[i] == "⊆" || tokens_fmla[i] == "≍" || tokens_fmla[i] == "∈") {
            return make_pair(i, tokens_fmla[i]);
        }
    }

    return make_pair(-1, "");
}

pair<int, string> parse_term(vector<string> tokens_term) {


    // check if parentheses are correct
        // unary symbol
        // bynary symbol
    // check if amount of predicate symbols is correct

    // vector<string> new_term = {};

    // // remove external parentheses
    // if (tokens_term[0] == "(") {
    //     for (int i = 1; i < tokens_term.size() - 1; i++) {
    //         new_term.push_back(tokens_term[i]);
    //     }
    // }
    // if (tokens_term[tokens_term.size() - 1] != ")") {
    //     new_term.push_back(tokens_term[tokens_term.size() - 1]);
    // }

    // atomic term
    if (tokens_term.size() == 1) {
        return make_pair(0, tokens_term[0]);
    }

    // non-atomic term
    bool has_parentheses = false;
    for (int i = 0; i < tokens_term.size(); i++) {
        if (tokens_term[i] == "(") {
            has_parentheses = true;
        }
    }

    if (has_parentheses == false) { // that is, the term is a composition of two atomic terms
        for (int i = 0; i < tokens_term.size(); i++) {
            if (has_non_unicode(tokens_term[i]) == true || tokens_term[i] == "-") {
                return make_pair(i, tokens_term[i]);
            }
        }
    }
    else {
        int opened_parentheses = 0;
        for (int i = 0; i < tokens_term.size(); i++) {
            if (tokens_term[i] == "(") {
                opened_parentheses++;
            }
            else if (tokens_term[i] == ")") {
                opened_parentheses--;
            }
            else if (opened_parentheses == 0 && i > 0) {
                return make_pair(i, tokens_term[i]);
            }
        }
    }
    return make_pair(-1, "");
}

vector< vector<string>> split_by_symb_idx(vector<string> tokens_fmla, int symb_idx) {
    vector< vector<string>> resulting_strings;

    vector<string> string1 = {}, string2 = {};

    for (int i = 0; i < symb_idx; i++) {
        string1.push_back(tokens_fmla[i]);
    }

    for (int i = symb_idx + 1; i < tokens_fmla.size(); i++) {
        string2.push_back(tokens_fmla[i]);
    }

    if (string1.size() > 0)
        resulting_strings.push_back(remove_external_parentheses(string1));
    if (string2.size() > 0)
        resulting_strings.push_back(remove_external_parentheses(string2));

    return resulting_strings;
}

vector<string> remove_external_parentheses(vector<string> str) {
    vector<string> result_string = {};
    if (str[0] == "(" && str[str.size() - 1] == ")") {
        for (int i = 1; i < str.size() - 1; i++) {
            result_string.push_back(str[i]);
        }
        return result_string;
    }
    else {
        return str;
    }
}
