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
            grid = sg;
            orientations = generate_orientations(sg);
        }

        int id;
        ShapeGrid grid;
        std::vector<ShapeGrid> orientations;

    private:
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
        
        std::vector<ShapeGrid> generate_orientations(ShapeGrid g) {
            std::vector<ShapeGrid> orientations;

            // Start with original then start flipping and rotating to create others
            orientations.push_back(g);
            orientations.push_back(generate_orientations_helper(orientations[orientations.size() - 1], 'v'));
            orientations.push_back(generate_orientations_helper(orientations[orientations.size() - 1], 'h'));
            orientations.push_back(generate_orientations_helper(orientations[orientations.size() - 1], 'v'));
            orientations.push_back(generate_orientations_helper(orientations[orientations.size() - 1], 'r'));
            orientations.push_back(generate_orientations_helper(orientations[orientations.size() - 1], 'v'));
            orientations.push_back(generate_orientations_helper(orientations[orientations.size() - 1], 'h'));
            orientations.push_back(generate_orientations_helper(orientations[orientations.size() - 1], 'v'));

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
            shape_counts = sc;
            grid = ShapeGrid(h, std::vector<bool>(w,0));
        }

        int height;
        int width;
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
int fit_region(Region region, std::vector<Shape> shapes) {
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
                    print_shape(s.orientations[j]);
                    place_present(region, s.orientations[j], row, col);
                }
            }
        }
    }
    print_shape(region.grid);
    // Figure out if any combination of one orientation per shape needed can be fit
    return 0;
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
        while (getline(input_file, line)) {
            int colon = line.find(':'); 
            int idx;
            if (colon != std::string::npos) {
                idx = std::stoi(line.substr(0,colon));
            }

            // Read present shape
            ShapeGrid shape(3, std::vector<bool>(3,0));
            int taken = line.find('#');
            int empty = line.find('.');
            int i = 0;
            // Find the lines relating to the present shape
            if (taken != std::string::npos || empty != std::string::npos) {
                while (taken != std::string::npos || empty != std::string::npos) {
                    std::string ln = line.substr(0, line.size());
                    for (int j = 0; j < ln.size(); j++) {
                        if (ln[j] == '#') {
                            shape[i][j]=(true);
                        } else if (ln[j] == '.') {
                            shape[i][j]=(false);
                        }
                    }
                    i++;
                    getline(input_file, line);
                    taken = line.find('#');
                    empty = line.find('.');
                }
                shapes.push_back(Shape(idx,shape));
                continue;
            }

            // Read region shape
            int row;
            int col;
            std::vector<int> shape_quantity;
            int x = line.find('x');
            if (x != std::string::npos) {
                row = std::stoi(line.substr(0,x));
                col = std::stoi(line.substr(x + 1,colon));
                int last_space = 0;
                int current_space = line.find(' ',last_space);
                // Read the number of each present desired for the region
                while (current_space != std::string::npos && current_space < line.size()) {
                    last_space = current_space + 1;
                    shape_quantity.push_back(std::stoi(line.substr(last_space, 1)));
                    current_space = line.find(' ',last_space);
                }
                regions.push_back(Region(row,col,shape_quantity));
            }
        }

        // Print what was read
        for (Shape s : shapes) {
            std::cout << "Shape Index: " << s.id << "\n";
            print_shape(s.grid);
        }
        for (Region r : regions) {
            std::cout << "Rows: " << r.height << " Cols: " << r.width << " Present Counts: ";
            for (int sc : r.shape_counts) {
                std::cout << " " << sc;
            }
            std::cout << "\n";
            print_shape(r.grid);
        }

        std::cout << "Find Solution:\n";
        // Fit presents into region and sum up the number of regions for which all presents fit
        int sum = 0;
        for (Region r : regions) {
            sum += fit_region(r,shapes);
            return 0;
        }

        std::cout << "\nTOTAL: " << sum << " Regions Fit \n";

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    std::cout << '\n';

    return 0;
}
