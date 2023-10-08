#pragma once

#include "vertex.h"

template<typename T> requires Arithmetic<T>
class BoundingBox {
    T x_min, x_max;
    T y_min, y_max;
public:
    explicit BoundingBox(const vector<Vertex<T>> &points) {
        if (points.empty())
            throw std::runtime_error("BoundingBox::Constructor points is empty");

        x_min = x_max = points[0].x;
        y_min = y_max = points[0].y;
        for (auto &p: points) {
            x_min = min(x_min, p.x);
            x_max = max(x_max, p.x);

            y_min = min(y_min, p.y);
            y_max = max(y_max, p.y);
        }
    }

    explicit BoundingBox(const vector<Segment<T>> &segments) {
        if (segments.empty())
            throw std::runtime_error("BoundingBox::Constructor segments is empty");

        x_min = x_max = segments[0].a.x;
        y_min = y_max = segments[0].a.y;
        for (auto &segm: segments) {
            x_min = min(x_min, segm.a.x);
            x_max = max(x_max, segm.a.x);

            y_min = min(y_min, segm.a.y);
            y_max = max(y_max, segm.a.y);
        }
    }

    T getXMin() const {
        return x_min;
    }

    T getXMax() const {
        return x_max;
    }

    T getYMin() const {
        return y_min;
    }

    T getYMax() const {
        return y_max;
    }

    ~BoundingBox() = default;
};
