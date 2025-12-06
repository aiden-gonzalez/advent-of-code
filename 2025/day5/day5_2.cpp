#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

/*
    Part 2:

    The elves start bringing their spoiled inventory to the trash cute.

    They would like to know ALL of the IDs that the fresh ingredient ID ranges consider
    to be fresh. An ingredient ID is still considered to be fresh if it is in any range.

    Now, the second section of the database (available ingredient IDs) is irrelevant.

    How many ingredient IDs are considered to be fresh?
*/

class Range {
    public:
        // Constructor
        Range(long s, long e) {
            start = s;
            end = e;
        }

        long start;
        long end;

        // friend function: stream operator overload
        friend std::ostream & operator<<(std::ostream & os, const Range & r);
};

// Override stream operator
std::ostream & operator<<(std::ostream & os, Range const & r) {
    os << "Range: " << r.start << "-" << r.end;
    return os;
}

int main() {
    std::string line;
    std::ifstream input_file;

    // Ingredient ID sets
    std::vector<Range> id_ranges;

    // Open input file
    input_file.open("input.txt");

    if (input_file.is_open()) {
        // Read line by line
        while (getline(input_file, line)) {
            // At empty line, go to next while loop
            if (line == "") {
                break;
            }

            int dash_pos = line.find('-');
            id_ranges.push_back(
                Range(
                    std::stol(line.substr(0, dash_pos)),
                    std::stol(line.substr(dash_pos + 1, line.length() - dash_pos))
                )
            );
        }

        // Sort id ranges by the starting number
        struct SortByStart {
            bool operator() (Range const & L, Range const & R) const { return L.start < R.start; }
        };
        std::sort(id_ranges.begin(), id_ranges.end(), SortByStart());

        // Now we will combine ranges to allow for simple count of IDs
        int i = 1;
        while (i < id_ranges.size()) {
            // If there is range overlap
            if (id_ranges[i].start <= id_ranges[i - 1].end) {
                // Extend the previous range if necessary
                id_ranges[i - 1].end = std::max(id_ranges[i - 1].end, id_ranges[i].end);

                // Absorb (remove) current range
                id_ranges.erase(id_ranges.begin() + i);
            } else {
                i++;
            }
        }

        for (int i = 0; i < id_ranges.size(); i++) {
            std::cout << id_ranges[i] << std::endl;
        }

        // Now we have a list of NON-overlapping ranges
        long fresh_id_count = 0;
        for (int i = 0; i < id_ranges.size(); i++) {
            fresh_id_count += id_ranges[i].end - id_ranges[i].start + 1;
        }

        std::cout << "Number of fresh ids: " << fresh_id_count << std::endl;

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    return 0;
}
