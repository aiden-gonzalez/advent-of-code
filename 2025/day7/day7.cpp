#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>

/*
    You exit the trash compactor and find yourself in a North Pole research wing.

    They seem to be researching teleportation. You step onto the large yellow
    teleporter pad.

    Suddenly, you find yourself in an unfamiliar room.  It has no doors, and the only
    way out is the teleporter.  Unfortunately, the teleporter seems to be leaking
    magic smoke.

    There are lots of spare parts, manuals, and diagnostic equipment lying around.

    After connecting one of the diagnostic tools, it helpfully displays error code
    "0H-N0" which apparently means that there's an issue with one of the tachyon
    manifolds.

    You quickly locate a diagram of the tachyon manifold (puzzle input). A tachyon
    beam enters the manifold at the location marked "S".  They always move downward.
    Tachyon beams pass freely through empty space ".".  However, if they encounter
    a splitter "^", the beam is stopped.  At that point, a new tachyon beam continues
    from the immediate left and right of the splitter.

    Example given.

    To repair the teleporter, you must know how many times beams are split.  How
    many times will the beam be split?

    After thinking about it for a while, I think I will try to just do exactly
    what the example problem describes.

    Ended up using an unordered set (hash set). Was simple and worked well.
*/

int main() {
    std::string line;
    std::ifstream input_file;

    int num_splits = 0;
    std::unordered_set<int> beam_positions;

    // Open input file
    input_file.open("input.txt");

    if (input_file.is_open()) {
        // Read line by line
        while (getline(input_file, line)) {
            // Scan the line
            for (int i = 0; i < line.length(); i++) {
                // Beam start point
                if (line[i] == 'S') {
                    beam_positions.insert(i);
                // Beam split point
                } else if (line[i] == '^' && beam_positions.count(i) == 1) {
                    num_splits++;
                    beam_positions.erase(i);
                    beam_positions.insert(i - 1);
                    beam_positions.insert(i + 1);
                }
            }
        }

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    std::cout << "Number of splits: " << num_splits << std::endl;

    return 0;
}
