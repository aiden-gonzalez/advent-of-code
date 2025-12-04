#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

/*
    Part 2: You're sure that's the right password but the door doesn't open.
    Nobody answers the door.  There is another security document that you
    find: "Please use password method 0x434C49434B".

    This means you're actually supposed to count the number of times
    *any click* causes the dial to point to 0, regardless of whether it
    happens during a rotation or at the end of one.

    So, following the same rotations as the example before, the dial points
    at 0 a few extra times.  Ok we can do this.
*/

int main()
{
    // Declare file reading variables
    std::string line;
    std::ifstream input_file;

    // Problem config
    int range_size = 100;

    // Dial position
    int dial_pos = 50;

    // Instructions
    char dir;
    int dist;

    // Password
    int password = 0;

    // Open input file
    input_file.open("input.txt");

    // Read line by line instructions
    if (input_file.is_open()) {
        while (getline(input_file, line)) {
            dir = line[0];
            dist = std::stoi(line.substr(1, 3));

            // Stupid naive approach: go one at a time
            if (dir == 'L') {
                while (dist > 0) {
                    dial_pos = (dial_pos - 1) % range_size;
                    dist -= 1;
                    if (dial_pos == 0) {
                        password += 1;
                    }
                }
            } else if (dir == 'R') {
                while (dist > 0) {
                    dial_pos = (dial_pos + 1) % range_size;
                    dist -= 1;
                    if (dial_pos == 0) {
                        password += 1;
                    }
                }
            }
        }
        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    std::cout << "Password = " << password << std::endl;
    
    return 0;
}
