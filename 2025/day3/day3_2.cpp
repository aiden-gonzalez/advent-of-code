#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

/*
    Part 2:

    The escalator doesn't move.  It probably needs more joltage to overcome static
    friction.  Now, you need to make the largest joltage by turning on exactly
    twelve batteries within each bank.

    The joltage output is still the number formed by the digits of the batteries
    you've turned on. The only difference is that now there will be 12 digits in
    each bank's joltage output instead of two.

    Ok, we will use the same approach, just generalized to 12 digits.

    It will be helpful to do this in a loop.
*/

int main() {
    std::string line;
    std::ifstream input_file;

    std::vector<long long> joltages;

    // Open input file
    input_file.open("input.txt");

    if (input_file.is_open()) {
        // Read line by line
        while (getline(input_file, line)) {
            // Joltage length
            int n = 12;

            // Current joltage
            long long joltage = 0;

            // Previous joltage digit index
            int prev_ind = -1;

            // Find each digit
            for (int dig = 1; dig <= n; dig++) {
                int max = 0;
                // Find max in the allowed range
                for (int i = prev_ind + 1; i < line.length() - (n - dig); i++) {
                    int val = line[i] - '0';
                    if (val > max) {
                        max = val;
                        prev_ind = i;
                    }
                }
                // Add this component to the joltage
                joltage += (long long)max * std::pow(10, n - dig);
            }
            
            // Add result to list of joltages
            joltages.push_back(joltage);
        }
        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    // Add up all the joltages
    long long total = 0;
    for (int i = 0; i < joltages.size(); i++) {
        total += joltages[i];
    }

    std::cout << "Total joltage: " << total << std::endl;

    return 0;
}
