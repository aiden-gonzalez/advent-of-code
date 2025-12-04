#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

/*
    Part 2:

    The clerk discovers that there are still invalid IDs in the ranges in
    your list.  Maybe there are other bad patterns.

    Now, an ID is invalid if it is made only of some sequence of digits repeated
    *at least* twice. So, 12341234 (1234 two times), 123123123 (123 three times),
    1212121212 (12 five times), and 1111111 (1 seven times) are all invalid IDs.

    So now do the problem again with these new rules.
*/

int main() {
    std::string line;
    std::ifstream input_file;

    long invalid_ids_sum = 0;

    // Open input file
    input_file.open("input.txt");

    if (input_file.is_open()) {
        // Read input line, separated by delimiter
        while (getline(input_file, line, ',')) {
            // Parse number range
            int dash_pos = line.find('-');

            long range_start = std::stol(line.substr(0, dash_pos));
            long range_end = std::stol(line.substr(dash_pos + 1, line.length()));
            
            // Iterate through the range
            for (long id = range_start; id <= range_end; id++) {
                // Resort to string-based technique
                std::string sid = std::to_string(id);

                // Test all chunk sizes
                for (int cs = sid.length() / 2; cs >= 1; cs--) {
                    // If string is not evenly divisible by this chunk size, skip
                    if (sid.length() % cs != 0) {
                        continue;
                    }

                    // Split string into chunks of this size
                    std::vector<std::string> chunks;
                    for (int c = 0; c < sid.length() / cs; c++) {
                        chunks.push_back(sid.substr(c * cs, cs));
                    }

                    // Check that all chunks are the same
                    bool chunks_match = true;
                    for (int c = 0; c < chunks.size() - 1; c++) {
                        // If one chunk doesn't match the next, give up
                        if (chunks[c] != chunks[c + 1]) {
                            chunks_match = false;
                            break;
                        }
                    }

                    // If all the chunks match, this is an invalid id
                    if (chunks_match) {
                        invalid_ids_sum += id;
                        // Skip to next id
                        break;
                    }
                }
            }
        }
        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    std::cout << "Invalid IDs sum: " << invalid_ids_sum << std::endl;

    return 0;
}
