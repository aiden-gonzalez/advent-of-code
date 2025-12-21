#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm>

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

    Ok for this alternate method, we will try a totally different approach. We will
    start with the biggest possible button and try to whittle down the joltages
    that way.
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

    bool operator<(const Button& other) const {
        // Sort in descending order
        return indexes.size() > other.indexes.size();
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

template<>
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

std::vector<int> press_buttons(const std::vector<Button> &buttons, const int result_size) {
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

std::vector<int> press_buttons(const std::unordered_set<Button> &buttons, const int result_size) {
    std::vector<Button> buttons_vec;
    for (const auto & button : buttons) {
        buttons_vec.push_back(button);
    }
    return press_buttons(buttons_vec, result_size);
}

std::vector<int> get_complement(std::vector<int> base, const Button &button) {
    for (const int ind : button.indexes) {
        base[ind]--;
    }
    return base;
}

bool result_too_large(const std::vector<int> &result, const std::vector<int> &target) {
    // Complain and return true if they aren't the same size
    if (result.size() != target.size()) {
        std::cout << "Running result_too_large with different size inputs!!";
        return true;
    }

    for (int i = 0; i < result.size(); i++) {
        if (result[i] > target[i]) {
            return true;
        }
    }

    return false;
}

bool complement_too_small(const std::vector<int>& complement) {
    for (const int i : complement) {
        if (i < 0) {
            return true;
        }
    }

    return false;
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

int solve_machine_helper(
    const std::vector<int> &target,
    const std::vector<Button> &buttons,
    std::vector<int> &presses,
    int current_button
) {
    return 0;
}

int solve_machine(const std::vector<int> &target, std::vector<Button> &buttons) {
    // We want to start with the biggest buttons and progress to the smallest
    std::sort(buttons.begin(), buttons.end());

    // Track presses of each button
    std::vector<int> button_presses(buttons.size(), 0);

    // Once that stops working, we back off and try the next biggest button, so on and so forth
    // Recursion will help us backtrack every time we run into a dead end

    // Then, we will try the next biggest button etc.

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
    input_file.open("super_simple_input.txt");

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
        for (int m = 0; m < machines.size(); m++) {
            std::cout << "Solving machine: " << machines[m] << "...\n";
            machines[m].min_presses = solve_machine(machines[m].joltages, machines[m].buttons);
            if (machines[m].min_presses == -1) {
                std::cout << "Warning: Couldn't solve!\n";
            }
            std::cout << '\n';
        }

        // Sum up the solutions
        int sum = 0;
        for (auto & machine : machines) {
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
