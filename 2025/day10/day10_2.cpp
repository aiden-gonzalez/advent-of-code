#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

/*
    Part 2:

    The machines are starting to come online. But joltage requirements matter now.

    Each machine needs to be configured to the exact specified joltage levels to
    function properly.  Below the buttons on each machine is a big lever that you
    can use to switch the buttons from configuring indicator lights to increasing
    joltage levels (ignore the indicator light diagrams).

    Each machine has a set of numeric counters tracking its joltage levels, one
    counter per joltage requirement.  They are initially set to zero.

    {3, 5, 4, 7} means the machine has 4 counters which are intially set to 0.
    Goal is to simultaneously configure the first counter to be 3, second to be
    5, third to be 4, and fourth to be 7.

    Button wiring is still relevant. Each button now indicates which counter
    it affects.  0 means the first counter, 1 the second, and so on.  When you
    push a button, each listed timer is increased by 1.

    So a button wiring schematic like (1, 3) means each time you push that button,
    the second and fourth counters would increase by 1. If the current joltage levels
    were {0, 1, 2, 3}, pushing the button would change them to be {0, 2, 2, 4}.

    You can push each button as many times as you like. however, your finger is getting
    sore. You will need to determine the fewest possible presses required to correctly
    configure each machines joltage level counters.

    Example input again.
*/

class Machine {
    public:
        Machine (const int il, const std::vector<int> &bs) {
            ind_lights = il;
            buttons = bs;
            min_presses = -1;
            joltage =
        }

        int ind_lights;
        std::vector<int> buttons;
        int min_presses;

        // Friend function: stream operator overload
        friend std::ostream & operator<<(std::ostream & os, const Machine & m);
};

std::ostream & operator<<(std::ostream & os, Machine const & m) {
    os << "Machine: [" << m.ind_lights << "]";
    for (int i = 0; i < m.buttons.size(); i++) {
        std::cout << " (" << m.buttons[i] << ")";
    }
    std::cout << " -> " << m.min_presses;
    return os;
}

int solve_one_or_two(const int target, const std::vector<int> &buttons, const std::unordered_set<int> &ignore) {
    std::cout << "solve_one_or_two call: Looking to make " << target << " with buttons";
    for (int i = 0; i < buttons.size(); i++) {
        if (ignore.count(i) == 0) {
            std::cout << " " << buttons[i];
        }
    }
    std::cout << " (ignoring indexes";
    for (const auto& i: ignore) {
        std::cout << " " << i;
    }
    std::cout << ")\n";

    // Base case: all numbers are ignored
    if (buttons.size() == ignore.size()) {
        std::cout << "All numbers are ignored, returning 0.\n";
        return 0;
    }

    // Base case: 1 more press works
    for (int b = 0; b < buttons.size(); b++) {
        if (ignore.count(b) == 0 && buttons[b] == target) {
            std::cout << "Found a 1 button solution: " << buttons[b] << '\n';
            return 1;
        }
    }

    // Find potential 2 press solution
    for (int b = 0; b < buttons.size(); b++) {
        if (ignore.count(b) == 1) {
            continue;
        }
        int complement = target ^ buttons[b];
        for (int bc = b; bc < buttons.size(); bc++) {
            if (buttons[bc] == complement) {
                std::cout << "Found two press solution: " << buttons[b] << " and " << buttons[bc] << '\n';
                return 2;
            }
        }
    }

    return -1;
}

void get_subsets_helper(const std::vector<int> &buttons, std::vector<std::unordered_set<int>> &sets, const int k, const int idx, std::unordered_set<int> subset) {
    if (subset.size() == k) {
        sets.push_back(subset);
        return;
    }
    for (int i = idx; i < buttons.size(); i++) {
        subset.insert(buttons[i]);
        get_subsets_helper(buttons, sets, k, i + 1, subset);
        subset.erase(buttons[i]);
    }
}

std::vector<std::unordered_set<int>> get_subsets(const std::vector<int> &buttons, const int k) {
    std::vector<std::unordered_set<int>> sets = {};
    get_subsets_helper(buttons, sets, k, 0, {});
    return sets;
}

