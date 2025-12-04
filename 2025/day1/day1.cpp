#include <iostream>
#include <fstream>
#include <string>

/*
    Elves have discovered project management.  So they know the 
    North Pole decorations have to be finished soon so that other
    critical tasks can start on time.  The bad news is there is
    another emergency: they don't have any time left to decorate
    the north pole.  lol.

    Need to finish decorating the North Pole by December 12th.

    Collect stars by solving puzzles.  Two puzzles will be made available
    each day. The second is unlocked when you complete the first.  Each
    puzzle grants one star.

    Day 1:

    Arrive at the North Pole. However the password has changed, so you
    can't get in. There is a document taped to the wall that explains
    that the password is locked in a safe.  The attached document has
    the new combination.

    The safe has a dial with only an arrow on it. Around the dial are the
    numbers 0 through 99 in order.  As you turn the dial, it makes a small
    click noise as it reaches each number.

    The attached document (puzzle input) contains a series of rotations, one
    per line, which tells you how to open the safe.  Each one starts with
    an L or R which indicates left (towards lower numbers) or right (towards 
    higher numbers). Then, there is a distance value which indicates how
    many clicks to turn the dial in that direction.

    For example, from 11, R8 would cause the dial to be at 19. L19 -> 0.

    Turning the dial left from 0 one click makes it point at 99. Also, turning
    right from 99 one click makes it point at 0.

    From 5, L10 would cause it to point at 95. Then R5 -> 0.

    Dial starts by pointing at 50.

    You could follow the instructions, but you know that the safe is actually
    a decoy. The actual password is the number of times the dial is left pointing
    at 0 after any rotation in the sequence.
*/

int main()
{
    // Declare file reading variables
    std::string line;
    std::ifstream input_file;

    // Dial position
    int dial_pos = 50;

    // Instruction vars
    char dir;
    int dist;

    // Password var
    int password = 0;

    // Open input file
    input_file.open("input.txt");

    // Read line by line instructions
    if (input_file.is_open()) {
        while (getline(input_file, line)) {
            dir = line[0];
            dist = std::stoi(line.substr(1, 3));

            if (dir == 'L') {
                dial_pos = (dial_pos - dist) % 100;
            } else if (dir == 'R') {
                dial_pos = (dial_pos + dist) % 100;
            }

            if (dial_pos == 0) {
                password += 1;
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
