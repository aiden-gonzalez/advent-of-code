#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

/*
    Part 2:

    With your analysis of the manifold complete, you begin fixing the teleporter.
    But, you discover that this is actually a quantum tachyon manifold.

    With this type, only a single particle is sent through the manifold.  It takes
    both the left and right path of each splitter encountered.  This is impossible,
    but the manual recommends a certain interpretation.  Each time a particle reaches
    a splitter, it's actually time itself which splits.  In one timeline it goes left,
    in the other timeline it goes right.

    So what we really need to know is the number of timelines active after a single
    particle completes all of its possible journeys through the manifold.

    So there is an exponential growth of these timelines, according to the layout
    of the splitters and possible paths etc.

    I think this should still be able to be solved simply enough, but it again
    tempts me to consider recursion.  We need to count the number of possible paths
    through this branching tree structure.

    Each time it splits, we create two new timelines no?  Say there was one splitter.
    The answer would be two timelines.  Then say we add another splitter. The answer
    is now 3. Hm.  It depends on other factors.

    Each time it hits a splitter, we split one timeline into two.  So in a way, we
    add one.  But it also splits apart every other timeline that involves hitting
    that splitter (there could be multiple).  That's the difference.
    
    We need to keep track of not only splits but which timeline the beam hitting
    the splitter came from.  Or really, how many possible timelines could hit that
    splitter.

    Just one more problem. The number of timelines at each position is growing
    exponentially. It's running quite slow trying to keep track. We need a faster
    way to do this.  Maybe instead of actually manually looping for each timeline
    split, just do the math that would be involved in one step. Should be simple
    enough.
*/

int main() {
    std::string line;
    std::ifstream input_file;

    long num_timelines = 1;
    // Key: x position; Value: number of timelines
    std::unordered_map<int, long> beam_timelines;

    // Open input file
    input_file.open("input.txt");

    if (input_file.is_open()) {
        // Read line by line
        while (getline(input_file, line)) {
            // Scan the line
            for (int i = 0; i < line.length(); i++) {
                // Beam start point
                if (line[i] == 'S') {
                    beam_timelines[i] = 1;
                // Beam split point
                } else if (line[i] == '^' && beam_timelines.count(i) == 1) {
                    // Split each timeline into two timelines
                    num_timelines += beam_timelines[i];
                    if (beam_timelines.count(i - 1) == 0) {
                        beam_timelines[i - 1] = 0;
                    }
                    beam_timelines[i - 1] += beam_timelines[i];
                    if (beam_timelines.count(i + 1) == 0) {
                        beam_timelines[i + 1] = 0;
                    }
                    beam_timelines[i + 1] += beam_timelines[i];
                    beam_timelines[i] = 0;
                }
            }
        }

        input_file.close();
    } else {
        std::cout << "Unable to open file";
        return 1;
    }

    std::cout << "Number of timelines: " << num_timelines << std::endl;

    return 0;
}
