#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <functional>
#include <unordered_map>

/*
    Part 2:

    Rectangle can only include red or green tiles now.  So every red tile is connected
    to the red tile before and after it in the list by a straight line of green tiles.

    The list wraps, so the first red tile is also connected to the last red tile by
    a line of green tiles.  Tiles that are adjacent in your list will always be on
    either the same row or the same column.

    Using the same example as before, tiles marked X would be green.

    In addition to the loop of green tiles, all the tiles inside the loop are also
    green.  The remaining tiles are never red nor green.

    The rectangle you choose still must have red tiles in opposite corners, but any
    other tiles it includes must now be red or green. This significantly limits your
    options.

    Ok so I can take my brute force and maybe add some rules to it.  I need to somehow
    filter out rectangles that would include tiles from outside the approved area.
    - There must exist some other point at or beyond the other two corners.
    - There's more requirements actually. Weird shapes can exist.
    - It might make sense to just draw a grid and check each piece.
*/

class Point {
    public:
        Point (int xcord, int ycord) {
            x = xcord;
            y = ycord;
        }

        Point () {
            x = -1;
            y = -1;
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
    std::vector<std::vector<bool>> grid;
    std::priority_queue<Pair> rectangles;

    // Open input file
    input_file.open("example_input.txt");

    if (input_file.is_open()) {
        // Read line by line
        int idx_start;
        int idx_end;
        bool horizontal;
        while (getline(input_file, line)) {
            int comma_idx = line.find(',');
            Point p = Point(
                std::stoi(line.substr(0, comma_idx)),
                std::stoi(line.substr(comma_idx + 1))
            );

            if (points.size()>1) {
                if (p.x < points.back().x) {
                    idx_start = p.x;
                    idx_end = points.back().x;
                    horizontal = 1;
                } else if (p.x > points.back().x) {
                    idx_start = points.back().x;
                    idx_end = p.x;
                    horizontal = 1;
                } else if (p.y < points.back().y) {
                    idx_start = p.y;
                    idx_end = points.back().y;
                    horizontal = 0;
                } else {
                    idx_start = points.back().y;
                    idx_start = p.y;
                    horizontal = 0;
                }
                if (horizontal) {
                    for (int i = idx_start; i <= idx_end; i++) {
                        grid[i][p.y] = 1;
                    }
                } else {
                    for (int i = idx_start; i <= idx_end; i++) {
                        grid[p.x][i] = 1;
                    }
                }
            }
            points.push_back(p);
        }
        for (std::vector<bool> grid_row : grid) {
            for (bool grid_element : grid_row) {
                std::cout << grid_element << std::endl;
            }
            std::cout << std::endl;
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
