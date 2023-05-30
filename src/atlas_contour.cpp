#include "atlas_contour.h"
#include <vector>
#include <algorithm>
#include <cassert>


# define STRATEGY_FIRST 1
# define STRATEGY_RANDOM_BEST 0

# define EPS 1e-5

namespace sac {
    Contour::Node::Node(float x, float y) : x(x), y(y) {}

    Contour::Node::operator std::pair<float, float>() const {
        return {x, y};
    }

    std::pair<float, float> Contour::addShape(const Shape &shape) {
        float dx = shape.aabb.xsize();
        float dy = shape.aabb.ysize();

        std::vector<std::pair<INode, float>> candidates;

        int zero_cost_candidates = 0;

        for (auto it = nodes.begin(); it != nodes.end(); it++) {
            if (!isUpperLeftCorner(it)) {
                continue;
            }

            if (isHavingEnoughSpace(it, dx, dy)) {
                float starting_area = xmax * ymax;
                xmax = std::max(xmax, dx + it->x);
                ymax = std::max(ymax, dy + it->y);
                float cost = (xmax * ymax) - starting_area;

                if (cost < EPS) {
#if STRATEGY_FIRST
                    return addShapeToNode(shape, it);
#endif
                    zero_cost_candidates++;
                }
                candidates.emplace_back(it, cost);
            }
            else {
                ;
            }
        }
        assert(!candidates.empty());

        std::sort(candidates.begin(), candidates.end(),
                  [](std::pair<INode, float> a, std::pair<INode, float> b) {
                      return a.second <= b.second; 
                  });
        if (zero_cost_candidates) {
#if STRATEGY_RANDOM_BEST
            throw "not_implemented";
#endif
        }


        return addShapeToNode(shape, candidates.front().first);
    }

    Contour::Contour() {
        nodes.push_back(Node(0.f, 0.f));
    }

    bool Contour::isHavingEnoughSpace(std::list<Contour::Node>::iterator node, float xsize, float ysize) {
        float x_edge = node->x + xsize;
        float y_edge = node->y + ysize;

        for (auto cur = std::make_reverse_iterator(node); cur != nodes.rend() && cur->x < x_edge; cur++) {
            if (cur->y > y_edge)
                return false;
        }

        for (auto cur = std::next(node); cur != nodes.end() && cur->y < y_edge; cur++) {
            if (cur->x > x_edge)
                return false;
        }

        return true;
    }

    std::pair<float, float> Contour::addShapeToNode(const Shape &shape, std::list<Contour::Node>::iterator it) {
        float x = it->x;
        float y = it->y;
        float dx = shape.aabb.xsize();
        float dy = shape.aabb.ysize();

        //
        //(0)->0
        //     |
        // 2---1
        it->x += dx;
        auto it1 = nodes.insert(std::next(it), Node(it->x, it->y + dy));
        nodes.insert(std::next(it1), Node(it->x - dx, it->y + dy));

        return {x, y};
    }

    bool Contour::isUpperLeftCorner(std::list<Contour::Node>::iterator it) const {
        auto prev = std::make_reverse_iterator(it);
        auto next = std::next(it);
        return (prev == nodes.rend()
                || next == nodes.end()
                || (prev->x > it->x && next->y > it->y));
    }

    float Contour::area() const {
        return xmax * ymax;
    }
}