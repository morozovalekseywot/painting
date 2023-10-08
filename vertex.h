#pragma once

#include <iostream>
#include <cmath>

using namespace std;

template<typename T>
concept Arithmetic =  std::is_integral_v<T> || std::is_floating_point_v<T>;

template<typename T> requires Arithmetic<T>
class Vertex {
public:
    T x = 0, y = 0, z = 0;

    Vertex(T _x = 0, T _y = 0) : x(_x), y(_y), z(0) {}

    Vertex(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

    Vertex operator+(const Vertex &a) const {
        return Vertex(a.x + x, a.y + y, a.z + z);
    }

    Vertex operator+=(const Vertex &a) {
        x += a.x;
        y += a.y;
        z += a.z;
        return *this;
    }

    Vertex operator-(const Vertex &a) const {
        return Vertex(x - a.x, y - a.y, z - a.z);
    }

    Vertex operator-=(const Vertex &a) {
        x += a.x;
        y += a.y;
        z += a.z;
        return *this;
    }

    [[nodiscard]] double mod() const {
        return sqrt(x * x + y * y + z * z);
    }

    [[nodiscard]] T mod2() const {
        return x * x + y * y + z * z;
    }

    bool operator==(const Vertex &a) const {
        if (a.x != x || a.y != y || a.z != z)
            return false;
        else
            return true;
    }

    bool operator!=(const Vertex &a) const {
        if (a.x != x || a.y != y || a.z != z)
            return true;
        else
            return false;
    }

    void normalize() {
        T len = mod();
        if (len > 0.0) {
            x /= len;
            y /= len;
            z /= len;
        }
    }

    template<typename C>
    Vertex operator*(const C &a) const {
        return Vertex(x * a, y * a, z * a);
    }

    template<typename C>
    Vertex operator/(const C &a) const {
        return Vertex(x / a, y / a, z / a);
    }

    T operator*(const Vertex &a) const {
        return a.x * x + a.y * y + a.z * z;
    }

    Vertex operator-() const {
        return Vertex(-x, -y, -z);
    }

    ~Vertex() = default;
};

template<typename T>
inline Vertex<T> cross(const Vertex<T> &a, const Vertex<T> &b) {
    return Vertex(a.y * b.z - a.z * b.y, -a.x * b.z + a.z * b.x, a.x * b.y - a.y * b.x);
}

template<typename T>
inline T area(const Vertex<T> &a, const Vertex<T> &b) {
    return a.x * b.y - a.y * b.x;
}

template<typename T>
inline std::ostream &operator<<(std::ostream &os, const Vertex<T> &a) {
    os << a.x << " " << a.y << " " << a.z;
    return os;
}

template<typename T>
inline T dist(const Vertex<T> &a, const Vertex<T> &b) {
    return (b - a).mod();
}

template<typename T>
inline bool equal(const Vertex<T> &a, const Vertex<T> &b, T eps = 0.0) {
    return abs(a.x - b.x) < eps && abs(a.y - b.y) < eps && abs(a.z - b.z) < eps;
}