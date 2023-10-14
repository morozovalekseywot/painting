#pragma once

#include <iostream>
#include <cmath>
#include <Magick++.h>
#include "vertex.h"

using namespace std;

void drawLine(int x1, int y1, int x2, int y2, Magick::Image &img, const Magick::Color &col) {
    if (x1 > x2) {
        swap(x1, x2);
        swap(y1, y2);
    }
    const int delta_x = 2 * abs(x2 - x1), delta_y = 2 * abs(y2 - y1);
    const int step_x = x1 < x2 ? 1 : -1, step_y = y1 < y2 ? 1 : -1;
    int error = delta_x - delta_y;
    while (x1 != x2 && y1 != y2) {
        img.pixelColor(x1, y1, col);
        if (error > -delta_y) {
            error -= delta_y;
            x1 += step_x;
        }
        if (error < delta_x) {
            error += delta_x;
            y1 += step_y;
        }
    }
    while (x1 != x2) {
        img.pixelColor(x1, y2, col);
        x1 += step_x;
    }
    while (y1 != y2) {
        img.pixelColor(x2, y1, col);
        y1 += step_y;
    }
    img.pixelColor(x2, y2, col);
}

void drawLine(const Vertex<int> &from, const Vertex<int> &to, Magick::Image &img, const Magick::Color &color) {
    drawLine(from.x, from.y, to.x, to.y, img, color);
}


vector<int> getCombCoeffs(int n) {
    if (n == 1)
        return {1};
    vector<int> coeffs(n);
    coeffs[0] = 1;
    coeffs.back() = 1;
    for (int i = 1; i < n / 2; i++) {
        coeffs[i] = coeffs[i - 1] * (n - i) / i;
        coeffs[n - i - 1] = coeffs[i];
    }
    if (n % 2 != 0) {
        coeffs[n / 2] = coeffs[n / 2 - 1] * (n - n / 2) / (n / 2);
    }

    return coeffs;
}

void drawBezierCurve(const vector<Vertex<int>> &_points, Magick::Image &img, const Magick::Color &color) {
    size_t n = _points.size();
    auto coeffs = getCombCoeffs(n);
    vector<Vertex<double>> points(n);
    for (size_t i = 0; i < n; ++i) {
        points[i] = convertToDoubleVertex(_points[i]);
    }
    // sum(coeffs[i] * (1 - t) ^ (n - i) * t ^ i * points[i])
    Vertex<int> last = _points[0];
    for (double t = 0.0; t <= 1.0; t += 0.01) {
        Vertex<double> a = {0.0, 0.0};
        for (size_t i = 0; i < n; i++) {
            a += points[i] * (coeffs[i] * pow((1 - t), n - i - 1) * pow(t, i));
        }
        Vertex<int> cur(roundToInt(a.x), roundToInt(a.y));
        if ((cur - last).mod2() <= 3)
            continue;
        drawLine(last.x, last.y, cur.x, cur.y, img, color);
        last = cur;
//        img.pixelColor(cur.x, cur.y, color);
    }
    drawLine(last, _points.back(), img, color);
}


/// Реализуйте функцию, которая с помощью одной или нескольких кривых Безье 3-го
/// порядка строит дугу окружности. Функция получает в качестве параметров координаты
/// центра окружности, радиус окружности и значения двух углов, которые задают радиус-вектора от центра окружности до
/// крайних точек дуги. Дуга строится против часовой стрелки.
void drawCircleWithBezie(const Vertex<int> &center, int r, double phi1, double phi2,
                         Magick::Image &img, const Magick::Color &color, const Magick::Color &color2) {
    double step = M_PI / 4;
    while (phi1 < phi2) {
        double R = r / sin(M_PI / 2 - step / 2);
        double F = 4.0 / 3 / (1 + 1 / cos(step / 4));
        while (phi1 + step <= phi2 + 1e-2) {
            Vertex<int> p1 = center + Vertex{roundToInt(r * cos(phi1)), roundToInt(r * sin(phi1))};
            Vertex<int> p4 = center + Vertex{roundToInt(r * cos(phi1 + step)), roundToInt(r * sin(phi1 + step))};
            Vertex<int> pt =
                    center + Vertex{roundToInt(R * cos(phi1 + step / 2)), roundToInt(R * sin(phi1 + step / 2))};
            Vertex<int> p2 = p1 + (pt - p1) * F;
            Vertex<int> p3 = p4 + (pt - p4) * F;
            drawBezierCurve({p1, p2, p3, p4}, img, color);
            phi1 += step;
            img.pixelColor(p1.x, p1.y, color2);
            img.pixelColor(p2.x, p2.y, color2);
            img.pixelColor(p3.x, p3.y, color2);
            img.pixelColor(p4.x, p4.y, color2);
        }
        step = phi2 - phi1;
    }
}
