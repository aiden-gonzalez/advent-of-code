#include <iostream>
#include <fstream>
#include <string>

/*
    
*/

int main() {
    std::string line;
    std::ifstream input_file;

    // Open input file
    input_file.open("example_input.txt");

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
