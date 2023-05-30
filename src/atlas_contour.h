#ifndef SIMPLE_ATLAS_CRAMMING_ATLAS_CONTOUR_H
#define SIMPLE_ATLAS_CRAMMING_ATLAS_CONTOUR_H

#include "ply_shape.h"
#include <utility>
#include <list>

namespace sac {
    class Contour {
        class Node {
        public:
            float x;
            float y;

            Node(float x, float y);

            operator std::pair<float, float>() const;
        };

        float xmax = 0;
        float ymax = 0;
        std::list<Node> nodes;

        typedef std::list<Node>::iterator INode;

        bool isUpperLeftCorner(INode it) const;
//        Node head;

        bool isHavingEnoughSpace(INode it, float xsize, float ysize);

        std::pair<float, float> addShapeToNode(const Shape &shape, std::list<Contour::Node>::iterator it);
    public:
        Contour();

        /**
         * @return upper-leftmost point of bounding box of shape
         */
        std::pair<float, float> addShape(const Shape &shape);

        float area() const;
    };

}

#endif //SIMPLE_ATLAS_CRAMMING_ATLAS_CONTOUR_H
