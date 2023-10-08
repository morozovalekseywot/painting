#pragma once

#include "vertex.h"

template<typename T> requires Arithmetic<T>
class Segment {
public:
    Vertex<T> a;
    Vertex<T> b;

    Segment() {};

    Segment(const Vertex<T> &a, const Vertex<T> &b) : a(a), b(b) {}

    [[nodiscard]] bool isInside(const Vertex<int> &v) const {
        return area(b - v, a - v) == 0 && (a - v) * (b - v) <= 0;
    }

    void draw(Magick::Image &img, const Magick::Color &col) const {
        drawLine(a.x, a.y, b.x, b.y, img, col);
    }

    Vertex<T> vec() const {
        return b - a;
    }
};