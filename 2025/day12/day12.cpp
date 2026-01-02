#include <iostream>
#include <fstream>
#include <string>
#include <vector>

/*
    Orientations:

    ### 1
    ##.
    ##.

    ### 2
    .##
    .##

    .## 3
    .##
    ###

    ##. 4
    ##.
    ###

    ### 5
    ###
    ..#

    ..# 6
    ###
    ###

    #.. 7
    ###
    ###

    ### 8
    ###
    #..
___________________________________________

    ### 1
    #..
    ###

    ### 2
    ..#
    ###

    #.# 3
    #.#
    ###

    ### 4
    #.#
    #.#

*/

// Alias for shape_grid
using ShapeGrid = std::vector<std::vector<bool>>;

class Shape {
    public:
        Shape(int i, ShapeGrid sg) {
            id = i;
            area = calculate_area(sg);
            grid = sg;
            orientations = generate_orientations(sg);
        }

        int id;
        int area;
        ShapeGrid grid;
        std::vector<ShapeGrid> orientations;

    private:
        int calculate_area(ShapeGrid &sg) {
            int a = 0;
            for (int i = 0; i < sg.size(); i++) {
                for (int j = 0; j < sg[i].size(); j++) {
                    if (sg[i][j]) {
                        a++;
                    }
                }
            }
            return a;
        }

        ShapeGrid generate_orientations_helper(ShapeGrid &orig, const char flip) {
            ShapeGrid new_grid = orig;
            for (int i = 0; i < orig.size(); i++) {
                for (int j = 0; j < orig[i].size(); j++) {
                    if (flip == 'v') {
                        // Flip vertically
                        new_grid[orig.size() - 1 - i][j] = orig[i][j];
                    } else if (flip == 'h') {
                        // Flip horizontally
                        new_grid[i][orig[i].size() - 1 - j] = orig[i][j];
                    } else if (flip == 'r') {
                        // Rotate 90 degrees CCW
                        new_grid[j][i] = orig[i][j];
                    }
                }
            }

            return new_grid;
        }

        bool grids_match(ShapeGrid &a, ShapeGrid &b) {
            if (a.size() != b.size()) { return false; }

            for (int i = 0; i < a.size(); i++) {
                if (a[i].size() != b[i].size()) { return false; }
                for (int j = 0; j < a[i].size(); j++) {
                    if (a[i][j] != b[i][j]) { return false; }
                }
            }

            return true;
        }
        
        std::vector<ShapeGrid> generate_orientations(ShapeGrid g) {
            std::vector<ShapeGrid> orientations;

            // Start with original then start flipping and rotating to create others
            orientations.push_back(g);
            std::vector<char> rotations = { 'v', 'h', 'v', 'r', 'v', 'h', 'v' };
            for (const char &r : rotations) {
                // For each new orientation, test to see if it's a duplicate (due to symmetry)
                ShapeGrid o = generate_orientations_helper(orientations.back(), r);
                bool new_orientation = true;
                for (ShapeGrid grid : orientations) {
                    if (grids_match(grid, o)) {
                        new_orientation = false;
                        break;
                    }
                }
                if (new_orientation) {
                    orientations.push_back(o);
                }
            }
            
            return orientations;
        }
};

// Print a shape grid with # for true and . for false
void print_shape(ShapeGrid sg) {
    for (int i = 0; i < sg.size(); i++) {
        for (int j = 0; j < sg[i].size(); j++) {
            if (sg[i][j]) {
                std::cout << "#";
            } else {
                std::cout << ".";
            }
        }
        std::cout << "\n";
    }
}

class Region {
    public:
        Region(int h, int w, std::vector<int> sc) {
            height = h;
            width = w;
            area = h * w;
            shape_counts = sc;
            grid = ShapeGrid(h, std::vector<bool>(w,0));
        }

        int height;
        int width;
        int area;
        std::vector<int> shape_counts;
        ShapeGrid grid;
};

// PLACEMENT FUNCTIONS

// Check if a present could be placed in a given spot on a grid
bool check_placement(Region region, ShapeGrid sg, int row, int col) {
    // Check if the present would go outside the region grid
    if (row + sg.size() >= region.grid.size() || col + sg[0].size() >= region.grid[0].size()) {
        return false;
    }

    // Now check if any spot is already occupied
    for (int r = 0; r < sg.size(); r++) {
        for (int c = 0; c < sg[r].size(); c++) {
            if (region.grid[row][col]) {
                // Space already occupied, return false
                std::cout << row << " r | c " << col << " FALSE\n";
                return false;
            }
        }
    }

    // Yes, you could place a present here
    return true;
}

// Place present on grid at specified location
void place_present(Region region, ShapeGrid sg, int row, int col) {
    for (int r = 0; r < sg.size(); r++) {
        for (int c = 0; c < sg[r].size(); c++) {
            if (sg[r][c]) {
                region.grid[row][col] = sg[r][c];
            }
        }
    }
}

