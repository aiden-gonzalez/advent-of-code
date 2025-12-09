#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <functional>
#include <unordered_set>

/*
    You step onto the repaired teleporter pad.  You rematerialize on an unfamiliar
    teleporter pad and find yourself in a vast underground space which contains a
    giant playground.

    Across the playground, there is a deocration project going on.  They have
    suspended a large number of small electrical junction boxes.  Their plan is to
    connect the junction boxes with long strings of lights.  Most of them don't
    provide electricity, but when two of them are connected by a string of lights,
    electricity can pass through those two boxes.

    They are trying to figure out which ones to connect so that electricity can
    reach every junction box.  They have a list of all of the boxes positions in
    3D space (the puzzle input).

    Example given.

    Each position is given as X, Y, Z coordinates.  To save on string lights, the
    elves would like to focus on connecting pairs that are as close together as
    possible according to straight-line distance.

    Once the two closest boxes are connected, they become part of the same circuit.
    Now the two that are closest together but still not connected are another pair.
    If you connect those, since one of them was already connected, there is now a
    single circuit that contains those three junction boxes.

    You repeat the process, and two unconnected boxes become connected. Now there is
    the first circuit with 3 boxes still, this new one with 2 boxes, and the rest
    of the boxes are still unconnected (circuits of 1).  This process continues.
    
    If two boxes are already in the same circuit, you don't need to connect them.

    The elves would like to know how big the circuits they are creating will be.
    After making the ten shortest connections, there are 11 circuits. One circuit
    contains 5 boxes, another contains 4, two circuits contain 2 boxes each, and
    seven circuits contain a single junction box.  Multiplying together the size
    of the three largest circuits (5, 4, and 2) produces 40.

    Your list contains many junction boxes.  Connect together the 1000 pairs of boxes
    that are closest together. Then, what do you get if you multiply together the
    sizes of the three largest circuits?

    Ok, interesting problem. The difficulty here is finding the 1000 pairs of boxes
    that are the closest together.  There are 1000 boxes gives in the input, and
    the order / way we connect them matters for the ending size of the circuits. So
    we need to try to connect them in order of distance to each other as given in
    the problem.
    
    Brute-force finding the distance for every possible pair and then sorting seems
    prohibitively expensive.  We need a smarter way to find the 1000 closest pairs.

    Lets say we just want to find the single closest pair in the whole list. Hm.

    On second thought, brute force with 1000 junction boxes might not be that bad.
    We would have about n^2 / 2 pairs.  The combination formula applies here:
    "1000 choose 2" or C(1000, 2), where C(n, r) = n! / (r! * (n - r)!).

    So C(1000, 2) = 1000! / (2! * (1000 - 2)!) = 1000! / (2! * 998!)
    = 1000 * 999 * 998! / 2! * 998! = 1000 * 999 / 2! = 999000 / 2 * 1 = 499500.

    Half a million pairs.  Half a million distances.  It's a lot, but it's not
    unreasonable computationally.

    Another consideration is how to tell which junction boxes are connected, either
    directly or indirectly.  Directly is easy, they are a pair. Indirectly is more
    tricky.  Maybe we can maintain groups of junction boxes which are part of the
    same circuit in hash sets or something.  That way we can efficiently check if
    a junction box is already in the same circuit as another junction box, without
    having to do some complicated search of the circuit or something.

    Once we finish this brute force version, lets create an optimal version
    (day8_opt.cpp) using some techniques we can find online.
*/

class Point {
    public:
        Point (int a, int b, int c) {
            x = a;
            y = b;
            z = c;
        }

        int x;
        int y;
        int z;

        double distance(Point p) const {
            return std::sqrt(std::pow(x - p.x, 2) + std::pow(y - p.y, 2) + std::pow(z - p.z, 2));
        }

        // Friend function: stream operator overload
        friend std::ostream & operator<<(std::ostream & os, const Point & p);

        // Necessary for placing into an unordered_set
        bool operator == (const Point & op) const {
            return x == op.x && y == op.y;
        }
};

// Necessary for placing into an unordered_set
struct PointHash {
    size_t operator() (const Point & point) const {
        size_t xHash = std::hash<int>()(point.x);
        // Shift over yHash by 1 bit
        size_t yHash = std::hash<int>()(point.y) << 1;
        // Shift over zHash by 2 bits
        size_t zHash = std::hash<int>()(point.z) << 2;
        // XOR (exclusive or) the three hashes
        return xHash ^ yHash ^ zHash;
    }
};

std::ostream & operator<<(std::ostream & os, Point const & p) {
    os << "Point: " << p.x << "," << p.y << "," << p.z;
    return os;
}

class Pair {
    public:
        Pair (const Point* p1, const Point* p2, double d) {
            point_1 = p1;
            point_2 = p2;
            dis = d;
        }

        const Point* point_1;
        const Point* point_2;
        double dis;

        // Friend function: stream operator overload
        friend std::ostream & operator<<(std::ostream & os, const Pair & p);

        // For min-heap prioritization of Pairs by distance
        bool operator > (const Pair & other) const {
            return dis > other.dis;
        }
};

std::ostream & operator<<(std::ostream & os, Pair const & p) {
    os << "Pair: " << p.point_1 << " -> " << p.point_2 << " (" << p.dis << ")";
    return os;
}

int main() {
    std::string line;
    std::ifstream input_file;

    std::vector<std::unordered_set<Point, PointHash>> circuits;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pairs;

    // Open input file
    input_file.open("example_input.txt");

    if (input_file.is_open()) {
        // Read line by line
        while (getline(input_file, line)) {
            // Parse line for coordinates separated by commas
            int first_comma_pos = line.find(',');
            int second_comma_pos = line.find(',', first_comma_pos + 1);
            // Create point and new unordered set (circuit)
            const Point p = Point(
                std::stoi(line.substr(0, first_comma_pos)),
                std::stoi(line.substr(first_comma_pos + 1, second_comma_pos - first_comma_pos - 1)),
                std::stoi(line.substr(second_comma_pos + 1))
            );
            std::unordered_set<Point, PointHash> c = { p };
            circuits.push_back(c);
        }

        // Create pairs of points and put them into a min-heap by distance
        for (int c1 = 0; c1 < circuits.size(); c1++) {
            for (int c2 = c1 + 1; c2 < circuits.size(); c2++) {      
                pairs.push(
                    Pair(
                        &(*circuits[c1].begin()),
                        &(*circuits[c2].begin()),
                        (*circuits[c1].begin()).distance(*circuits[c2].begin())
                    )
                );
            }
        }

        // Connect the n closest pairs
        int point_count = circuits.size();
        for (int pair = 0; pair < point_count; pair++) {
            Pair close_pair = pairs.top();
            std::cout << close_pair << '\n';
            pairs.pop();
        }

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    return 0;
}
