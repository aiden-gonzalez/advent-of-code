#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

/*
    Notes written by Cassandra:

    Part two. The elves want you to remove the paper for real this time 
    instead of just talking about it. Change everything thats removed to
    a X after it happened and @ before it happened. The final output
    will be the number of rolls that are able to be removed. 

    Aiden thinks you should just take all the ones that are accessible and remove them.
    And then when thats done do it again do it again until you can never remove any 
    more rolls. 

    That's it.
*/

int main() {
    std::string line;
    std::ifstream input_file;

    std::vector<std::string> input_grid;

    std::vector<std::vector<int>> adj_count_grid;

    // Open input file
    input_file.open("input.txt");

    if (input_file.is_open()) {
        // Read line by line
        while (getline(input_file, line)) {
            // Add line to input grid
            input_grid.push_back(line);
        }

        // Fill adj_count_grid with zeros
        for (int row = 0; row < input_grid.size(); row++) {
            std::vector<int> row_zeros;
            for (int col = 0; col < input_grid[row].size(); col++) {
                row_zeros.push_back(0);
            }
            adj_count_grid.push_back(row_zeros);
        }

        // Go through input grid once more
        for (int row = 0; row < input_grid.size(); row++) {
            for (int col = 0; col < input_grid[row].size(); col++) {
                // If this is a roll, increment adj roll counts for adjacent positions only (within bounds)
                if (input_grid[row][col] == '@') {
                    for (int i = std::max(row - 1, 0); i <= std::min(row + 1, (int)input_grid.size() - 1); i++) {
                        for (int j = std::max(col - 1, 0); j <= std::min(col + 1, (int)input_grid[row].size() - 1); j++) {
                            if (i != row || j != col) {
                                adj_count_grid[i][j] += 1;
                            }
                        }
                    }
                }
            }
        }

        // Finally, count the number of rolls with less than 4 adjacent rolls of paper
        int rolls_removed = 0;
        bool removed_a_roll;
        do {
            removed_a_roll = false;
            for (int row = 0; row < adj_count_grid.size(); row++) {
                for (int col = 0; col < adj_count_grid[row].size(); col++) {
                    if (input_grid[row][col] == '@' && adj_count_grid[row][col] < 4) {
                        // Remove the roll
                        input_grid[row][col] = '.';
                        rolls_removed += 1;
                        // We will do the loop again
                        removed_a_roll = true;
                        for (int i = std::max(row - 1, 0); i <= std::min(row + 1, (int)input_grid.size() - 1); i++) {
                            for (int j = std::max(col - 1, 0); j <= std::min(col + 1, (int)input_grid[row].size() - 1); j++) {
                                if (i != row || j != col) {
                                    adj_count_grid[i][j] -= 1;
                                }
                            }
                        }
                    }
                }
            }
        } while (removed_a_roll);

        std::cout << "Rolls Removed: " << rolls_removed << std::endl;

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    return 0;
}
