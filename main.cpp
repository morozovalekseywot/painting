#define MAGICKCORE_QUANTUM_DEPTH 16
#define MAGICKCORE_HDRI_ENABLE 1

#include <iostream>
#include <utility>
#include <vector>
#include "polyhedron.h"
#include <Magick++.h>
#include "tests.h"

const int DEPTH = (2 << MAGICKCORE_QUANTUM_DEPTH) - 1;

using namespace std;
using namespace Magick;

const Color Yellow(DEPTH, DEPTH, DEPTH / 2.5);
const Color Red(DEPTH, 0, 0, 0);
const Color Green(0, DEPTH, 0, 0);
const Color Blue(0, 0, DEPTH, 0);
const Color Black(0, 0, 0);
const Color White(DEPTH, DEPTH, DEPTH);
const Color Orange(DEPTH, 0.3 * DEPTH, 0);


void saveImg(Magick::Image &img, const string &filename) {
    img.flip();
    img.magick("png");
    if (filename.ends_with(".png"))
        img.write("../images/" + filename);
    else
        img.write("../images/" + filename + ".png");
}

Polyhedron create_star() {
    vector<Vertex<int>> points = {{150, 200},
                                  {450, 350},
                                  {100, 350},
                                  {400, 200},
                                  {250, 450}};

    return Polyhedron(points);
}

Polyhedron create_convex() {
    vector<Vertex<int>> points = {{50,  50},
                                  {100, 20},
                                  {250, 200},
                                  {300, 300},
                                  {350, 450},
                                  {200, 300}};
    return Polyhedron(points);
}

void draw1() {
    Magick::Image img("500x500", "white");
    Polyhedron pol1 = create_star();
    pol1.move({-100, -200});
    pol1.scale(0.6);
    Polyhedron pol2 = create_star();
    pol2.scale(0.6);
    pol1.fillWithNonZeroWinding(img, Blue);
    pol1.drawBounds(img, Black);

    pol2.fillWithEvenOddRule(img, Blue);
    pol2.drawBounds(img, Black);

    cout << "isConvex: " << pol1.isConvex() << '\n';
    cout << "IsSimple: " << pol1.IsSimple() << '\n';

    saveImg(img, "line.png");
}

void drawBezie() {
    Magick::Image img("500x500", "white");
    drawBezierCurve({
                            {200, 400},
                            {500, 200},
                            {100, 200},
                            {400, 400}}, img, Black);
    saveImg(img, "bezie_line.png");
}

void drawClip() {
    Magick::Image img("2560x1440", "white");
    Polyhedron pol = create_convex();
    pol.move({1000, 500});
    pol.scale(3);
    pol.drawBounds(img, Blue);

    Segment<int> line1({100, 200}, {2400, 1400});
    auto ans1 = cyrusBeckClipLine(line1, pol);
    line1.draw(img, Orange);
    ans1.draw(img, Red);

    Segment<int> line2({100, 1300}, {2300, 300});
    auto ans2 = cyrusBeckClipLine(line2, pol);
    line2.draw(img, Orange);
    ans2.draw(img, Red);

    Segment<int> line3({1000, 400}, {1200, 600});
    auto ans3 = cyrusBeckClipLine(line3, pol);
    line3.draw(img, Orange);
    ans3.draw(img, Red);

    Segment<int> line4({2000, 700}, {1300, 900});
    auto ans4 = cyrusBeckClipLine(line4, pol);
    line4.draw(img, Orange);
    ans4.draw(img, Red);

    Segment<int> line5({300, 700}, {800, 900});
    auto ans5 = cyrusBeckClipLine(line5, pol);
    line5.draw(img, Orange);
    ans5.draw(img, Red);

    saveImg(img, "clip_line.png");
}

void drawCircle() {
    Magick::Image img("500x500", "white");
    drawCircleWithBezie({250, 250}, 100, 0, 6 * M_PI / 5, img, Black, Red);
    saveImg(img, "circle.png");
}

void testDrawLine() {
    Magick::Image img("10x10", "white");
    drawLine(0, 0, 8, 3, img, Black);
    drawLine(8, 3, 0, 0, img, Blue);
    saveImg(img, "test_line1.png");

    Magick::Image img2("10x10", "white");
    drawLine(0, 0, 3, 8, img2, Black);
    drawLine(3, 8, 0, 0, img2, Green);
    saveImg(img2, "test_line2.png");

    Magick::Image img3("10x10", "white");
    drawLine(9, 0, 3, 8, img3, Black);
    drawLine(3, 8, 9, 0, img3, Red);
    saveImg(img3, "test_line3.png");
}

int main() {
    RunTests();
//    draw1();
//    drawBezie();
//    drawClip();
    testDrawLine();
//    drawCircle();
    return 0;
}