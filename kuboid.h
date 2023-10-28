#pragma once

#include "polyhedron.h"

class Kuboid {
public:
    struct Face {
        array<Vertex<int>, 4> points;
        Vertex<int> center;
        Vertex<int> n;

        void draw(Magick::Image &img, const Magick::Color &color) {
            drawLine(points[0], points[1], img, color);
            drawLine(points[1], points[2], img, color);
            drawLine(points[2], points[3], img, color);
            drawLine(points[3], points[0], img, color);
//            drawLine(center, center + n, img, color);
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

    void rotate(double alpha, double betta, double gamma, const Vertex<int> &center = {0, 0, 0}) {
        for (auto &face: faces) {
            face.n.rotate(alpha, betta, gamma, center);
            face.center.rotate(alpha, betta, gamma, center);
            for (auto &v: face.points) {
                v.rotate(alpha, betta, gamma, center);
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

    void show(Magick::Image &img, const Magick::Color &color) {
        for (auto &face: faces) {
            if (face.n.z == 0)
                continue;
            if (face.n.z < 0)
                face.draw(img, color);
        }
    }

    void drawBounds(Magick::Image &img, const Magick::Color &color) {
        for (auto &face: faces)
            face.draw(img, color);
    }
};