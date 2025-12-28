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

class Shape_Grid {
    public:
        Shape_Grid(std::vector<std::vector<bool>> g) {
            grid = g;
        }

        std::vector<std::vector<bool>> grid;
};

std::vector<Shape_Grid> generate_orientations(Shape_Grid g) {
    // TODO generate orientations
    std::vector<Shape_Grid> orientations;

    // Original
    orientations.push_back(g);

    // Flip vertical
    Shape_Grid g1 = g;
    for (int i = 0; i < g.grid.size(); i++) {
        for (int j = 0; j < g.grid[i].size(); j++) {
            g1.grid[g.grid.size()-1-i][j] = g.grid[i][j];
        }
    }
    orientations.push_back(g1);

    // Flip horizontal
    Shape_Grid g2 = g;
    for (int i = 0; i < g.grid.size(); i++) {
        for (int j = 0; j < g.grid[i].size(); j++) {
            g2.grid[i][g.grid[i].size()-1-j] = g1.grid[i][j];
        }
    }
    orientations.push_back(g2);

    // Flip vertical
    Shape_Grid g3 = g;
    for (int i = 0; i < g.grid.size(); i++) {
        for (int j = 0; j < g.grid[i].size(); j++) {
            g3.grid[g.grid[i].size()-1-i][j] = g2.grid[i][j];
        }
    }
    orientations.push_back(g3);

    // Rotate 90 ccw
    Shape_Grid g4 = g;
    for (int i = 0; i < g.grid.size(); i++) {
        for (int j = 0; j < g.grid[i].size(); j++) {
            g4.grid[j][i] = g3.grid[i][j];
        }
    }
    orientations.push_back(g4);

    // Flip vertical
    Shape_Grid g5 = g;
    for (int i = 0; i < g.grid.size(); i++) {
        for (int j = 0; j < g.grid[i].size(); j++) {
            g5.grid[g.grid[i].size()-1-i][j] = g4.grid[i][j];
        }
    }
    orientations.push_back(g5);

    // Flip horizontal
    Shape_Grid g6 = g;
    for (int i = 0; i < g.grid.size(); i++) {
        for (int j = 0; j < g.grid[i].size(); j++) {
            g6.grid[i][g.grid[i].size()-1-j] = g5.grid[i][j];
        }
    }
    orientations.push_back(g6);

    // Flip vertical
    Shape_Grid g7 = g;
    for (int i = 0; i < g.grid.size(); i++) {
        for (int j = 0; j < g.grid[i].size(); j++) {
            g7.grid[g.grid[i].size()-1-i][j] = g6.grid[i][j];
        }
    }
    orientations.push_back(g7);

    return orientations;
}

class Shape {
    public:
        Shape(int i, std::vector<std::vector<bool>> sg) {
            id = i;
            grid = sg;
            // orientations.push_back(sg);
            orientations = generate_orientations(sg);
        }

        int id;
        std::vector<std::vector<bool>> grid;
        std::vector<Shape_Grid> orientations;
};

class Region {
    public:
        Region(int h, int w, std::vector<int> sc) {
            height = h;
            width = w;
            shape_counts = sc;
        }

        int height;
        int width;
        std::vector<int> shape_counts;
};

void print_shape(Shape_Grid sg) {
    for (int i = 0; i < sg.grid.size(); i++) {
        for (int j = 0; j < sg.grid[i].size(); j++) {
            if (sg.grid[i][j]) {
                std::cout << "#";
            } else {
                std::cout << ".";
            }
        }
        std::cout << "\n";
    }
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
            for (int i = 0; i < r.shape_counts.size(); i++) {
                std::cout << " " << r.shape_counts[i];
            }
            std::cout << "\n";
        }
        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    std::cout << '\n';

    return 0;
}
