/**
 * @file parser.cpp
 * @author João Mendes
 * @brief Parser of formulas in prefixed notation
 * @version 0.0.1
 * @date 2025-06-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "parser.h"

using namespace std;

/**
 * @brief Parses a formulas written in prefixed notation
 * 
 * @param fmla_str 
 * @return Formula described `fmla_str` parsed 
 */
Fmla parse_fmla(string fmla_str) {

    map<string, int> function_symbs = pre_process_function_symbs();
    map<string, int> predicate_symbs = pre_process_predicate_symbs();

    Fmla resulting_fmla;

    // remove white spaces
    string fmla_no_space = "";
    for (char c : fmla_str) {
        if (!isspace(c)) {
            fmla_no_space += c;
        }
    }

    queue<tuple<int, string>> queue; // current_parent, text to be splitted, index of root symbol

    queue.push(make_tuple(0, fmla_no_space));
    int current_fmla_size;

    while(!queue.empty()) {
        tuple<int, string> current = queue.front();
        int current_parent = get<0>(current);
        string string_to_parse = get<1>(current);
        
        queue.pop();

        // get main symbol of the string we are parsing
        string main_symb = "";
        string remaining_string;
        for (int i = 0; i < string_to_parse.size(); i++) {
            char c = string_to_parse[i];
            if (c != '(' && c != ',') {
                main_symb += c;
            }
            else {
                remaining_string = string_to_parse.substr(i);
                break;
            }
        }

        // add symbol in the formula
        FmlaNode fmla_node(main_symb, current_parent, {});

        resulting_fmla.push_back(fmla_node);

        current_fmla_size = resulting_fmla.size();
        if (current_fmla_size > 1){ // We are not in the root node of the formula
            resulting_fmla[current_parent].children.push_back(current_fmla_size - 1);
        }

        if (is_function_symb(main_symb, function_symbs) || is_predicate_symb(main_symb, predicate_symbs)) {
            int arity = -1;
            if (is_function_symb(main_symb, function_symbs)) {
                arity = function_symbs[main_symb]; 
            }
            if (is_predicate_symb(main_symb, predicate_symbs)) {
                arity = predicate_symbs[main_symb]; 
            }

            if (arity > 0) {
                if (remaining_string[0] != '(') {
                    cerr << "Error parsing the formula " << fmla_str << "\n";
                    cerr << "Missing opening parenthesis of " << main_symb << "\n";
                    return resulting_fmla;
                }
                if (remaining_string[remaining_string.size() - 1] != ')') {
                    cerr << "Error parsing the formula " << fmla_str << "\n";
                    cerr << "Missing closing parenthesis of " << main_symb << "\n";
                    return resulting_fmla;
                }

                remaining_string = remaining_string.substr(1, remaining_string.size() - 2);
                
                int parentheses_counter = 0;
                string children_string = "";

                for (int i = 0; i < remaining_string.size(); i++) {
                    char c = remaining_string[i];

                    // Control opening/closing parenthesis
                    if (c == '(') parentheses_counter++;
                    if (c == ')') parentheses_counter--;

                    // Verifications regarding comma
                    if (c != ',') {
                        children_string += c;
                    }
                    else {
                        if (parentheses_counter == 0) {
                            queue.push(make_tuple(current_fmla_size - 1, children_string));
                            children_string = "";
                        }
                        else {
                            children_string += c;
                        }
                    }

                    // End of the string
                    if (i == remaining_string.size() - 1) {
                        queue.push(make_tuple(current_fmla_size - 1, children_string));
                    }
                }
            }
            else {
                // Do nothing
            }
        }
        else {
            // Do nothing
        }

    }

    return resulting_fmla;

}