#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

/*
    There are devices with names. And each line of the input says what other devices
    they are attached to.  And they just want to know how many paths take you from
    "you" (the root) to "out" (the end).

    Graph and traversal. Lets do it.
*/

class Node {
    public:
        Node (std::string nm) {
            name = nm;
        }

        std::string name;
        std::vector<Node*> cables;

        // Friend function: stream operator overload
        friend std::ostream & operator<<(std::ostream & os, const Node & n);
};

std::ostream & operator<<(std::ostream & os, Node const & n) {
    os << n.name << ':';
    for (int i = 0; i < n.cables.size(); i++) {
        os << ' ' << n.cables[i]->name;
    }
    return os;
}

template<>
struct std::hash<Node> {
    std::size_t operator()(const Node& n) const noexcept {
        std::size_t result = std::hash<std::string>()(n.name);
        for (int i = 0; i < n.cables.size(); i++) {
            const std::size_t hash = std::hash<std::string>()(n.cables[i]->name);
            result = result ^ hash << i;
        }
        return result;
    }
};

int main() {
    std::string line;
    std::ifstream input_file;

    std::unordered_map<std::string, Node> nodes;

    // Open input file
    input_file.open("example_input.txt");

    if (input_file.is_open()) {
        // Read line by line
        while (getline(input_file, line)) {
            // First just fill up list of nodes
            std::string node_name = line.substr(0, line.find(':'));
            Node new_node = Node(node_name);
            std::cout << new_node << '\n';
            nodes[node_name] = new_node;
        }

        // Go back to beginning of file
        input_file.clear();
        input_file.seekg(0);

        // Now read in cable connections
        while (getline(input_file, line)) {
            Node root = nodes.at(line.substr(0, line.find(':')));
            int node_start = line.find(' ') + 1;
            int node_end = line.find(' ', node_start) - 1;
            do {
                // Grab node name and add the address of that node to cable connections
                std::string node_name = line.substr(node_start, node_end - node_start + 1);
                root.cables.push_back(&nodes.at(node_name));

                // Identify next node
                node_start = node_end + 2;
                node_end = line.find(' ', node_start) - 1;
            } while (node_start < line.size());
        }

        for (const auto& node : nodes) {
            std::cout << node.second << '\n';
        }

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    return 0;
}
