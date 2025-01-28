#include "pre_process.h"

using namespace std;

map<string, int> process_function_symbs() {

    map <string, int> resulting_function_symbs;
    string filename = "../src/symbols";

    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << "\n";
        return resulting_function_symbs;
    }

    vector<string> lines;
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }

    file.close();

    if (lines.size() == 0) {
        cout << "Empty file\n";
        return resulting_function_symbs;
    }

    if (lines[0] != "function") {
        cout << "Wrong file fomatting\n";
        return resulting_function_symbs;
    }

    for (int i = 1; i < lines.size(); i++) {
        if (lines[i] == "predicate") break;

        // get colon idx
        int colon_idx = 0;
        for (char c : lines[i]) {
            if (c == ':') break;
            colon_idx += 1;
        }

        // get arity of the function from this line
        string arity_str = lines[i].substr(0, colon_idx + 1);
        int arity_int = stoi(arity_str);

        // split the rest of the line by commas
        string func_symbs_str = lines[i].substr(colon_idx + 1);
        stringstream stream_func(func_symbs_str);
        string func_symb;
        vector<string> func_symbs_with_space;

        while (getline(stream_func, func_symb, ',')) {
            func_symbs_with_space.push_back(func_symb);
        }

        // remove white spaces and adding function symbol to the map
        for (int j = 0; j < func_symbs_with_space.size(); j++) {
            func_symb = "";
            for (char c : func_symbs_with_space[j]) {
                if (!isspace(c)) {
                    func_symb += c;
                }
            }
            resulting_function_symbs[func_symb] = arity_int;
        }
    }
    return resulting_function_symbs;

}

map<string, int> process_predicate_symbs() {
    map <string, int> resulting_predicate_symbs;
    string filename = "../src/symbols";

    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << "\n";
        return resulting_predicate_symbs;
    }

    vector<string> lines;
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }

    file.close();

    if (lines.size() == 0) {
        cout << "Empty file\n";
        return resulting_predicate_symbs;
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

        // get colon idx
        int colon_idx = 0;
        for (char c : lines[i]) {
            if (c == ':') break;
            colon_idx += 1;
        }

        // get arity of the predicate from this line
        string arity_str = lines[i].substr(0, colon_idx + 1);
        int arity_int = stoi(arity_str);

        // split the rest of the line by commas
        string pred_symbs_str = lines[i].substr(colon_idx + 1);
        stringstream stream_func(pred_symbs_str);
        string pred_symb;
        vector<string> pred_symbs_with_space;

        while (getline(stream_func, pred_symb, ',')) {
            pred_symbs_with_space.push_back(pred_symb);
        }

        // remove white spaces and adding function symbol to the map
        for (int j = 0; j < pred_symbs_with_space.size(); j++) {
            pred_symb = "";
            for (char c : pred_symbs_with_space[j]) {
                if (!isspace(c)) {
                    pred_symb += c;
                }
            }
            resulting_predicate_symbs[pred_symb] = arity_int;
        }
    }
    return resulting_predicate_symbs;

}