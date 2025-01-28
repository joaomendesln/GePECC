#include "test_pre_process.h"
#include <iostream>

using namespace std;

void test_process_function_symbs() {

    map <string, int> function_symbs = process_function_symbs();

    for (const auto& pair : function_symbs) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

}

void test_process_predicate_symbs() {

    map <string, int> predicate_symbs = process_predicate_symbs();

    for (const auto& pair : predicate_symbs) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

}