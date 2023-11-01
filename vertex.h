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

    void rotate(double alpha, double betta, double gamma, const Vertex<int> &center = {0, 0, 0}) {
        double cos_a = cos(alpha), sin_a = sin(alpha);
        double cos_b = cos(betta), sin_b = sin(betta);
        double cos_g = cos(gamma), sin_g = sin(gamma);
        Vertex<int> p = *this - center;
        x = center.x + cos_b * cos_g * p.x - sin_g * cos_b * p.y + sin_b * p.z;
        y = center.y + (sin_a * sin_b * cos_g + sin_g * cos_a) * p.x +
            (-sin_a * sin_b * sin_g + cos_a * cos_g) * p.y - sin_a * cos_b * p.z;
        z = center.z + (sin_a * sin_g - sin_b * cos_a * cos_g) * p.x +
            (sin_a * cos_g + sin_b * sin_g * cos_a) * p.y + cos_a * cos_b * p.z;
    }

    void rotateAboveAxes(double nx, double ny, double nz, double phi) {
        double cos_phi = cos(phi), sin_phi = sin(phi);
        double new_x = (cos_phi + nx * nx * (1 - cos_phi)) * x + (nx * ny * (1 - cos_phi) - nz * sin_phi) * y +
                       (nx * nz * (1 - cos_phi) + ny * sin_phi) * z;
        double new_y = (nx * ny * (1 - cos_phi) + nz * sin_phi) * x + (cos_phi + ny * ny * (1 - cos_phi)) * y +
                       (ny * nz * (1 - cos_phi) - nx * sin_phi) * z;
        double new_z = (nx * nz * (1 - cos_phi) - ny * sin_phi) * x + (ny * nz * (1 - cos_phi) + nx * sin_phi) * y +
                       (cos_phi + nz * nz * (1 - cos_phi)) * z;
        x = new_x;
        y = new_y;
        z = new_z;
    }

    template<typename C>
    Vertex operator*(const C &a) const {
        return Vertex(x * a, y * a, z * a);
    }

    Vertex<int> multy(double c) const {
        return Vertex(int(round(c * x)), int(round(c * y)), int(round(c * z)));
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

    auto operator<=>(const Vertex &) const = default;

    ~Vertex() = default;
};

template<typename T>
inline Vertex<T> cross(const Vertex<T> &a, const Vertex<T> &b) {
    return Vertex(a.y * b.z - a.z * b.y, -a.x * b.z + a.z * b.x, a.x * b.y - a.y * b.x);
}

template<typename T>
inline Vertex<T> scalar(const Vertex<T> &a, const Vertex<T> &b) {
    return Vertex(a.x * b.x, a.y * b.y, a.z * b.z);
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

Vertex<double> convertToDoubleVertex(const Vertex<int> &a) {
    return Vertex<double>{(double) a.x, (double) a.y, (double) a.z};
}

int roundToInt(double x) {
    return int(round(x));
}