#include <iostream>
#include <cmath>
#include <algorithm>
#include "happly.h"

#define TEST_SAMPLE "data/src/uv_input_1.ply"
//#define TEST_SAMPLE "data/src/uv_input_2.ply"
//#define TEST_SAMPLE "data/src/uv_input_simple.ply"


/** 2D-shape with precalculated options, such as:
 * perimeter
 * area
 * AABB-box
 * vertex coordinates are 2d, with coordinates relative to upper-left corner of bounding box.
 */
float distance(float x1, float y1, float x2, float y2) {
    float dx_sqr = (x2 - x1) * (x2 - x1);
    float dy_sqr = (y2 - y1) * (y2 - y1);
    return std::sqrt(dx_sqr + dy_sqr);
}

class Shape {
public:
    /**
     *
     * @param vertices uses only first two vectors (supposedly x and y)
     * @param indices indices of vertices that are used, in some order.
     */
    Shape(const std::vector<std::vector<float>> &vertices, const std::vector<int> &indices);
    friend std::ostream& operator<<(std::ostream&, const Shape&);
    friend bool greater_area(Shape& a, Shape& b);
//private:
    std::vector<std::pair<float, float>> vertices;
    double area;
    float perimeter;
    float AABB_xmin;
    float AABB_ymin;
    float AABB_xmax;
    float AABB_ymax;
};



Shape::Shape(const std::vector<std::vector<float>> &vertices, const std::vector<int> &indices) {
    std::vector<std::pair<float, float>> res_vertices;
    float area = 0;
    float perimeter = 0;

    float xmin = std::numeric_limits<float>::max();
    float ymin = std::numeric_limits<float>::max();
    float xmax = std::numeric_limits<float>::min();
    float ymax = std::numeric_limits<float>::min();

    for (int i = 0; i < indices.size(); ++i) {
        int j = (i + 1) % indices.size();
        int ind1 = indices[i];
        int ind2 = indices[j];

        float x1 = vertices[0][ind1];
        float x2 = vertices[0][ind2];
        float y1 = vertices[1][ind1];
        float y2 = vertices[1][ind2];

        xmin = std::min(xmin, x1);
        ymin = std::min(ymin, y1);
        xmax = std::max(xmax, x1);
        ymax = std::max(ymax, y1);

        perimeter += distance(x1, y1, x2, y2);
        area += (x1*y2 -  x2*y1);

        res_vertices.emplace_back(x1, y1);
    }

    this->area = std::abs(area * 0.5);
    this->perimeter = perimeter;

    this->AABB_xmin = xmin;
    this->AABB_ymin = ymin;
    this->AABB_xmax = xmax;
    this->AABB_ymax = ymax;

    for (auto v : res_vertices) {
        v.first  -= xmin;
        v.second -= ymin;
    }
    this->vertices.swap(res_vertices);
}

std::ostream &operator<<(std::ostream &os, const Shape &sh) {
    os << "Shape with " << sh.vertices.size() << " vertices." << std::endl;
    os << "Area: " << sh.area << std::endl;
    os << "Perimeter: " << sh.perimeter << std::endl;
    os << std::endl;

    os << "Bounding box: [" << sh.AABB_xmin << " - " << sh.AABB_xmax << "] x [" << sh.AABB_ymin << " - " << sh.AABB_ymax << "]" << std::endl;

    return os;
}

bool greater_area(Shape &a, Shape &b) {
    return (a.area > b.area);
}


int main() {
    happly::PLYData plyIn(TEST_SAMPLE);
    std::vector<std::vector<float>> vertices;
    vertices.push_back(plyIn.getElement("vertex").getProperty<float>("x"));
    vertices.push_back(plyIn.getElement("vertex").getProperty<float>("y"));
    vertices.push_back(plyIn.getElement("vertex").getProperty<float>("z"));

    std::vector<std::vector<int>> faces =
            plyIn.getElement("face").getListProperty<int>("vertex_index");

    std::vector<Shape> shapes;

    for (int i = 0; i < vertices[0].size(); ++i) {
        std::cout << i << " " << vertices[0][i] << " " << vertices[1][i] << " " << vertices[2][i] << std::endl;
    }
    for (int i = 0; i < faces.size(); ++i) {
        std::cout << "Shape " << i << std::endl;
        for (int j = 0; j < faces[i].size(); ++j) {
            std::cout << faces[i][j] << " ";
        }
        std::cout << std::endl;

        shapes.emplace_back(vertices, faces[i]);

        auto sh = shapes.back();

        std::cout << sh << std::endl;
        for (auto v : sh.vertices) {
            std::cout << "(" << v.first << ", " << v.second << "); ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "In order of the area: " << std::endl;
    std::sort(shapes.begin(), shapes.end(), greater_area);

    for (auto const& sh: shapes)
        std::cout << sh;

    return 0;
}
