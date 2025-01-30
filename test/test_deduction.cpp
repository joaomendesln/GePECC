#include "test_deduction.h"
#include <iostream>

using namespace std;

void test_get_initial_tableau() {
    vector<TblNode> tbl = get_initial_tableau();

    print_tableau_as_list(tbl);
}

void test_get_all_arranges() {

    vector<vector<int>> all_arranges = get_all_arranges(4, 3);

    for (int i = 0; i < all_arranges.size(); i++){
        print_vec_int(all_arranges[i]);
        cout << "\n";
    }

}