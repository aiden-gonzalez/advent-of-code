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

// BUTTON CODE

class Button {
public:
    Button (std::vector<int> nums) {
        indexes = nums;
    }

    std::vector<int> indexes;

    // Friend function: stream operator overload
    friend std::ostream & operator<<(std::ostream & os, const Button & b);

    bool operator==(const Button& other) const {
        return indexes == other.indexes;
    }
};

std::ostream & operator<<(std::ostream & os, Button const & b) {
    os << "(";
    for (int i = 0; i < b.indexes.size() - 1; i++) {
        os << b.indexes[i] << ",";
    }
    os << b.indexes[b.indexes.size() - 1] << ")";
    return os;
}

template <>
struct std::hash<Button> {
    std::size_t operator()(const Button& b) const noexcept {
        std::size_t result = 0;
        for (int i = 0; i < b.indexes.size(); i++) {
            const std::size_t hash = std::hash<int>()(b.indexes[i]);
            result = result ^ hash << i;
        }
        return result;
    }
};

// MACHINE CODE

class Machine {
    public:
        Machine (const int il, const std::vector<Button> &bs, const std::vector<int> &js) {
            ind_lights = il;
            buttons = bs;
            min_presses = -1;
            joltages = js;
        }

        int ind_lights;
        std::vector<Button> buttons;
        int min_presses;
        std::vector<int> joltages;

        // Friend function: stream operator overload
        friend std::ostream & operator<<(std::ostream & os, const Machine & m);
};

std::ostream & operator<<(std::ostream & os, Machine const & m) {
    os << "Machine: [" << m.ind_lights << "] ";
    for (const auto & button : m.buttons) {
        os << button << ' ';
    }
    os << '{';
    for (int i = 0; i < m.joltages.size() - 1; i++) {
        os << m.joltages[i] << ',';
    }
    os << m.joltages[m.joltages.size() - 1] << "} -> " << m.min_presses;
    return os;
}

// BUTTON PRESSING FUNCTIONS

std::vector<int> press_buttons(const std::unordered_set<Button> &buttons, const int result_size) {
    // Initialize empty result array of given size
    std::vector<int> result(result_size);

    // For each button, increment the referenced indexes
    for (const auto & button : buttons) {
        for (const int ind : button.indexes) {
            result[ind]++;
        }
    }

    return result;
}

std::vector<int> get_complement(std::vector<int> base, const Button &button) {
    for (const int ind : button.indexes) {
        base[ind]--;
    }
    return base;
}

// SUBSET FUNCTIONS

void get_subsets_helper(const std::vector<Button> &buttons, std::vector<std::unordered_set<Button>> &sets, const int k, const int idx, std::unordered_set<Button> subset) {
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

std::vector<std::unordered_set<Button>> get_subsets(const std::vector<Button> &buttons, const int k) {
    std::vector<std::unordered_set<Button>> sets = {};
    get_subsets_helper(buttons, sets, k, 0, {});
    return sets;
}

// SOLVING FUNCTIONS

int solve_one_or_two(const std::vector<int> &target, const std::vector<Button> &buttons, const std::unordered_set<int> &ignore) {
    std::cout << "solve_one_or_two call: Looking to make " << target << " with buttons";
    for (int i = 0; i < buttons.size(); i++) {
        if (ignore.count(i) == 0) {
            std::cout << buttons[i];
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

    // Base case: 1 press works
    for (int b = 0; b < buttons.size(); b++) {
        // If button is not ignored
        if (ignore.count(b) == 0) {
            // If pressing the button produces the target result
            if (press_buttons({buttons[b]}, target.size()) == target) {
                std::cout << "Found a 1 button solution: " << buttons[b] << "\n";
                return 1;
            }
        }
    }

    // Find potential 2 press solution
    for (int b = 0; b < buttons.size(); b++) {
        if (ignore.count(b) == 1) {
            continue;
        }
        std::vector<int> complement = get_complement(target, buttons[b]);
        for (int bc = b; bc < buttons.size(); bc++) {
            if (press_buttons({buttons[bc]}, complement.size()) == complement) {
                std::cout << "Found two press solution: " << buttons[b] << " and " << buttons[bc] << '\n';
                return 2;
            }
        }
    }

    return -1;
}

int solve_machine(const std::vector<int>& target, const std::vector<Button> &buttons) {
    std::cout << "solve_machine call: Looking to make " << target << " with buttons ";
    for (const auto & button : buttons) {
        std::cout << button;
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

        std::vector<int> complement = get_complement(target, buttons[b]);
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
        std::vector<std::unordered_set<Button>> subsets = get_subsets(buttons, k);

        // Check each subset for a solution
        for (int s = 0; s < subsets.size(); s++) {
            // Press all the buttons in the subset
            std::vector<int> result = press_buttons(subsets[s], target.size());

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

// DRIVER CODE

int main() {
    std::string line;
    std::ifstream input_file;

    std::vector<Machine> machines;
    std::vector<int> presses;

    // Open input file
    input_file.open("example_input.txt");

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
            std::vector<Button> buttons;
            int button_start = line.find('(');
            std::vector<int> button_wires;
            while (button_start != std::string::npos) {
                // Read wires for button
                int wire_start = button_start + 1;
                int comma_ind = line.find(',', wire_start + 1);
                int button_end = line.find(')', wire_start + 1);
                while (comma_ind < button_end && comma_ind != std::string::npos) {
                    int wire_num = std::stoi(line.substr(wire_start, comma_ind - wire_start));
                    button_wires.push_back(wire_num);
                    wire_start = comma_ind + 1;
                    comma_ind = line.find(',', wire_start + 1);
                }
                // Read last wire
                int last_wire_num = std::stoi(line.substr(wire_start, button_end - wire_start));
                button_wires.push_back(last_wire_num);

                // Add to machine buttons
                buttons.push_back(Button(button_wires));
                
                // Seek to next button
                button_start = line.find('(', button_end + 1);
                button_wires = {};
            }

            // Read joltages
            std::vector<int> joltages;
            int joltages_start = line.find('{');
            int joltages_end = line.find('}');
            int jolt_start = joltages_start + 1;
            int comma_ind = line.find(',', jolt_start);
            while (comma_ind < joltages_end  && comma_ind != std::string::npos) {
                int jolt_num = std::stoi(line.substr(jolt_start, comma_ind - jolt_start));
                joltages.push_back(jolt_num);
                jolt_start = comma_ind + 1;
                comma_ind = line.find(',', jolt_start + 1);
            }
            // Read last joltage
            int last_jolt_num = std::stoi(line.substr(jolt_start, joltages_end - jolt_start));
            joltages.push_back(last_jolt_num);

            machines.push_back(Machine(indicator, buttons, joltages));
        }

        // Find the solution for each machine
        // for (int m = 0; m < machines.size(); m++) {
        //     std::cout << "Solving machine " << m + 1 << "...\n";
        //     machines[m].min_presses = solve_machine(machines[m].joltages, machines[m].buttons);
        //     if (machines[m].min_presses == -1) {
        //         std::cout << "Warning: Couldn't solve!\n";
        //     }
        //     std::cout << '\n';
        // }

        // Sum up the solutions
        int sum = 0;
        for (auto & machine : machines) {
            std::cout << machine << '\n';
            sum += machine.min_presses;
        }
        std::cout << "Total presses: " << sum << '\n';

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }
    
    return 0;
}
