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
    orientations.push_back(g);
    return orientations;
}

class Shape {
    public:
        Shape(int i, std::vector<std::vector<bool>> sg) {
            id = i;
            grid = sg;
            orientations.push_back(sg);
            // orientations = generate_orientations(sg);
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
                std::cout << idx << " next shape dectected\n";
            }

            // Read present shape
            std::vector<std::vector<bool>> shape(3, std::vector<bool>(3,0));
            int taken = line.find('#');
            int empty = line.find('.');
            int i = 0;
            // Find the lines relating to the present shape
            while (taken != std::string::npos || empty != std::string::npos) {
                std::cout << "reading present shape " << idx << "\n";
                std::string ln = line.substr(0, line.size());
                for (int j = 0; j < ln.size(); j++) {
                    std::cout << ln.size() << "\n";
                    std::cout << ln[j] << "\n";
                    if (ln[j] == '#') {
                        shape[i][j]=(true);
                    } else if (ln[j] == '.') {
                        shape[i][j]=(false);
                    }
                    std::cout << "just assigned: " << shape[i][j] << "\n";
                }
                std::cout << "\n";
                i++;
                getline(input_file, line);
                taken = line.find('#');
                empty = line.find('.');
            }
            shapes.push_back(Shape(idx,shape));
            std::cout << "psnext \n";
            // print_shape(shapes[i].orientations[0]);
            
            // Read region shape
            int row;
            int col;
            std::vector<int> shape_quantity;
            int x = line.find('x');
            if (x != std::string::npos) {
                std::cout << "reading region shape\n";
                row = std::stoi(line.substr(0,x));
                col = std::stoi(line.substr(x + 1,colon));
                std::cout << row << " r | c " << col << "\n";
                int last_space = 0;
                int current_space = line.find(' ',last_space);
                // Read the number of each present desired for the region
                while (current_space != std::string::npos && current_space < line.size()) {
                    std::cout << "while" << current_space << " ls " << last_space << "\n";
                    last_space = current_space + 1;
                    shape_quantity.push_back(std::stoi(line.substr(last_space, 1)));
                    current_space = line.find(' ',last_space);
                }
            }
            regions.push_back(Region(row,col,shape_quantity));
        }

        // Print what was read
        for (Shape s : shapes) {
            std::cout << "Shape Index: " << s.id << "\n";
            for (int i = 0; i < s.grid.size(); i++) {
                print_shape(s.orientations[i]);
            }
        }
        for (Region r : regions) {
            std::cout << "Rows: " << r.height << " Cols: " << r.width;
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
