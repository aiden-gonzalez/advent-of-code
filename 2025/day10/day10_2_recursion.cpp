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
        presses = 0;
    }

    std::vector<int> indexes;
    int presses;

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

void unpress_button(std::vector<int>& result, Button& button, int num_presses = 1) {
    // Decrement the referenced indexes
    for (const int ind : button.indexes) {
        result[ind] -= num_presses;
    }
    // Decrement number of presses
    button.presses -= num_presses;
}

void reset_button(std::vector<int>& result, Button& button) {
    // Fully unpress all of the referenced indexes
    for (const int ind : button.indexes) {
        result[ind] -= button.presses;
    }
    button.presses = 0;
}

void press_button(std::vector<int>& result, Button& button, int num_presses = 1) {
    // Increment the referenced indexes
    for (const int ind : button.indexes) {
        result[ind] += num_presses;
    }
    // Increment number of presses
    button.presses += num_presses;
}

int press_max_times(std::vector<int>& result, const std::vector<int>& target, Button& button) {
    // Find minimum difference between result and target across this button's indexes
    int min_diff = 1000000;
    for (const int but_ind : button.indexes) {
        int diff = target[but_ind] - result[but_ind];
        if (diff < min_diff) {
            min_diff = diff;
        }
    }

    // If min diff is negative, we are already past the max times we can press this button
    if (min_diff < 0) {
        return 0;
    }

    // Press button min_diff times
    press_button(result, button, min_diff);

    return min_diff;
}

// RESULT CHECKING FUNCTIONS

bool result_is_empty(const std::vector<int>& result) {
    bool empty = true;
    for (int i = 0; i < result.size(); i++) {
        if (result[i] != 0) {
            empty = false;
            break;
        }
    }
    return empty;
}

std::unordered_set<int> too_large_indexes(const std::vector<int> &result, const std::vector<int> &target) {
    std::unordered_set<int> too_large;
    // Complain and return true if they aren't the same size
    if (result.size() != target.size()) {
        std::cout << "Running too_large_indexes with different size inputs!!";
        return too_large;
    }
    for (int i = 0; i < result.size(); i++) {
        if (result[i] > target[i]) {
            too_large.insert(i);
        }
    }

    return too_large;
}

bool result_too_small(const std::vector<int>& result) {
    for (const int i : result) {
        if (i < 0) {
            return true;
        }
    }

    return false;
}

void print_current_result(const std::vector<int>& current_result) {
    std::cout << "current_result: {";
    for (int i = 0; i < current_result.size() - 1; i++) {
        std::cout << current_result[i] << ',';
    }
    std::cout << current_result[current_result.size() - 1] << "}\n";
}

int sum_up_buttons(const std::vector<Button>& buttons) {
    int sum = 0;
    for (int b = 0; b < buttons.size(); b++) {
        sum += buttons[b].presses;
    }
    return sum;
}

// SOLVING FUNCTIONS

void press_unique_buttons(const std::vector<int> &target, std::vector<Button> &buttons, std::vector<int> &current_result, std::unordered_set<int> &ignore) {
    bool unique_button_found;
    do {
        unique_button_found = false;
        for (int b = 0; b < buttons.size(); b++) {
            // If button is already ignored, don't investigate it for unique indexes
            if (ignore.count(b) == 1) {
                continue;
            }

            // Get all indexes from other buttons. Ugly but works.
            std::unordered_set<int> all_other_button_inds;
            for (int o = 0; o < buttons.size(); o++) {
                // Skip the current button or ignored buttons
                if (o == b || ignore.count(o) == 1) {
                    continue;
                }

                for (int i : buttons[o].indexes) {
                    all_other_button_inds.insert(i);
                }
            }

            // Check to see if the current button has any unique indexes
            for (int but_ind : buttons[b].indexes) {
                // If index is unique, press that button the required number of times to reach its specified joltage
                if (all_other_button_inds.count(but_ind) == 0) {
                    unique_button_found = true;
                    int num_presses = press_max_times(current_result, target, buttons[b]);
                    std::cout << "Button " << buttons[b] << " has unique index, " << but_ind << ", pressed " << num_presses << " times.\n";
                    ignore.insert(b);
                    // Don't need to press it any more or less times, break
                    break;
                }
            }
        }
    // Repeat unique button finding process until no more unique buttons are found
    } while (unique_button_found);    
}

void solve_machine_helper(const std::vector<int>& target, std::vector<Button>& buttons, const std::unordered_set<int> &ignore, std::vector<int> &current_result, int current_button, int &min_presses) {
    // If this problem is solved, set min_presses if necessary
    if (current_result == target) {
        min_presses = std::min(min_presses, sum_up_buttons(buttons));
        std::cout << "Solution found, min_presses is now " << min_presses << ".\n";
    }

    // If current button is out of range, return
    if (current_button == buttons.size()) {
        return;
    }

    // If this button is ignored, skip it
    if (ignore.count(current_button) == 1) {
        std::cout << "Current button " << current_button << ' ' << buttons[current_button] << " is ignored, skipping it...\n";
        return solve_machine_helper(target, buttons, ignore, current_result, current_button + 1, min_presses);
    }

    // Do button pressing recursion
    while (too_large_indexes(current_result, target).size() == 0) {
        std::cout << "Pressing button " << current_button << " " << buttons[current_button] << "...\n";
        press_button(current_result, buttons[current_button]);
        print_current_result(current_result);
        solve_machine_helper(target, buttons, ignore, current_result, current_button + 1, min_presses);
        // Reset all later buttons for the next press iteration
        for (int r = current_button + 1; r < buttons.size(); r++) {
            reset_button(current_result, buttons[r]);
        }
    }
}

int solve_machine(const std::vector<int> &target, std::vector<Button> &buttons) {
    // We want to start with the biggest buttons and progress to the smallest
    std::sort(buttons.begin(), buttons.end());

    // Keep track of the current result (starts with 0s) and buttons to ignore (used later)
    std::vector<int> current_result(target.size(), 0);
    std::unordered_set<int> ignore;

    // Find buttons with unique indexes and press them first
    press_unique_buttons(target, buttons, current_result, ignore);
    if (!result_is_empty(current_result)) {
        std::cout << "After pressing unique buttons, ";
        print_current_result(current_result);
    }
    
    // Start recursion on first button
    int min_presses = 1000000;
    solve_machine_helper(target, buttons, ignore, current_result, 0, min_presses);

    // Didn't find a solution
    if (min_presses == 1000000) {
        std::cout << "Didn't find a solution, returning -1.\n";
        return -1;
    }

    // Otherwise return min presses
    return min_presses;
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
        for (int m = 0; m < machines.size(); m++) {
            std::cout << "Solving machine: " << machines[m] << "...\n";
            machines[m].min_presses = solve_machine(machines[m].joltages, machines[m].buttons);
            if (machines[m].min_presses == -1) {
                std::cout << "Warning: Couldn't solve! Failed on machine " << m + 1 << ".\n";
                return 1;
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
