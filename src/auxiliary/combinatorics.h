#ifndef COMBINATORICS_H
#define COMBINATORICS_H

#include <vector>
#include <set>
#include <cmath>

using namespace std;

bool are_arrange_digits_different(vector<int> arrange);

vector<int> increment_arrange(vector<int> arrange, int base);

vector<vector<int>> get_all_arranges(int branch_size, int premises_size);

vector<int> increment_multibase_arrange_repitition(vector<int> arrange, vector<int> base);

#endif // COMBINATORICS_H