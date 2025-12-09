#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <functional>
#include <unordered_map>

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

    std::vector<Point> points;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> pairs;

    // Open input file
    input_file.open("example_input.txt");

    if (input_file.is_open()) {
        // Read line by line
        int circuit_id = 0;
        while (getline(input_file, line)) {
            // Parse line for coordinates separated by commas
            int first_comma_pos = line.find(',');
            int second_comma_pos = line.find(',', first_comma_pos + 1);
            // Create point
            const Point p = Point(
                std::stoi(line.substr(0, first_comma_pos)),
                std::stoi(line.substr(first_comma_pos + 1, second_comma_pos - first_comma_pos - 1)),
                std::stoi(line.substr(second_comma_pos + 1)),
                ++circuit_id
            );
            points.push_back(p);
        }

        // Create pairs of points and put them into a min-heap by distance
        for (int p1 = 0; p1 < points.size(); p1++) {
            for (int p2 = p1 + 1; p2 < points.size(); p2++) {      
                pairs.push(Pair(&points[p1], &points[p2], points[p1].distance(points[p2])));
            }
        }

        // Connect the n closest pairs
        for (int pair = 0; pair < points.size(); pair++) {
            Pair close_pair = pairs.top();
            std::cout << close_pair << '\n';
            // If this pair is not yet part of the same circuit, connect (combine) their circuits
            if (close_pair.point_1->circ_id != close_pair.point_2->circ_id) {
                close_pair.point_2->circ_id = close_pair.point_1->circ_id;
            }
            pairs.pop();
        }

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    return 0;
}
