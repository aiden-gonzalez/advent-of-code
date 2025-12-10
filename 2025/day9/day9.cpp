#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <functional>
#include <unordered_map>

/*
    Slide down the firepole in the corner and land in the North Pole movie theater.

    Big tile floor with an interesting pattern.  Elves are redecorating by switching
    out some of the square tiles in the big grid they form.  Some of the tiles are
    red. The elves would like to find the largest rectangle that uses red tiles for
    two of its opposite corners.

    They have a list of where the red tiles are located in the grid (puzzle input).

    Example given.

    You can choose any two red tiles as the opposite corners of the rectange. The
    goal then is to find the largest rectangle possible.

    There may be multiple largest rectangles with the same area, keep in mind.

    Using two red tiles as opposite corners, what is the largest area of any rectangle
    you could make?
   
    Brute force solution: calculate every possible rectangle. Stupid but works.

    Want to maximize: (x2 - x1 + 1) * (y2 - y1 + 1) = maximize
*/

class Point {
    public:
        Point (int xcord, int ycord) {
            x = xcord;
            y = ycord;
        }

        int x;
        int y;

        long area(Point p) const {
            return (long)(std::abs(x - p.x) + 1) * (long)(std::abs(y - p.y) + 1);
        }

        // Friend function: stream operator overload
        friend std::ostream & operator<<(std::ostream & os, const Point & p);
};

std::ostream & operator<<(std::ostream & os, Point const & p) {
    os << "Point: " << p.x << "," << p.y;
    return os;
}

class Pair {
    public:
        Pair (Point* p1, Point* p2, long a) {
            point_1 = p1;
            point_2 = p2;
            area = a;
        }

        Point* point_1;
        Point* point_2;
        long area;

        // Friend function: stream operator overload
        friend std::ostream & operator<<(std::ostream & os, const Pair & p);

        // For max-heap prioritization of Pairs by area
        bool operator < (const Pair & other) const {
            return area < other.area;
        }
};

std::ostream & operator<<(std::ostream & os, Pair const & p) {
    os << "Pair: " << *p.point_1 << " -> " << *p.point_2 << " (" << p.area << ")";
    return os;
}

int main() {
    std::string line;
    std::ifstream input_file;

    std::vector<Point> points;
    std::priority_queue<Pair> rectangles;

    // Open input file
    input_file.open("input.txt");

    if (input_file.is_open()) {
        // Read line by line
        while (getline(input_file, line)) {
            int comma_idx = line.find(',');
            points.push_back(Point(
                std::stoi(line.substr(0, comma_idx)),
                std::stoi(line.substr(comma_idx + 1))
            ));
        }

        // Create all possible rectangles
        for (int i = 0; i < points.size(); i++) {
            for (int j = i; j < points.size(); j++) {
                rectangles.push(Pair(
                    &points[i],
                    &points[j],
                    points[i].area(points[j])
                ));
            }
        }

        std::cout << "Largest rectangle area: " << rectangles.top().area << std::endl;

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    return 0;
}
