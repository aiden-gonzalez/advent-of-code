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

std::vector<ShapeGrid> generate_orientations(ShapeGrid g) {
    // TODO simplify this with helper function
    std::vector<ShapeGrid> orientations;

    // Original
    orientations.push_back(g);

    // Flip vertical
    ShapeGrid g1 = g;
    for (int i = 0; i < g.size(); i++) {
        for (int j = 0; j < g[i].size(); j++) {
            g1[g.size()-1-i][j] = g[i][j];
        }
    }
    orientations.push_back(g1);

    // Flip horizontal
    ShapeGrid g2 = g;
    for (int i = 0; i < g.size(); i++) {
        for (int j = 0; j < g[i].size(); j++) {
            g2[i][g[i].size()-1-j] = g1[i][j];
        }
    }
    orientations.push_back(g2);

    // Flip vertical
    ShapeGrid g3 = g;
    for (int i = 0; i < g.size(); i++) {
        for (int j = 0; j < g[i].size(); j++) {
            g3[g[i].size()-1-i][j] = g2[i][j];
        }
    }
    orientations.push_back(g3);

    // Rotate 90 ccw
    ShapeGrid g4 = g;
    for (int i = 0; i < g.size(); i++) {
        for (int j = 0; j < g[i].size(); j++) {
            g4[j][i] = g3[i][j];
        }
    }
    orientations.push_back(g4);

    // Flip vertical
    ShapeGrid g5 = g;
    for (int i = 0; i < g.size(); i++) {
        for (int j = 0; j < g[i].size(); j++) {
            g5[g[i].size()-1-i][j] = g4[i][j];
        }
    }
    orientations.push_back(g5);

    // Flip horizontal
    ShapeGrid g6 = g;
    for (int i = 0; i < g.size(); i++) {
        for (int j = 0; j < g[i].size(); j++) {
            g6[i][g[i].size()-1-j] = g5[i][j];
        }
    }
    orientations.push_back(g6);

    // Flip vertical
    ShapeGrid g7 = g;
    for (int i = 0; i < g.size(); i++) {
        for (int j = 0; j < g[i].size(); j++) {
            g7[g[i].size()-1-i][j] = g6[i][j];
        }
    }
    orientations.push_back(g7);

    return orientations;
}

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
};

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
        std::vector<std::vector<bool>> grid;
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

// Put a present in a part of the grid starting from row and col on the grid
bool place_present(Region region, ShapeGrid sg, int row, int col) {
    // Check if the present would go outside the region grid
    if (row + sg.size() >= region.grid.size() || col + sg[0].size() >= region.grid[0].size()) {
        return false;
    }

    for (int r = 0; r < sg.size(); r++) {
        for (int c = 0; c < sg[r].size(); c++) {
            if (region.grid[row][col]) {
                std::cout << row << " r | c " << col << " FALSE\n";
            } else {
                std::cout << row << " r | c " << col << " TRUE\n";
            }
            region.grid[row][col] = sg[r][c];
        }
    }

    return true;
}

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

int main() {
    std::string line;
    std::ifstream input_file;

    // Open input file
    input_file.open("example_input.txt");

    if (input_file.is_open()) {
        // Read line by line
        std::vector<Shape> shapes;
        std::vector<Region> regions;
        while (getline(input_file, line)) {
            int colon = line.find(':'); 
            int idx;
            if (colon != std::string::npos) {
                idx = std::stoi(line.substr(0,colon));
            }

            // Read present shape
            std::vector<std::vector<bool>> shape(3, std::vector<bool>(3,0));
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
            for (int i = 0; i < s.orientations.size(); i++) {
                std::cout << "Orientation " << i << "\n";
                print_shape(s.orientations[i]);
            }
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
