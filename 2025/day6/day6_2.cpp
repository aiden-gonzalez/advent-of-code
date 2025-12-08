#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/*
    Part 2:

    The cephalopods come back to check on how things are going.  They see the grand
    total doesn't match the one expected by the worksheet.  They realize they forgot
    how to explain how to read cephalopod math.

    Cephalopod math is writting right-to-left in columns.  Each number is given in
    its own column, with the most significant digit at the top and the least
    significant digit at the bottom.

    Things are pivoted from part 1.  Gotta change the way we're parsing the numbers.
*/

long add(long a, long b) {
    return a + b;
}

long mult(long a, long b) {
    return a * b;
}

std::vector<std::string> string_vec_of_length(int len) {
    std::vector<std::string> vec;
    for (int i = 0; i < len; i++) {
        vec.push_back("");
    }
    return vec;
}

int main() {
    std::string line;
    std::ifstream input_file;

    std::vector<long (*) (long, long)> operations;
    std::vector<std::vector<std::string>> nums;
    std::vector<long> answers;

    // Open input file
    input_file.open("input.txt");

    if (input_file.is_open()) {
        // Read line by line
        while (getline(input_file, line)) {
            // Collect each operation in the final line
            if (line[0] == '*' || line[0] == '+') {
                // Keep track of how many numbers are in each problem
                int num_nums = 0;
                for (int i = 0; i < line.length(); i++) {
                    if (line[i] == '*') {
                        operations.push_back(mult);
                        answers.push_back(1);
                        if (num_nums - 1 > 0) {
                            nums.push_back(string_vec_of_length(num_nums - 1));
                        }
                        num_nums = 1;
                    } else if (line[i] == '+') {
                        operations.push_back(add);
                        answers.push_back(0);
                        if (num_nums - 1 > 0) {
                            nums.push_back(string_vec_of_length(num_nums - 1));
                        }
                        num_nums = 1;
                    } else if (line[i] == ' ') {
                        num_nums += 1;
                    }
                }
                // Last num nums
                nums.push_back(string_vec_of_length(num_nums));
            }
        }

        // Reset file pointer
        input_file.clear();
        input_file.seekg(0);

        // Now read line by line again
        while (getline(input_file, line)) {
            // If we reached the bottom line, break
            if (line[0] == '*' || line[0] == '+') {
                break;
            }

            // Read numbers column-by-column
            int line_ind = 0;
            for (int prob = 0; prob < operations.size(); prob++) {
                for (int num = 0; num < nums[prob].size(); num++) {
                    nums[prob][num] += line[line_ind];
                    line_ind++;
                }
                // Skip space
                line_ind++;
            }
        }

        // Reverse operations and answers vectors
        int l = 0;
        int r = operations.size() - 1;
        while (l < r) {
            long (*swap_func) (long, long) = operations[l];
            operations[l] = operations[r];
            operations[r] = swap_func;
            long swap_long = answers[l];
            answers[l] = answers[r];
            answers[r] = swap_long;
            l++;
            r--;
        }

        // Reverse and convert num strings to longs
        std::vector<std::vector<long>> nums_longs;
        for (int prob = operations.size() - 1; prob >= 0; prob--) {
            std::vector<long> prob_longs;
            for (int num = nums[prob].size() - 1; num >= 0; num--) {
                prob_longs.push_back(std::stol(nums[prob][num]));
            }
            nums_longs.push_back(prob_longs);
        }

        // Calculate final answers
        for (int prob = 0; prob < operations.size(); prob++) {
            for (int num = 0; num < nums_longs[prob].size(); num++) {
                answers[prob] = operations[prob](answers[prob], nums_longs[prob][num]);
            }
        }

        // Calculate total of all answers
        long total = 0;
        for (int ans = 0; ans < answers.size(); ans++) {
            total += answers[ans];
        }

        std::cout << "Grand total: " << total << '\n';

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    return 0;
}
