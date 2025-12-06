#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

/*
    Ride the escalator the printing department. They are getting ready for Christmas.
    Large rolls of paper, and a massive printer in the corner.

    They can make their own decorations. What you need is a way to get further
    into the north pole base while the elevators are offline.

    They are pretty sure there is a cafeteria on the other side of the back wall.
    If you can break through the wall, you could keep moving.  But their forklifts
    are too busy moving rolls of paper around.

    If you can optimize the forklift work, they might have enough spare time to break
    through the wall.

    The rolls of paper (@) are arranged on a large grid. The elves even have a helpful
    diagram (puzzle input) indicating where everything is located. Example given.

    Forklifts can only access a roll of paper if there are fewer than four rolls of
    paper in the eight adjacent positions. If you can figure out which rolls of paper
    the forklifts can access, they'll spend less time looking and more time breaking
    down the wall to the cafeteria.

    Consider the complete diagram of paper roll locations. How many rolls of paper can
    be accessed by a forklift?

    So obviously there is a brute force solution here. That's one way to go.

    However, I want to try to do something more clever.  Is there a data structure I can
    build / use that will make this simpler or faster?

    Yes. I could create a 2d array of the counts of surrounding rolls.  Then I just need
    to iterate through the grid once.  For each roll you find, increment the nearby roll
    counts of all the adjacent spots in the 2d array.

    Then, iterate through your 2d array and count how many have a value of less than four.

    Simple as.
*/

int main() {
    std::string line;
    std::ifstream input_file;

    std::vector<std::string> input_grid;

    std::vector<std::vector<int>> adj_count_grid;

    // Open input file
    input_file.open("example_input.txt");

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
        int accessible_rolls = 0;
        for (int row = 0; row < adj_count_grid.size(); row++) {
            for (int col = 0; col < adj_count_grid[row].size(); col++) {
                if (input_grid[row][col] == '@' && adj_count_grid[row][col] < 4) {
                    std::cout << 'x';
                    accessible_rolls += 1;
                } else {
                    std::cout << input_grid[row][col];
                }
            }
            std::cout << '\n';
        }

        std::cout << "Accessible rolls: " << accessible_rolls << std::endl;

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    return 0;
}
