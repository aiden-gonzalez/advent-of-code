#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

/*
    Descend a short staircase, enter a big lobby, and cleared by a security checkpoint.
    When you get to the elevators, each one has a red light above it: all offline!

    Electrical surge fried them.  You need to get further underground though. You could
    take the escalator to the printing department, but you can't get much further
    than that without the elevators.  But the escalators are also offline.

    But it's not fried, it just needs power.  Maybe you can get the escalators working
    while the elf works on the elevators?

    There are batteries nearby for emergency power.  They are each labeled with their
    joltage rating, a value from 1 to 9.  You make a note of their joltage ratings
    (puzzle input).  For example:

    987654321111111
    811111111111119
    234234234234278
    818181911112111

    They are arranged into banks.  Each line of digits in your input corresponds to a 
    bank of batteries.  Within each bank, need to turn on *exactly 2* batteries.

    The joltage that the bank produces is equal to the number formed by the digits on
    the batteries you've turned on. For example, if you have a bank 12345 and you turn
    on batteries 2 and 4, the bank would produce 24 jolts.  Cannot rearrange batteries.

    You need to find the largest possible joltage each bank could produce.  In the
    above example: 98, 89, 78, and 92.

    The total output is the sum of the maximum joltage from each bank.  Find the total
    maximum output joltage you could produce from all the banks.
*/

int main() {
    std::string line;
    std::ifstream input_file;

    std::vector<int> joltages;

    // Open input file
    input_file.open("example_input.txt");

    if (input_file.is_open()) {
        // Read line by line
        while (getline(input_file, line)) {

            // First get the largest number that's not the last one
            int max_first = 0;
            int max_first_ind = 0;
            for (int i = 0; i < line.length() - 1; i++) {
                int val = line[i] - '0';
                if (val > max_first) {
                    max_first = val;
                    max_first_ind = i;
                }
            }

            // Then get the largest number from the rest of the list
            int max_second = 0;
            for (int i = max_first_ind + 1; i < line.length(); i++) {
                int val = line[i] - '0';
                if (val > max_second) {
                    max_second = val;
                }
            }

            // Add result to list of joltages
            joltages.push_back(max_first * 10 + max_second);
        }
        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    // Add up all the joltages
    int total = 0;
    for (int i = 0; i < joltages.size(); i++) {
        total += joltages[i];
    }

    std::cout << "Total joltage: " << total << std::endl;

    return 0;
}
