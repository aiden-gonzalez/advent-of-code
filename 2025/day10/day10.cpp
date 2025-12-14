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
*/

class Machine {
    public:
        Machine (int il, std::vector<int> bs) {
            ind_lights = il;
            buttons = bs;
        }

        int ind_lights;
        std::vector<int> buttons;

        // Friend function: stream operator overload
        friend std::ostream & operator<<(std::ostream & os, const Machine & m);
};

std::ostream & operator<<(std::ostream & os, Machine const & m) {
    os << "Machine: [" << m.ind_lights << "]";
    for (int i = 0; i < m.buttons.size(); i++) {
        std::cout << " (" << m.buttons[i] << ")";
    }
    return os;
}

int main() {
    std::string line;
    std::ifstream input_file;

    std::vector<Machine> machines;

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

        // TODO REMOVE: Preview machines
        for (int i = 0; i < machines.size(); i++) {
            std::cout << machines[i] << '\n';
        }

        // Test: Push 2 buttons
        int lights = 0;
        // Push (0, 2) (10)
        lights = lights ^ 10;
        // Push (0, 1) (12)
        lights = lights ^ 12;
        // Result should equal 6
        std::cout << "lights result: " << lights << '\n';

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    return 0;
}
