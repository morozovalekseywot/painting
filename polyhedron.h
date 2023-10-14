#pragma once

#include "draw.h"
#include "segment.h"
#include "bounding_box.h"
#include <cmath>

template<class T>
T get(const vector<T> &vec, int i) {
    if (i < vec.size()) {
        if (i >= 0)
            return vec[i];
        else
            return vec[i + vec.size()];
    }
    return vec[i - vec.size()];
}

inline bool isBoxIntersects(int a, int b, int c, int d) {
    if (a > b) swap(a, b);
    if (c > d) swap(c, d);
    return max(a, c) <= min(b, d);
}

enum PlaceType {
    CROSS,
    PARALLEL,
    COLLINEAR,
};

pair<bool, PlaceType>
intersectSegment(const Vertex<int> &a, const Vertex<int> &b, const Vertex<int> &c, const Vertex<int> &d) {
    int ab_cd = -area(b - a, d - c);
    if (ab_cd == 0) {  // параллельны
        if (area(d - c, c - a) != 0)
            return {false, PARALLEL};
        // лежат на одной прямой
        return {isBoxIntersects(a.x, b.x, c.x, d.x)
                && isBoxIntersects(a.y, b.y, c.y, d.y), COLLINEAR};
    }

    double t1 = static_cast<double>(area(d - c, c - a)) / ab_cd;
    double t2 = static_cast<double>(area(b - a, c - a)) / ab_cd;

//    double x1 = a.x + t1 * (b.x - a.x);
//    double y1 = a.y + t1 * (b.y - a.y);
//
//    double x2 = c.x + t2 * (d.x - c.x);
//    double y2 = c.y + t2 * (d.y - c.y);
//
//    assert(abs(x1 - x2) < 1e-6);
//    assert(abs(y1 - y2) < 1e-6);

    return {0 <= t1 && t1 <= 1 && 0 <= t2 && t2 <= 1, CROSS};
}


tuple<double, double, PlaceType>
intersectionPoint(const Vertex<int> &a, const Vertex<int> &b, const Vertex<int> &c, const Vertex<int> &d) {
    int ab_cd = -area(b - a, d - c);
    if (ab_cd == 0) {  // параллельны
        if (area(d - c, c - a) != 0)
            return {0, 0, PARALLEL};
        // лежат на одной прямой
        if (a.x == c.x) {
            auto from_1 = min(a.y, b.y);
            auto to_1 = max(a.y, b.y);
            auto from_2 = min(c.y, d.y);
            auto to_2 = max(c.y, d.y);
            return {(from_2 - from_1) / (to_1 - from_1), 0, COLLINEAR};
        } else {
            auto from_1 = min(a.x, b.x);
            auto to_1 = max(a.x, b.x);
            auto from_2 = min(c.x, d.x);
            auto to_2 = max(c.x, d.x);
            return {(from_2 - from_1) / (to_1 - from_1), 0, COLLINEAR};
        }
    }

    double t1 = static_cast<double>(area(d - c, c - a)) / ab_cd;
    double t2 = static_cast<double>(area(b - a, c - a)) / ab_cd;

    return {t1, t2, CROSS};
}

pair<bool, PlaceType>
intersectSegment(const Segment<int> &first, const Segment<int> &second) {
    return intersectSegment(first.a, first.b, second.a, second.b);
}

bool isInsideSegment(const Segment<int> &segm, const Vertex<int> &v) {
    return (segm.a - v) * (segm.b - v) < 0;
}

class Polyhedron {
private:
    vector<Segment<int>> segments;
public:
    explicit Polyhedron(const vector<Vertex<int>> &_points) {
        auto points = _points;
        if (area(points[1] - points[0], points[2] - points[0]) > 0)
            std::reverse(points.begin(), points.end()); // do CW

        segments = vector<Segment<int>>(points.size());
        for (size_t i = 0; i < points.size() - 1; i++) {
            segments[i] = {points[i], points[i + 1]};
        }
        segments.back() = {points.back(), points[0]};

        fixNormals(getCenter());
    };

    void drawBounds(Magick::Image &img, const Magick::Color &col) {
        if (segments.empty())
            return;
        for (auto &segm: segments)
            segm.draw(img, col);
    }

    [[nodiscard]] bool isConvex() const {
        if (!IsSimple())
            return false;
        int n = segments.size();
        if (n <= 2)
            return false;

        int sign = area(segments[0].vec(), segments[1].vec()) > 0 ? 1 : -1;
        for (int i = 1; i < n - 1; i++) {
            if (sign * area(segments[i].vec(), segments[i + 1].vec()) <= 0)
                return false;
        }
        if (sign * area(segments.back().vec(), segments[0].vec()) <= 0)
            return false;

        return true;
    }

