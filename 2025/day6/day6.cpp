#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/*
    After helping the elves in the kitchen, you jumped into the garbage chute.

    You find yourself in the garbage smasher, and the door's been magnetically sealed.

    You are approached by a familiy of cephalopods.  They can get the door open,
    but it will take time.  While you wait, they want to see if you can help the
    youngest one with her math homework.

    The worksheet (puzzle input) consists of a list of problems. Each problem has
    a group of numbers that need to be either added (+) or multiplied (*) together.

    However, they are arranged strangely.  They seem to be presented next to each
    ther in a very long horizontal list.

    Each problem's numbers are arranged vertically. And the bottom has the symbol
    of the operation that needs to be performed.  Problems are separated by a full
    column of only spaces.  left/right alignment of numbers within each problem
    can be ignored.

    Once you perform all the problems, the grand total of the answers added together
    will be the answer to the puzzle.
*/

long add(long a, long b) {
    return a + b;
}

long mult(long a, long b) {
    return a * b;
}

int main() {
    std::string line;
    std::ifstream input_file;

    std::vector<long (*) (long, long)> operations;
    std::vector<long> answers;

    // Open input file
    input_file.open("input.txt");

    if (input_file.is_open()) {
        // Read line by line
        while (getline(input_file, line)) {
            // Collect each operation in the final line
            if (line[0] == '*' || line[0] == '+') {
                for (int i = 0; i < line.length(); i++) {
                    if (line[i] == '*') {
                        operations.push_back(mult);
                        answers.push_back(1);
                    } else if (line[i] == '+') {
                        operations.push_back(add);
                        answers.push_back(0);
                    }
                }
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

            // Apply operations cumulatively
            std::string num_string = "";
            int i = 0;
            int a_i = 0;
            while (i < line.length() && a_i < answers.size()) {
                if (line[i] != ' ') {
                    num_string += line[i];
                } else if (num_string.size() > 0) {
                    answers[a_i] = operations[a_i](answers[a_i], std::stoi(num_string));
                    num_string = "";
                    a_i++;
                }
                i++;
            }
            // Last number
            if (num_string.size() > 0 && a_i < answers.size()) {
                answers[a_i] = operations[a_i](answers[a_i], std::stoi(num_string));
            }
        }

        // Add up all answers
        long grand_total = 0;
        for (int i = 0; i < answers.size(); i++) {
            grand_total += answers[i];
        }

        std::cout << "Grand total: " << grand_total << std::endl;

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    return 0;
}
