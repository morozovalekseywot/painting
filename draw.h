#pragma once

#include <iostream>
#include <cmath>
#include <Magick++.h>

using namespace std;

void drawLine(int x1, int y1, int x2, int y2, Magick::Image &img, const Magick::Color &col) {
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
    while (x1 != x2)
    {
        img.pixelColor(x1, y2, col);
        x1 += step_x;
    }
    while (y1 != y2){
        img.pixelColor(x2, y1, col);
        y1 += step_y;
    }
    img.pixelColor(x2, y2, col);
}