    [[nodiscard]] bool IsSimple() const {
        int n = segments.size();
        if (n <= 2)
            return false;

        for (int i = 2; i < n - 1; ++i) {
            if (intersectSegment(segments[0], segments[i]).first)
                return false;
        }

        for (int i = 1; i < n - 2; ++i) {
            for (int j = i + 2; j < n; ++j) {
                if (intersectSegment(segments[i], segments[j]).first)
                    return false;
            }
        }

        return true;
    }

    [[nodiscard]] bool isInsideEvenOddRule(const Vertex<int> &v) const {
        if (segments.size() <= 2)
            return false;

        Vertex<int> end(0, v.y);
        Segment<int> line = {v, end};
//        end = v + (end - v) * 100;
        int count = 0;
        for (auto &segm: segments) {
            auto check = intersectSegment(segm, line);
            if (check.first) {
                if (check.second == CROSS || isInsideSegment(segm, v))
                    count++;
            }
        }

        if (count % 2 == 0) {
            return false;
        }

        return true;
    }

    [[nodiscard]] bool isInsideNonZeroWinding(const Vertex<int> &v) const {
        if (segments.size() <= 2)
            return false;

        Vertex<int> end(0, v.y);
        Vertex l = end - v;
        Segment<int> line = {v, end};
//        end = v + (end - v) * 100;
        int winding = 0;
        for (auto &segm: segments) {
            auto check = intersectSegment(segm, line);
            if (check.first) {
                if (check.second == CROSS || isInsideSegment(segm, v))
                    winding += area(segm.vec(), l) > 0 ? 1 : -1;
            }
        }

        if (winding == 0) {
            return false;
        }

        return true;
    }

    void fillWithEvenOddRule(Magick::Image &img, const Magick::Color &col) const {
        if (segments.size() <= 2)
            return;

        BoundingBox<int> bbox(segments);
        for (int i = bbox.getXMin(); i < bbox.getXMax(); i++) {
            for (int j = bbox.getYMin(); j < bbox.getYMax(); j++) {
                if (isInsideEvenOddRule({i, j}))
                    img.pixelColor(i, j, col);
            }
        }
    }

    void fillWithNonZeroWinding(Magick::Image &img, const Magick::Color &col) const {
        if (segments.size() <= 2)
            return;

        BoundingBox<int> bbox(segments);
        for (int i = bbox.getXMin(); i < bbox.getXMax(); i++) {
            for (int j = bbox.getYMin(); j < bbox.getYMax(); j++) {
                if (isInsideNonZeroWinding({i, j}))
                    img.pixelColor(i, j, col);
            }
        }
    }

    [[nodiscard]] Vertex<int> getCenter() const {
        Vertex<int> center;
        for (auto &segm: segments) {
            center += segm.a;
        }

        return center / segments.size();
    }

    void move(const Vertex<int> &shift) {
        for (auto &segm: segments) {
            segm.a += shift;
            segm.b += shift;
        }
    }

    void scale(double s) {
        Vertex<int> center = getCenter();
        for (auto &segm: segments) {
            segm.a = (segm.a - center) * s + center;
            segm.b = (segm.b - center) * s + center;
        }
    }

    void fixNormals(const Vertex<int> &point) {
        for (auto &segm: segments) {
            if (segm.n * (point - segm.getCenter()) < 0)
                segm.n = -segm.n;
        }
    }

    [[nodiscard]] vector<Segment<int>> getSegments() const {
        return segments;
    }

    ~Polyhedron() = default;
};

Segment<int> cyrusBeckClipLine(const Segment<int> &line, const Polyhedron &pol) {
    auto l = line.vec();
    double t1 = 0, t2 = 1;
    for (auto &segm: pol.getSegments()) {
        auto ans = intersectionPoint(line.a, line.b, segm.a, segm.b);
        if (get<2>(ans) == PlaceType::PARALLEL)
            continue;
        if (l * segm.n > 0) {
            t1 = max(t1, get<0>(ans));
        } else {
            t2 = min(t2, get<0>(ans));
        }
    }

    if (t1 > t2)
        return Segment<int>{line.a, line.a};

    Vertex<int> a = {roundToInt(line.a.x + l.x * t1), roundToInt(line.a.y + l.y * t1), roundToInt(line.a.y + l.y * t1)};
    Vertex<int> b = {roundToInt(line.a.x + l.x * t2), roundToInt(line.a.y + l.y * t2), roundToInt(line.a.y + l.y * t2)};

    return Segment<int>{a, b};
}