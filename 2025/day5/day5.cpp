#include <iostream>
#include <fstream>
#include <string>

/*
    As the forklifts break through the wall, there is indeed a cafeteria on the other side.
    Commotion coming from the kitchen.  There is an issue with their inventory management
    system.  Because of their complicated new system, they can't figure out which of
    their ingredients are fresh and which are spoiled.  They give you a copy of their
    database (the puzzle input).

    The database operates on ingredient IDs. It consists of a list of fresh ingredient ID
    ranges, a blank line, and a list of available ingredient IDs.

    The fresh ID ranges are inclusive.  They can also overlap.  An ingredient ID is fresh
    if it is in ANY of the ranges.

    They are trying to determine which of the available ingredient IDs are fresh.

    How many of the available ingredient IDs are fresh?
*/

int main() {
    std::string line;
    std::ifstream input_file;

    // Ingredient ID sets
    std::vector<long> start;
    std::vector<long> end;

    int num_fresh_ids = 0;

    // Open input file
    input_file.open("example_input.txt");

    if (input_file.is_open()) {
        // Read line by line
        while (getline(input_file, line)) {
            // At empty line, go to next while loop
            if (line == "") {
                break;
            }

            int dash_pos = line.find('-');
            start.push_back(std::stol(line.substr(0, dash_pos)));
            end.push_back(std::stol(line.substr(dash_pos + 1, line.length() - dash_pos)));

            std::cout << "start: " << start[start.size()-1] << " end: " << start[end.size()-1] << std::endl;
        }

        while (getline(input_file, line)) {
            long id = std::stol(line);
            for (int i = 0; i < start.size(); i++) {
                if (id >= start[i] && id <= end[i]) {
                    num_fresh_ids += 1;
                    break;
                }
            }
        }

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    std::cout << "Number of fresh ids: " << num_fresh_ids << std::endl;

    return 0;
}