// Put a present in a part of the grid starting from row and col on the grid
bool check_and_place_present(Region region, ShapeGrid sg, int row, int col) {
    // Check if the present can be placed here
    bool can_be_placed = check_placement(region, sg, row, col);
    if (!can_be_placed) {
        return can_be_placed;
    }

    // It can be placed here, so place it and return true
    place_present(region, sg, row, col);
    return true;
}

// SOLVING FUNCTIONS

// Solve for if the region can fit the shapes needed
bool fit_region(Region region, std::vector<Shape> shapes) {
    // First check if shape areas exceed region area
    int total_shapes_area = 0;
    for (int sid = 0; sid < region.shape_counts.size(); sid++) {
        total_shapes_area += region.shape_counts[sid] * shapes[sid].area;
    }
    if (total_shapes_area > region.area) {
        return false;
    }

    // In this region, find out which shapes and how many need to fit
    for (int i = 0; i < region.shape_counts.size(); i++) {
        std::cout << "i:" << i << " ";

        // How many of this shape is needed
        int sc = region.shape_counts[i];
        if (sc == 0) {
            std::cout << "skip\n";
            continue;
        }
        std::cout << "sc:" << sc << "\n";

        // Access that shape from the list of shapes
        Shape s = shapes[i];
        
        // Place the present in the region
        for (int row = 0; row <= region.height - s.grid.size(); row++) {
            for (int col = 0; col <= region.width - s.grid.size(); col++) {
                // Try placing the present for every orientation of that shape
                for (int j = 0; j < s.orientations.size(); j++) {
                    ShapeGrid sg = s.orientations[j];
                    place_present(region, s.orientations[j], row, col);
                }
            }
        }
    }
    // Figure out if any combination of one orientation per shape needed can be fit
    return false;
}

// DRIVER CODE

int main() {
    std::string line;
    std::ifstream input_file;

    std::vector<Shape> shapes;
    std::vector<Region> regions;

    // Open input file
    input_file.open("example_input.txt");

    if (input_file.is_open()) {
        // Read line by line
        int index = -1;
        ShapeGrid grid;
        while (getline(input_file, line)) {
            // If this is a region
            if (line.find('x') != std::string::npos) {
                // Read region shape
                int x_ind = line.find('x');
                int colon_ind = line.find(':');
                int width = std::stoi(line.substr(0, x_ind));
                int height = std::stoi(line.substr(x_ind + 1, colon_ind - x_ind - 1));

                // Read the number of each present desired for the region
                std::vector<int> shape_quantity;
                int current_space = colon_ind + 1;
                int next_space = line.find(' ', current_space + 1);
                while (current_space != std::string::npos && current_space < line.size()) {
                    if (next_space == std::string::npos) {
                        next_space = line.size();
                    }
                    shape_quantity.push_back(std::stoi(line.substr(current_space + 1, next_space - current_space - 1)));
                    current_space = next_space;
                    next_space = line.find(' ', current_space + 1);
                }
                regions.push_back(Region(height, width, shape_quantity));
                continue;
            }

            // If starting a new shape
            if (index == -1) {
                index = std::stoi(line.substr(0, line.find(':')));
                continue;
            }
            
            // If this is the end of a shape
            if (line == "") {
                shapes.push_back(Shape(index, grid));
                index = -1;
                grid.clear();
                continue;
            }

            // If this is a shape line
            int pound_ind = line.find('#');
            if (pound_ind != std::string::npos) {
                std::vector<bool> grid_row;
                for (int i = 0; i < line.size(); i++) {
                    if (line[i] == '#') {
                        grid_row.push_back(true);
                    } else if (line[i] == '.') {
                        grid_row.push_back(false);
                    }
                }
                grid.push_back(grid_row);
                continue;
            }
        }

        // Print what was read
        for (Shape s : shapes) {
            std::cout << "Shape Index: " << s.id << "\n";
            for (int i = 0; i < s.orientations.size(); i++) {
                print_shape(s.orientations[i]);
                std::cout << '\n';
            }
            std::cout << '\n';
        }
        for (Region r : regions) {
            std::cout << "Rows: " << r.height << " Cols: " << r.width << " Present Counts: ";
            for (int sc : r.shape_counts) {
                std::cout << " " << sc;
            }
            std::cout << "\n";
            print_shape(r.grid);
            std::cout << '\n';
        }

        std::cout << "Find Solution:\n";
        // Fit presents into region and sum up the number of regions for which all presents fit
        int sum = 0;
        for (const Region r : regions) {
            if (fit_region(r, shapes)) {
                sum++;
            }
        }

        std::cout << "\nTOTAL: " << sum << " Regions Fit \n";

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    return 0;
}
