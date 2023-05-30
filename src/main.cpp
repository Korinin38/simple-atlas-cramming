#include <iostream>
#include <algorithm>
#include "happly.h"
#include "ply_shape.h"

#define DATA_DIR "data/src/"
#define DEBUG_DIR "data/debug/"

#define TEST_SAMPLE "uv_input_1.ply"
//#define TEST_SAMPLE "uv_input_2.ply"
//#define TEST_SAMPLE "uv_input_simple.ply"


int main() {
    using namespace sac;
    std::vector<Shape> shapes = import_ply(std::string(DATA_DIR) + TEST_SAMPLE);

    std::cout << "In order of the AABB area: " << std::endl;
    std::sort(shapes.begin(), shapes.end(), greater_bb_area);

    for (auto const &sh: shapes)
        std::cout << sh;

    // count vertices
    int verts = 0;
    for (const Shape &sh: shapes)
        verts += sh.vertices.size();

    std::vector<std::pair<float, float>> offsets(verts, {0, 0});

    export_ply(shapes, offsets, std::string(DEBUG_DIR) + TEST_SAMPLE);
    return 0;
}
