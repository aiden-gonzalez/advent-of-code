#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/*
    Part 2:

    Find all paths from svr to out that also visit dac and fft.

    Turns out there are a lot of paths. Like a LOT.

    We are assuming at this point that this is a DAG. All the testing we have done
    indicates that it is, as far as we can tell.  To do this efficiently, we can
    use Kahn's algorithm to topographically sort the graph. Then, work backwards
    from the target to the source.

    This also means we have to calculate and track what nodes point to a given node.
    Not just what nodes a given node points to.  That's an extra step we will need
    to do. But we can easily set that up using the map of nodes we have already
    set up.
*/

class Node {
    public:
        Node (std::string nm) {
            name = nm;
            paths_to_target = 0;
        }

        std::string name;
        std::vector<Node*> cables_out;
        std::vector<Node*> cables_in;
        int paths_to_target;

        // Friend function: stream operator overload
        friend std::ostream & operator<<(std::ostream & os, const Node & n);
};

std::ostream & operator<<(std::ostream & os, Node const & n) {
    if (n.cables_in.size() > 0) {
        for (int i = 0; i < n.cables_in.size(); i++) {
            os << n.cables_in[i]->name << ' ';
        }
        os << "-> ";
    } 
    os << "[" << n.name << "] ->";
    for (int i = 0; i < n.cables_out.size(); i++) {
        os << ' ' << n.cables_out[i]->name;
    }
    return os;
}

template<>
struct std::hash<Node> {
    std::size_t operator()(const Node& n) const noexcept {
        std::size_t result = std::hash<std::string>()(n.name);
        for (int i = 0; i < n.cables_out.size(); i++) {
            const std::size_t hash = std::hash<std::string>()(n.cables_out[i]->name);
            result = result ^ hash << i;
        }
        return result;
    }
};

void print_nodes(std::vector<Node*>& nodes) {
    std::cout << "Nodes vector:";
    for (int i = 0; i < nodes.size(); i++) {
        std::cout << ' ' << (*nodes[i]).name << ",";
    }
    std::cout << '\n';
}

void print_path(std::vector<std::string>& path, std::string last_node = "") {
    std::cout << path[0];
    for (int p = 1; p < path.size(); p++) {
        std::cout << " -> " << path[p];
    }
    if (last_node != "") {
        std::cout << " -> " << last_node;
    }
    std::cout << '\n';
}

// Simple one target
void dfs(Node* root, std::string& start, std::string& target, int &count) {
    // If we have reached the target, increment the count
    if (root->name == target) {
        count++;
        std::cout << "Count is now: " << count << '\n';
        // Don't traverse any further (just in case)
        return;
    }

    // Traverse the cables
    for (int c = 0; c < root->cables_out.size(); c++) {
        // Don't go to the starting node again (avoid potential cycle)
        if (root->cables_out[c]->name == start) {
            continue;
        }

        dfs(root->cables_out[c], start, target, count);
    }
}

// Two target (no cycle detection)
void dfs(Node* root, std::string target_one, std::string target_two, int &count_one, int &count_two) {
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
    for (int c = 0; c < root->cables_out.size(); c++) {
        dfs(root->cables_out[c], target_one, target_two, count_one, count_two);
    }
}

