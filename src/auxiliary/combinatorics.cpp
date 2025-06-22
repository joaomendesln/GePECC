/**
 * @file combinatorics.cpp
 * @author João Mendes
 * @brief Combinatorial machinery used in the application of the tableau expansion rules and in the search for proof-isomorphic sets of signed formulas
 * 
 * @version 0.0.1
 * @date 2025-06-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "combinatorics.h"

using namespace std;

/**
 * @brief Cheks if all digits in an arrange are different
 * 
 * @param arrange Arrange
 * @return True if all digits of `arrange` are different
 * @return False otherwise 
 */
bool are_arrange_digits_different(vector<int> arrange) {
    set<int> digits_set;
    for (int i = 0; i < arrange.size(); i++) {
        digits_set.insert(arrange[i]);
    }
    if (digits_set.size() == arrange.size()) return true;
    else return false;
}

/**
 * @brief Increments one in an arrange
 * 
 * @param arrange_mask Arrange
 * @param base The succesor of the maximum value a digit of `arrange` can be
 * @return Result of incrementing one in `arrange`
 */
vector<int> increment_arrange(vector<int> arrange, int base){
    for (int i = arrange.size() - 1; i >= 0; i--) {
        if (arrange[i] == base - 1) {
            arrange[i] = 0;
        }
        else {
            arrange[i] = arrange[i] + 1;
            if (are_arrange_digits_different(arrange)) {
                return arrange;
            }
            else {
                i = arrange.size();
            }
        }
    }
    return {};
}

/**
 * @brief Gets all arranges with `premises_size` items from `branch_size`
 * 
 * @param branch_size Size of the branch
 * @param premises_size Amount of premises
 * @return All arranges with `premises_size` items from `branch_size`
 */
vector<vector<int>> get_all_arranges(int branch_size, int premises_size) {
    vector<vector<int>> all_arranges;

    int arranges_amt = 1;
    int size = premises_size;
    int base = branch_size;
    while (size > 0) {
        arranges_amt *= base;
        base--;
        size--;
    }

    vector<int> arrange;

    if (premises_size == 1) {
        for (int i = 0; i < branch_size; i++) {
            arrange = {i};
            all_arranges.push_back(arrange);
        }
    }
    else {
        for (int i = 0; i < premises_size; i++) arrange.push_back(0);
        for (int i = 0; i < arranges_amt; i++){
            arrange = increment_arrange(arrange, branch_size);
            all_arranges.push_back(arrange);
        }
    }

    return all_arranges;
}

/**
 * @brief Increments one in a multibase arrange in which repetition is allowed
 * 
 * @param arrange Arrange
 * @param base Vector in which the n-th element indicates the successor of the maximum value of the n-th digit of `arrange`
 * @return Result of incrementing one in `arrange` 
 */
vector<int> increment_multibase_arrange_repitition(vector<int> arrange, vector<int> base) {
    for (int i = arrange.size() - 1; i >= 0; i--) {
        if (arrange[i] == base[i] - 1) {
            arrange[i] = 0;
        }
        else {
            arrange[i] = arrange[i] + 1;
            return arrange;
        }
    }
    return arrange;
}