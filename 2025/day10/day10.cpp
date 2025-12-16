#include <iostream>
#include <fstream>
#include <string>

/*
    Across the hall is a large factory. The elves have plenty of time to decorate.
    All the machines are offline, and they can't figure out the initialization
    procedure.

    There is a manual, but the section detailing initialization was eaten by a Shiba
    Inu.  All that remains are some indicator light diagrams, button wiring schematics,
    and joltage requirements for each machine.

    Example given.

    Describes one machine per line.  Each line contains a single indicator light
    diagram in [square brackets], one or more button wiring schematics in
    (parentheses), and joltage requirements in {curly braces}.

    To start a machine, indicator lights must match those shown in the diagram.
    "." is off and "#" is on.  The machine has the number of lights shown, but
    they are all initially off.  [.##.] means the machine has 4 lights that are
    initially off and the goal is to simultaneously configure the first light to be
    off, second to be on, third to be on, and fourth to be off.

    You can toggle the state of indicator lights by pushing any of the listed buttons.
    Each button lists what light it toggles. 0 is the first light, 1 is the second,
    etc.  When you push a button, each light gets toggled.  You have to push each
    button an integer number of times.  No 0.5, no negative presses.

    Because none of the machines are running, the joltage requirements can be ignored.

    You can push a button as many times as you like. But to save on time, you need to
    determine the fewest total presses required to correctly configure all indicator
    lights for all machines in your list.

    First machine example discussed. And second and third.

    What is the fewest button presses required to correctly configure the indicator
    lights on all of the machines (sum up all machines)?

    [....] -> [.#.#] -> [.##.].  No need to hit (0, 1) twice. In fact, there is no need
    to press anything more than once. Pressing anything twice is the same as not
    pressing it at all.  Three times is the same as pressing it once, etc. No repeats.

    So, you could try every combination of one button. Then two. Then three. Etc until
    you get a combo that works. Not exactly sure how I'll set up the loop to be able
    to do that, but I think I can do it.
    - Actually, here's a more clever, "two sum" sort of way to do it.  If it's one,
      then the lights will directly match one of the buttons.  light = button.
    - Then if it's two presses, in a two sum sense you could see if for each button,
      the target minus that button equals another button
    - For three sum, you'll need to pick a button then do two sum with the rest of
      the possible buttons and the target end configuration. And then do that for
      each button.
    - For four sum, you'll need to pick every combo of two buttons possible and
      then do two sum for the remaining.
    - For five sum, you'll need to pick every combo of three possible and then do
      two sum with the remaining.
    So on and so forth. Simple in theory, but to actually implement a generalized
    loop of this might be tricky. But not impossible.

    If button_1 ^ button_2 = ind_lights.  For example 10 ^ 12 = 6.  Then,
    10 = 6 ^ 12. And therein lies your two-sum.
*/

class Machine {
    public:
        Machine (int il, std::vector<int> bs) {
            ind_lights = il;
            buttons = bs;
            min_presses = 0;
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
            // Zero presses isn't part of the input, but theoretically possible
            if (machines[m].ind_lights == 0) {
                machines[m].min_presses = 0;
                continue;
            }

            // Check for one press solution
            for (int b = 0; b < machines[m].buttons.size(); b++) {
                if (machines[m].buttons[b] == machines[m].ind_lights) {
                    machines[m].min_presses = 1;
                    break;
                }
            }
            if (machines[m].min_presses == 1) {
                continue;
            }

            // For two presses and up: two-sum style solution
            for (int b = 0; b < machines[m].buttons.size(); b++) {
                int complement = machines[m].ind_lights ^ machines[m].buttons[b];
                for (int bc = b + 1; bc < machines[m].buttons.size(); bc++) {
                    if (bc == complement) {
                        machines[m].min_presses = 2;
                        break;
                    }
                }
                if (machines[m].min_presses == 2) {
                    break;
                }
            }
            if (machines[m].min_presses == 2) {
                continue;
            }

            // TODO generalize for 3, 4, .... n button press solutions
        }

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    return 0;
}
