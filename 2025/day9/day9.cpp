#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <functional>
#include <unordered_map>

/*
    Slide down the firepole in the corner and land in the North Pole movie theater.

    Big tile floor with an interesting pattern.  Elves are redecorating by switching
    out some of the square tiles in the big grid they form.  Some of the tiles are
    red. The elves would like to find the largest rectangle that uses red tiles for
    two of its opposite corners.

    They have a list of where the red tiles are located in the grid (puzzle input).

    Example given.

    You can choose any two red tiles as the opposite corners of the rectange. The
    goal then is to find the largest rectangle possible.

    There may be multiple largest rectangles with the same area, keep in mind.

    Using two red tiles as opposite corners, what is the largest area of any rectangle
    you could make?
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
