#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

/*
    Now you get inside and take the elevator to the gift shop. A clerk asks
    for your help.  One of the younger elves was playing on a gift shop
    computer and added a whole bunch of invalid product IDs to their gift
    shop database.  Now, they need help identifying the invalid product IDs.

    They have already checked most of the product ID ranges already. You only
    have to help them check a few product ID ranges (your puzzle input).

    For example:
    11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-1698528,
    446443-446449,38593856-38593862,565653-565659,824824821-824824827,
    2121212118-2121212124.

    The ranges are separated by commas.  Each range gives its first ID and
    last ID separated by a dash.

    Since the young elf was just doing silly patterns, you can find the invalid
    IDs by looking for any ID which is made only of some sequence of digits
    repeated twice.  So, 55 (5 twice), 6464 (64 twice), and 123123 (123 twice)
    would all be invalid IDs.

    None of the numbers have leading zeroes. 0101 isn't an ID at all.  101 is a
    valid ID that you would ignore.

    Your job is to find all lf the invalid IDs that appear in the given ranges.

    In the above example:
    1. 11-22 has two invalid IDs, 11 and 22.
    2. 95-115 has one invalid ID, 99.
    3. 998-1012 has one invalid ID, 1010.
    etc.

    What do you get if you add up all the invalid IDs?
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
            for (long i = range_start; i <= range_end; i++) {
                // Get order of magnitude (how many digits)
                int digits = std::floor(log10(i)) + 1;

                // If number is not an even length, skip
                if (digits % 2 != 0) {
                    continue;
                }
                
                // Grab the right side of the number by performing modulus down the middle (ex. 123,123 % 1000 = 123)
                int mod = std::pow(10, digits / 2);
                long right = i % mod;

                // Compose a symmetric version (ex. 123 * 1000 + 123 = 123,123)
                long symmetric = right * mod + right;

                // Check if the symmetric version matches the original, then the original was symmetric
                if (symmetric == i) {
                    invalid_ids_sum += i;
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
