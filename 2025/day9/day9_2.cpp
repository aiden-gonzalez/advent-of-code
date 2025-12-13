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

void draw_line(std::vector<std::vector<bool>>& grid, Point *p, Point *prev_p) {
    // Horizontal line
    if (p->x != prev_p->x) {
        // Ensure grid row is large enough (2 rows buffer)
        while (grid[p->y].size() < std::max(p->x, prev_p->x) + 3) {
            grid[p->y].push_back(false);
        }
        // Draw horizontal line in grid
        for (int col = std::min(p->x, prev_p->x); col <= std::max(p->x, prev_p->x); col++) {
            grid[p->y][col] = true;
        }
    // Vertical line
    } else if (p->y != prev_p->y) {
        // Ensure grid has enough rows (1 row buffer)
        while (grid.size() < std::max(p->y, prev_p->y) + 2) {
            std::vector<bool> new_row(p->x + 1, false);
            grid.push_back(new_row);
        }
        // Draw vertical line in grid
        for (int row = std::min(p->y, prev_p->y); row <= std::max(p->y, prev_p->y); row++) {
            grid[row][p->x] = true;
        }
    }
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
        while (getline(input_file, line)) {
            int comma_idx = line.find(',');
            Point p = Point(
                std::stoi(line.substr(0, comma_idx)),
                std::stoi(line.substr(comma_idx + 1))
            );

            if (points.size() > 0) {
                Point prev_p = points.back();                
                draw_line(grid, &p, &prev_p);
            } else {
                // Initialize grid
                for (int row = 0; row <= p.y; row++) {
                    std::vector<bool> row_vec(p.x + 1, false);
                    grid.push_back(row_vec);
                }
                // Mark first point on grid
                grid[p.y][p.x] = true;
            }

            points.push_back(p);
        }

        // Draw last line
        draw_line(grid, &points.back(), &points.front());

        // Rectangularize grid
        int width = 0;
        // Find the max width
        for (int i = 0; i < grid.size(); i++) {
            if (grid[i].size() > width) {
                width = grid[i].size();
            }
        }
        // Make all rows the max width
        for (int row = 0; row < grid.size(); row++) {
            while (grid[row].size() < width) {
                grid[row].push_back(false);
            }
        }

        // Cassandra's thing:
        // for (int row = 0; row < grid.size(); row++) {
        //     for (int col = 0; col < ceil(grid[row].size()/2); col++) {
        //         if (grid[row][col] && !grid[row][col+1]) {
        //                 grid[row][col+1] = true;
        //         }
        //         if (grid[row][grid[row].size()-col]) {
        //                 grid[row][grid[row].size()-col-1] = true;
        //         }
        //     }
        // }
        // Fill the inside of the shape
        for (int row = 0; row < grid.size(); row++) {
            for (int col = 0; col < grid[row].size(); col++) {
                // If we encounter a 1, fill space
                if (grid[row][col]) {
                    // Handle possible horizontal line
                    while (col < grid[row].size() && grid[row][col]) {
                        col++;
                    }
                    int fill_start = col;
                    // Fill until another 1 (or the end of the grid)
                    while (col < grid[row].size() && !grid[row][col]) {
                        grid[row][col] = true;
                        col++;
                    }
                    // If we went outside the grid without seeing a 1, undo the fill
                    if (col == grid[row].size()) {
                        for (int i = col - 1; i >= fill_start; i--) {
                            grid[row][i] = false;
                        }
                        break;
                    }
                }
            }
        }

        // TODO remove: preview grid
        for (std::vector<bool> grid_row : grid) {
            for (bool grid_element : grid_row) {
                std::cout << grid_element;
            }
            std::cout << std::endl;
        }

        // Create all allowable rectangles
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
