#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <functional>
#include <unordered_map>

/*
    
*/

int main() {
    std::string line;
    std::ifstream input_file;

    // Open input file
    input_file.open("input.txt");

    if (input_file.is_open()) {
        // Read line by line
        while (getline(input_file, line)) {
            
        }

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    return 0;
}
