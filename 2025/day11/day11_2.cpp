#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

/*
    Part 2:

    Find all paths from svr to out that also visit dac and fft.
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

void dfs(Node* root, std::string target, int &count, bool dac_seen, bool fft_seen) {
    // If we have reached the target AND dac and fft were seen, increment the count
    if (root->name == target && dac_seen && fft_seen) {
        count++;
        std::cout << "Count is now: " << count << '\n';
    }

    // If this is dac
    if (root->name == "dac") {
        dac_seen = true;
    }

    // If this is fft
    if (root->name == "fft") {
        fft_seen = true;
    }

    // Traverse the cables
    for (int c = 0; c < root->cables.size(); c++) {
        dfs(root->cables[c], target, count, dac_seen, fft_seen);
    }
}

int main() {
    std::string line;
    std::ifstream input_file;

    std::unordered_map<std::string, Node*> nodes;

    // Open input file
    input_file.open("input.txt");

    if (input_file.is_open()) {
        // Read line by line
        while (getline(input_file, line)) {
            // First just fill up list of nodes
            std::string node_name = line.substr(0, line.find(':'));
            nodes[node_name] = new Node(node_name);
        }

        // Go back to beginning of file
        input_file.clear();
        input_file.seekg(0);

        // Also add "out" node
        nodes["out"] = new Node("out");

        // Now read in cable connections
        while (getline(input_file, line)) {
            Node* root = nodes.at(line.substr(0, line.find(':')));
            int node_start = line.find(' ') + 1;
            int node_end = line.find(' ', node_start) - 1;
            if (node_end < 0) {
                node_end = line.size() - 1;
            }
            do {
                // Grab node name and add the address of that node to cable connections
                std::string node_name = line.substr(node_start, node_end - node_start + 1);
                root->cables.push_back(nodes.at(node_name));

                // Identify next node
                node_start = node_end + 2;
                node_end = line.find(' ', node_start) - 1;
                if (node_end < 0) {
                    node_end = line.size() - 1;
                }
            } while (node_start < line.size());
        }

        // Print out nodes for check
        for (const auto& node : nodes) {
            if (node.first != "out") {
                std::cout << *node.second << '\n';
            }
        }

        // Find number of paths from node "svr" to node "out"
        Node* svr = nodes.at("svr");
        int num_paths = 0;
        dfs(svr, "out", num_paths, false, false);

        std::cout << "Number of paths: " << num_paths << '\n';

        // Free all created nodes
        for (const auto& node : nodes) {
            delete node.second;
        }

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    return 0;
}
