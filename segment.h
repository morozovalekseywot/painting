#pragma once

#include "vertex.h"

template<typename T> requires Arithmetic<T>
class Segment {
public:
    Vertex<T> a; /// начало отрезка
    Vertex<T> b; /// конец отрезка
    Vertex<T> n; /// внутренняя нормаль

    Segment() = default;

    Segment(const Vertex<T> &a, const Vertex<T> &b) : a(a), b(b) {
        n = {(b.y - a.y), {a.x - b.x}, 0};
    }

    Segment(const Vertex<T> &a, const Vertex<T> &b, const Vertex<T> &n) : a(a), b(b), n(n) {}

    [[nodiscard]] bool isInside(const Vertex<int> &v) const {
        return area(b - v, a - v) == 0 && (a - v) * (b - v) <= 0;
    }

    void draw(Magick::Image &img, const Magick::Color &col) const {
        drawLine(a.x, a.y, b.x, b.y, img, col);
    }

    [[nodiscard]] Vertex<T> getCenter() const {
        return (a + b) / 2;
    }

    [[nodiscard]] Vertex<T> getPoint(double t) const {
        return a + (b - a) * t;
    }

    void scale(double s) {
        Vertex<int> center = getCenter();
        a = (a - center) * s + center;
        b = (b - center) * s + center;
    }

    Vertex<T> vec() const {
        return b - a;
    }
};