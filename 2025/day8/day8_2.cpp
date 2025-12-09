#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <functional>
#include <unordered_map>

/*
    Part 2:

    
*/

class Point {
    public:
        Point (int a, int b, int c, int cid) {
            x = a;
            y = b;
            z = c;
            circ_id = cid;
        }

        int x;
        int y;
        int z;
        int circ_id;

        double distance(Point p) const {
            return std::sqrt(std::pow(x - p.x, 2) + std::pow(y - p.y, 2) + std::pow(z - p.z, 2));
        }

        // Friend function: stream operator overload
        friend std::ostream & operator<<(std::ostream & os, const Point & p);
};

std::ostream & operator<<(std::ostream & os, Point const & p) {
    os << "Point: " << p.x << "," << p.y << "," << p.z << " (Circuit " << p.circ_id << ")";
    return os;
}

class Pair {
    public:
        Pair (Point* p1, Point* p2, double d) {
            point_1 = p1;
            point_2 = p2;
            dis = d;
        }

        Point* point_1;
        Point* point_2;
        double dis;

        // Friend function: stream operator overload
        friend std::ostream & operator<<(std::ostream & os, const Pair & p);

        // For min-heap prioritization of Pairs by distance
        bool operator > (const Pair & other) const {
            return dis > other.dis;
        }
};

std::ostream & operator<<(std::ostream & os, Pair const & p) {
    os << "Pair: " << *p.point_1 << " -> " << *p.point_2 << " (" << p.dis << ")";
    return os;
}

int main() {
    std::string line;
    std::ifstream input_file;

    int connections = 1000;

    std::vector<Point*> points;
    std::unordered_map<int, std::vector<Point*>> circuits;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pairs;

    // Open input file
    input_file.open("input.txt");

    if (input_file.is_open()) {
        // Read line by line
        int circuit_id = -1;
        while (getline(input_file, line)) {
            // Parse line for coordinates separated by commas
            int first_comma_pos = line.find(',');
            int second_comma_pos = line.find(',', first_comma_pos + 1);
            // Create point
            Point* p = new Point(
                std::stoi(line.substr(0, first_comma_pos)),
                std::stoi(line.substr(first_comma_pos + 1, second_comma_pos - first_comma_pos - 1)),
                std::stoi(line.substr(second_comma_pos + 1)),
                ++circuit_id
            );
            // Create circuit with same ID
            std::vector<Point*> circ = { p };
            circuits[circuit_id] = circ;
            points.push_back(p);
        }

        // Create pairs of points and put them into a min-heap by distance
        for (int p1 = 0; p1 < points.size(); p1++) {
            for (int p2 = p1 + 1; p2 < points.size(); p2++) {      
                pairs.push(Pair(points[p1], points[p2], (*points[p1]).distance(*points[p2])));
            }
        }

        // Connect the "connections" closest pairs
        for (int c = 0; c < connections; c++) {
            Pair close_pair = pairs.top();
            // If this pair is not yet part of the same circuit, connect (combine) their circuits
            if (close_pair.point_1->circ_id != close_pair.point_2->circ_id) {
                // Larger circuit absorbs the smaller circuit
                int l_cid;
                int s_cid;
                if (circuits[close_pair.point_1->circ_id].size() >= circuits[close_pair.point_2->circ_id].size()) {
                    l_cid = close_pair.point_1->circ_id;
                    s_cid = close_pair.point_2->circ_id;
                } else {
                    l_cid = close_pair.point_2->circ_id;
                    s_cid = close_pair.point_1->circ_id;
                }

                // Move points from the smaller circuit to the bigger circuit
                for (int p = 0; p < circuits[s_cid].size(); p++) {
                    circuits[s_cid][p]->circ_id = l_cid;
                    circuits[l_cid].push_back(circuits[s_cid][p]);
                }

                // Empty out and discard the smaller circuit
                circuits[s_cid].clear();
                circuits.erase(s_cid);
            }
            pairs.pop();
        }

        // Get largest 3 circuit sizes
        int l = 0;
        int sl = 0;
        int tl = 0;
        for (const auto & pair : circuits) {
            if (pair.second.size() >= l) {
                tl = sl;
                sl = l;
                l = pair.second.size();
            } else if (pair.second.size() >= sl) {
                tl = sl;
                sl = pair.second.size();
            } else if (pair.second.size() >= tl) {
                tl = pair.second.size();
            }
        }

        // Multiply together the size of the 3 largest circuits
        long result = l * sl * tl;

        std::cout << "Result: " << result << std::endl;

        // Delete allocated points
        for (int i = 0; i < points.size(); i++) {
            delete points[i];
        }

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    return 0;
}
