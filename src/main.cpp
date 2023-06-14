#include <iostream>
#include <algorithm>
#include "ply_shape.h"
#include "atlas_contour.h"

#define DATA_DIR "data/src/"
#define DEBUG_DIR "data/debug/"

#define TEST_SAMPLE "uv_input_1.ply"
//#define TEST_SAMPLE "uv_input_2.ply"
//#define TEST_SAMPLE "uv_input_simple.ply"


int main() {
    using namespace sac;
    std::vector<Shape> shapes = import_ply(std::string(DATA_DIR) + TEST_SAMPLE);

//    std::cout << "In order of the AABB area: " << std::endl;
    std::sort(shapes.begin(), shapes.end(), greater_bb_area);

//    for (auto const &sh: shapes)
//        std::cout << sh;

    // count vertices
    int verts = 0;
    for (const Shape &sh: shapes)
        verts += sh.vertices.size();

    std::vector<std::pair<float, float>> offsets;
    offsets.reserve(verts);

    Contour contour;
    for (auto const &sh : shapes) {
        offsets.push_back(contour.addShape(sh));
    }

    export_ply(shapes, offsets, std::string(DEBUG_DIR) + TEST_SAMPLE);
    export_ply_aabb(shapes, offsets, std::string(DEBUG_DIR) + "aabb_" + TEST_SAMPLE);

    std::cout << "Total area: " << contour.area() << std::endl;

    return 0;
}