int solve_machine(const int target, const std::vector<int> &buttons) {
    std::cout << "solve_machine call: Looking to make " << target << " with buttons";
    for (int i = 0; i < buttons.size(); i++) {
        std::cout << " " << buttons[i];
    }
    std::cout << "\n";

    // Initialize empty ignore array for later use
    std::unordered_set<int> ignore = {};

    // See if a simple one or two button solution would work
    const int one_or_two_sol = solve_one_or_two(target, buttons, ignore);
    if (one_or_two_sol > -1) {
        return one_or_two_sol;
    }

    // Three sum
    for (int b = 0; b < buttons.size(); b++) {
        if (ignore.count(b) == 1) {
            continue;
        }

        const int complement = target ^ buttons[b];
        ignore.insert(b);
        const int result = solve_one_or_two(complement, buttons, ignore);
        ignore.erase(b);

        // If we found a solution
        if (result > -1) {
            std::cout << "Returning 1 + " << result << '\n';
            return 1 + result;
        }
    }

    // Now try combinations of four or greater
    for (int k = 4; k <= buttons.size(); k++) {
        // Get all subsets of size k
        std::vector<std::unordered_set<int>> subsets = get_subsets(buttons, k);

        // Check each subset for a solution
        for (int s = 0; s < subsets.size(); s++) {
            // Press all the buttons in the subset
            int result = 0;
            for (const auto& num : subsets[s]) {
                result = result ^ num;
            }

            // If the result equals the target, return the subset size
            if (result == target) {
                return k;
            }
        }
    }

    // Didn't find a solution
    std::cout << "Didn't find a solution, returning -1.\n";
    return -1;
}

int main() {
    std::string line;
    std::ifstream input_file;

    std::vector<Machine> machines;
    std::vector<int> presses;

    // Open input file
    input_file.open("input.txt");

    if (input_file.is_open()) {
        // Read line by line
        while (getline(input_file, line)) {
            // Read indicator lights
            std::string lights = line.substr(1, line.find(']') - 1);
            int indicator = 0;
            for (int i = 0; i < lights.size(); i++) {
                if (lights[i] == '#') {
                    indicator = indicator | 1 << (lights.size() - i - 1);
                }
            }

            // Read buttons
            std::vector<int> buttons;
            int button_start = line.find('(');
            int button_wires = 0;
            while (button_start != std::string::npos) {
                // Read wires for button
                int wire_start = button_start + 1;
                int comma_ind = line.find(',', wire_start + 1);
                int button_end = line.find(')', wire_start + 1);
                while (comma_ind < button_end && comma_ind != std::string::npos) {
                    int wire_num = std::stoi(line.substr(wire_start, comma_ind - wire_start));
                    button_wires = button_wires | 1 << (lights.size() - wire_num - 1);
                    wire_start = comma_ind + 1;
                    comma_ind = line.find(',', wire_start + 1);
                }
                // Read last wire
                int last_wire_num = std::stoi(line.substr(wire_start, button_end - wire_start));
                button_wires = button_wires | 1 << (lights.size() - last_wire_num - 1);

                // Add to machine buttons
                buttons.push_back(button_wires);
                
                // Seek to next button
                button_start = line.find('(', button_end + 1);
                button_wires = 0;
            }
            machines.push_back(Machine(indicator, buttons));
        }

        // Find the solution for each machine
        for (int m = 0; m < machines.size(); m++) {
            std::cout << "Solving machine " << m + 1 << "...\n";
            machines[m].min_presses = solve_machine(machines[m].ind_lights, machines[m].buttons);
            if (machines[m].min_presses == -1) {
                std::cout << "Warning: Couldn't solve!\n";
            }
            std::cout << '\n';
        }

        // Sum up the solutions
        int sum = 0;
        for (int m = 0; m < machines.size(); m++) {
            std::cout << machines[m] << '\n';
            sum += machines[m].min_presses;
        }
        std::cout << "Total presses: " << sum << '\n';

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    std::cout << '\n';
    
    return 0;
}
