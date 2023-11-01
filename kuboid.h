#pragma once

#include "polyhedron.h"

class Kuboid {
public:
    struct Face {
        array<Vertex<int>, 4> points;
        Vertex<int> center;
        Vertex<int> n;

        void draw(Magick::Image &img, const Magick::Color &color) const {
            drawLine(points[0], points[1], img, color);
            drawLine(points[1], points[2], img, color);
            drawLine(points[2], points[3], img, color);
            drawLine(points[3], points[0], img, color);
        }
    };

    array<Face, 6> faces;

    explicit Kuboid(const array<array<Vertex<int>, 4>, 6> &_faces) {
        Vertex<int> center(0, 0, 0);
        for (size_t i = 0; i < _faces.size(); ++i) {
            faces[i].points = _faces[i];
            faces[i].center = (_faces[i][0] + _faces[i][1] + _faces[i][2] + _faces[i][3]) / 4;
            center += faces[i].center;
        }
        center = center / faces.size();

        for (auto &face: faces) {
            face.n = center - face.center;
        }
    }

    [[nodiscard]] Vertex<int> getCenter() const {
        Vertex<int> center(0, 0, 0);
        for (auto &face: faces)
            center += face.center;
        center = center / faces.size();

        return center;
    }

    void rotate(double alpha, double betta, double gamma, const Vertex<int> &center = {0, 0, 0}) {
        for (auto &face: faces) {
            face.n.rotate(alpha, betta, gamma, center);
            Vertex<int> face_center(0, 0, 0);
            for (auto &v: face.points) {
                v.rotate(alpha, betta, gamma, center);
                face_center += v;
            }
            face.center = face_center / face.points.size();
        }

        fixNormals();
    }

    void rotateAboveAxes(double x, double y, double z, double phi) {
        double l = sqrt(x * x + y * y + z * z);
        double nx = x / l, ny = y / l, nz = z / l;
        for (auto &face: faces) {
            face.n.rotateAboveAxes(nx, ny, nz, phi);
            face.center.rotateAboveAxes(nx, ny, nz, phi);
            for (auto &v: face.points) {
                v.rotateAboveAxes(nx, ny, nz, phi);
            }
        }

        fixNormals();
    }

    void fixNormals() {
        Vertex<int> center(0, 0, 0);
        for (auto &face: faces) {
            center += face.center;
        }
        center = center / faces.size();

        for (auto &face: faces) {
            face.n = center - face.center;
        }
    }

    /// Отображение без скрытых граней
    void show(Magick::Image &img, const Magick::Color &color) const {
        for (auto &face: faces) {
            if (face.n.z <= 0)
                face.draw(img, color);
        }
    }

    /// Отображение всех граней
    void drawBounds(Magick::Image &img, const Magick::Color &color) const {
        for (auto &face: faces)
            face.draw(img, color);
    }

    void onePointProjection(double r, Magick::Image &img, const Magick::Color &color) const {
        Vertex<int> center = getCenter();
        center = Vertex<int>(center.x / (1 + r * center.z),
                             center.y / (1 + r * center.z),
                             center.z / (1 + r * center.z));
        for (auto &face: faces) {
            array<Vertex<int>, 4> points;
            Vertex<int> face_center = Vertex<int>(face.center.x / (1 + r * face.center.z),
                                                  face.center.y / (1 + r * face.center.z),
                                                  face.center.z / (1 + r * face.center.z));
            for (size_t i = 0; i < face.points.size(); i++) {
                auto p = face.points[i];
                points[i] = Vertex<int>(p.x / (1 + r * p.z), p.y / (1 + r * p.z), p.z / (1 + r * p.z));
            }
            Vertex<int> n = cross(points[1] - points[0], points[2] - points[1]);
            if (n * (center - face_center) < 0) {
                n = -n;
            }

            if (n.z < 0) {
                continue;
            }
            if (n.z == 0 && (n.x < 0 || n.y < 0)) {
                continue;
            }
            for (size_t i = 0; i < face.points.size(); i++) {
                drawLine(points[i], points[(i + 1) % face.points.size()], img, color);
            }
        }
    }
};