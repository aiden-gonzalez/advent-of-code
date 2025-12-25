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

// Simple one target
void dfs(Node* root, std::string start, std::string target, int &count) {
    // If we have reached the target, increment the count
    if (root->name == target) {
        count++;
        //std::cout << "Count is now: " << count << '\n';
        // Don't traverse any further (just in case)
        return;
    }

    // Traverse the cables
    for (int c = 0; c < root->cables.size(); c++) {
        // Don't go to the starting node again (avoid potential cycle)
        if (root->cables[c]->name == start) {
            continue;
        }

        dfs(root->cables[c], start, target, count);
    }
}

// Two target
void dfs(Node* root, std::string start, std::string target_one, std::string target_two, int &count_one, int &count_two) {
    // If we have found one of the targets, increment the appropriate count and stop
    if (root->name == target_one) {
        count_one++;
        if (count_one % 1000 == 0) {
            std::cout << "Count one is now " << count_one << '\n';
        }
        return;
    }
    if (root->name == target_two) {
        count_two++;
        if (count_two % 1000 == 0) {
            std::cout << "Count two is now " << count_two << '\n';
        }
        return;
    }

    // Traverse the cables
    for (int c = 0; c < root->cables.size(); c++) {
        // Don't go to the starting node again (avoid potential cycle)
        if (root->cables[c]->name == start) {
            continue;
        }

        dfs(root->cables[c], start, target_one, target_two, count_one, count_two);
    }
}

// Start to end with dac and fft detection
void dfs(Node* root, std::string start, std::string target, int &count, bool dac_seen, bool fft_seen) {
    // If we have reached the target AND dac and fft were seen, increment the count
    if (root->name == target && dac_seen && fft_seen) {
        count++;
        std::cout << "Count is now: " << count << '\n';
        // Don't traverse any further (just in case)
        return;
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
        // If we have already seen dac, don't visit it again (avoid cycle)
        if (dac_seen && root->cables[c]->name == "dac") {
            continue;
        }

        // If we have already seen fft, don't visit it again (avoid cycle)
        if (fft_seen && root->cables[c]->name == "fft") {
            continue;
        }

        // Don't go to the starting node again (avoid potential cycle)
        if (root->cables[c]->name == start) {
            continue;
        }

        dfs(root->cables[c], start, target, count, dac_seen, fft_seen);
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

        // Find number of paths
        Node* svr = nodes.at("svr");
        int svr_dac = 0;
        int svr_fft = 0;
        dfs(svr, "svr", "dac", "fft", svr_dac, svr_fft);

        Node* dac = nodes.at("dac");
        int dac_fft = 0;
        int dac_out = 0;
        dfs(dac, "dac", "fft", "out", dac_fft, dac_out);

        Node* fft = nodes.at("fft");
        int fft_dac = 0;
        int fft_out = 0;
        dfs(fft, "fft", "dac", "out", fft_dac, fft_out);

        std::cout << "Number of paths svr dac: " << svr_dac << " | ";
        std::cout << "svr fft: " << svr_fft << " | ";
        std::cout << "dac fft: " << dac_fft << " | ";
        std::cout << "fft dac: " << fft_dac << " | ";
        std::cout << "dac out: " << dac_out << " | ";
        std::cout << "fft out: " << fft_out << '\n';

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