// Two target with cycle detection
void dfs(Node* root, std::vector<std::string>& seen, std::string target_one, std::string target_two, int &count_one, int &count_two) {
    // If we have found one of the targets, increment the appropriate count and stop
    if (root->name == target_one) {
        count_one++;
        if (count_one % 100 == 0) {
            std::cout << "Count one is now " << count_one << '\n';
            print_path(seen, root->name);
        }
        return;
    }
    if (root->name == target_two) {
        count_two++;
        if (count_two % 100 == 0) {
            std::cout << "Count two is now " << count_two << '\n';
            print_path(seen, root->name);
        }
        return;
    }
    
    // Add this node to "seen" nodes
    seen.push_back(root->name);

    // Traverse the cables
    for (int c = 0; c < root->cables_out.size(); c++) {
        // Avoid cycles
        if (std::find(seen.begin(), seen.end(), root->cables_out[c]->name) != seen.end()) {
            std::cout << "Cycle detected! Haha\n";
            continue;
        }

        dfs(root->cables_out[c], seen, target_one, target_two, count_one, count_two);
    }

    // Remove this node from "seen" nodes
    seen.pop_back();
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
    for (int c = 0; c < root->cables_out.size(); c++) {
        // If we have already seen dac, don't visit it again (avoid cycle)
        if (dac_seen && root->cables_out[c]->name == "dac") {
            continue;
        }

        // If we have already seen fft, don't visit it again (avoid cycle)
        if (fft_seen && root->cables_out[c]->name == "fft") {
            continue;
        }

        // Don't go to the starting node again (avoid potential cycle)
        if (root->cables_out[c]->name == start) {
            continue;
        }

        dfs(root->cables_out[c], start, target, count, dac_seen, fft_seen);
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

        // Read in cable connections
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
                root->cables_out.push_back(nodes.at(node_name));

                // Identify next node
                node_start = node_end + 2;
                node_end = line.find(' ', node_start) - 1;
                if (node_end < 0) {
                    node_end = line.size() - 1;
                }
            } while (node_start < line.size());
        }

        // Set up "cables_in" pointers
        for (const auto& node : nodes) {
            // For every node this node points to, add this node to that node's "cables_in"
            for (const auto& out_node : node.second->cables_out) {
                out_node->cables_in.push_back(node.second);
            }
        }

        // Print out nodes for check
        std::cout << "Read all nodes:\n";
        for (const auto& node : nodes) {
            if (node.first != "out") {
                std::cout << *node.second << '\n';
            }
        }
        std::cout << '\n';

        // Do topological sort of nodes
        std::vector<Node*> nodes_sorted;
        std::unordered_set<Node*> zero_in_nodes;
        for (const auto& node : nodes) {
            // If node has no in-cables, add to set
            if (node.second->cables_in.size() == 0) {
                zero_in_nodes.insert(node.second);
            }
        }
        while (zero_in_nodes.size() > 0) {
            // Remove a node from zero_in_nodes
            Node* n = *(zero_in_nodes.begin());
            zero_in_nodes.erase(n);

            // Add node to sorted list
            nodes_sorted.push_back(n);

            // For each node pointed to by this node
            for (const auto& out_node : n->cables_out) {
                // Use erase-remove idiom to erase this node from "cables_in"
                out_node->cables_in.erase(
                    std::remove(
                        out_node->cables_in.begin(),
                        out_node->cables_in.end(),
                        n
                    ),
                    out_node->cables_in.end()
                );
                // We will not actually modify cables out, since we still need it later
                // later.  But if we didn't need it, we'd clear cables_out to completely
                // remove the cable from the graph.

                // If no other income edges for out_node, insert into zero_in_nodes
                if (out_node->cables_in.size() == 0) {
                    zero_in_nodes.insert(out_node);
                }
            }
        }

        // Cycle check:
        for (const auto& node : nodes) {
            if (node.second->cables_in.size() > 0) {
                std::cout << "ERROR: Graph has at least one cycle!";
                return 1;
            }
        }

        // Print out topologically sorted nodes for check
        print_nodes(nodes_sorted);
        std::cout << '\n';
        
        // Find number of paths
        nodes_sorted[nodes_sorted.size() - 1]->paths_to_target = 1;
        for (int i = nodes_sorted.size() - 2; i >= 0; i--) {
            // Number of paths to target is the sum of the paths for all its descendants
            int sum = 0;
            for (const auto& n : nodes_sorted[i]->cables_out) {
                sum += n->paths_to_target;
            }
            nodes_sorted[i]->paths_to_target = sum;
        }

        std::cout << "ANSWER: " << nodes_sorted[0]->paths_to_target << " paths\n";

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
