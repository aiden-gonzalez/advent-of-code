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

long gi(long row,long col,long gw) {
    return gw * row + col;
} 

int main() {
    std::string line;
    std::ifstream input_file;

    // Open input file
    input_file.open("input.txt");
    
    if (input_file.is_open()) {
        // Read file to find biggest x and y coord
        int gw = 0;
        int gh = 0;
        while (getline(input_file, line)) {
            int comma_idx = line.find(',');
            int x = std::stoi(line.substr(0, comma_idx));
            int y = std::stoi(line.substr(comma_idx + 1));

            if (x > gw) {
                gw = x;
            }
            if (y > gh) {
                gh = y;
            }
        }
        // Change width and height to size rather than last coordinate
        gw++;
        gh++;

        std::vector<Point> points;
        std::priority_queue<Pair> rectangles;

        // Initialize 1D vector to represent the grid
        // to index through the elements of the grid, row 0 is 0 to gw-1, row 1 is gw*1 to gw*2 - 1
        // index = gw * row + col
        // for row index 1 col index 2 if grid is 3x3: gw*1 + 2 = grid[5]
        std::vector<bool> grid (gh * gw, false);

        // Reset file pointer
        input_file.clear();
        input_file.seekg(0);

        // Read points line by line
        while (getline(input_file, line)) {
            int comma_idx = line.find(',');
            
            Point p = Point(
                std::stoi(line.substr(0, comma_idx)),
                std::stoi(line.substr(comma_idx + 1))
            );
            
            if (points.size() > 0) {
                Point prev_p = points.back();
                // Horizontal line
                if (p.x != prev_p.x) {
                    // Draw horizontal line in grid
                    for (int col = std::min(p.x, prev_p.x); col <= std::max(p.x, prev_p.x); col++) {
                        grid[gi(p.y, col, gw)] = true;
                    }
                // Vertical line
                } else if (p.y != prev_p.y) {
                    // Draw vertical line in grid
                    for (int row = std::min(p.y, prev_p.y); row <= std::max(p.y, prev_p.y); row++) {
                        grid[gi(row, p.x, gw)] = true;
                    }
                }
            } else {
                // Mark first point on grid
                grid[gi(p.y, p.x, gw)] = true;
            }

            points.push_back(p);
        }

        // Draw last line
        // Horizontal line
        Point p = points.back();
        Point prev_p = points.front();
        if (p.x != prev_p.x) {
            // Draw horizontal line in grid
            for (int col = std::min(p.x, prev_p.x); col <= std::max(p.x, prev_p.x); col++) {
                grid[gi(p.y, col, gw)] = true;
            }
        // Vertical line
        } else if (p.y != prev_p.y) {
            // Draw vertical line in grid
            for (int row = std::min(p.y, prev_p.y); row <= std::max(p.y, prev_p.y); row++) {
                grid[gi(row, p.x, gw)] = true;
            }
        }

        // Fill the inside of the shape
        for (int row = 0; row < gh; row++) {
            for (int col = 0; col < gw; col++) {
                std::cout << "Filling " << row << " " << col << '\n';
                // If we encounter a 1, fill space
                if (grid[gi(row, col, gw)]) {
                    // Handle possible horizontal line
                    while (col < gw && grid[gi(row, col, gw)]) {
                        col++;
                    }
                    int fill_start = col;
                    // Fill until another 1 (or the end of the grid)
                    while (col < gw && !grid[gi(row, col, gw)]) {
                        grid[gi(row, col, gw)] = true;
                        col++;
                    }
                    // If we went outside the grid without seeing a 1, undo the fill
                    if (col == gw) {
                        for (int i = col - 1; i >= fill_start; i--) {
                            grid[gi(row, i, gw)] = false;
                        }
                        break;
                    }
                }
            }
        }

        // TODO remove: preview grid
        for (int i = 0; i < gh; i++) {
            for (int j = 0; j < gw; j++) {
                std::cout << grid[gi(i, j, gw)];
            }
            std::cout << std::endl;
        }

        // Create all allowable rectangles
        for (int i = 0; i < points.size() - 1; i++) {
            for (int j = i + 1; j < points.size(); j++) {
                // Check that rectangle is allowable
                bool allowed = true;
                for (int x = std::min(points[i].x, points[j].x); x <= std::max(points[i].x, points[j].x); x++) {
                    for (int y = std::min(points[i].y, points[j].y); y <= std::max(points[i].y, points[j].y); y++) {
                        if (!grid[gi(y, x, gw)]) {
                            allowed = false;
                            break;
                        }
                    }
                    if (!allowed) {
                        break;
                    }
                }
                if (allowed) {
                    rectangles.push(Pair(
                        &points[i],
                        &points[j],
                        points[i].area(points[j])
                    ));
                }
            }
        }

        Pair largest = rectangles.top();
        std::cout << "Largest rectangle area: " << largest.area << '\n';
        std::cout << largest.point_1->x << "," << largest.point_1->y << '\n';
        std::cout << largest.point_2->x << "," << largest.point_2->y << std::endl;

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    return 0;
}
