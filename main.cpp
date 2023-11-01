#define MAGICKCORE_QUANTUM_DEPTH 16
#define MAGICKCORE_HDRI_ENABLE 1

#include <iostream>
#include <utility>
#include <vector>
#include "polyhedron.h"
#include <Magick++.h>
#include "tests.h"
#include "kuboid.h"

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
                                  {460, 350},
                                  {100, 350},
                                  {400, 200},
                                  {250, 460}};

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

void testShowProjection() {
    Magick::Image img("500x500", "white");

    vector<Vertex<int>> low_points = {
            {10,  10,  10},
            {10,  300, 10},
            {300, 300, 10},
            {300, 10,  10},
    };

    vector<Vertex<int>> high_points(low_points);
    for (auto &v: high_points) {
        v.z = 20;
        v.x += 100;
        v.y += 100;
    }

    vector<Segment<int>> segments(12);
    for (int i = 0; i < 4; i++) {
        segments[i] = Segment<int>(get(low_points, i), get(low_points, i + 1));
        segments[i + 4] = Segment<int>(get(high_points, i), get(high_points, i + 1));
        segments[i + 8] = Segment<int>(low_points[i], high_points[i]);
    }

//    showProjection(segments, 20, img, Black);
    for (auto &segm: segments)
        segm.draw(img, Black);

    saveImg(img, "projection.png");
}

void testOnePointProjection() {
    Magick::Image img("800x800", "white");

    int a = 300;
    int min_x = 200, min_y = 200, min_z = 100, max_z = 200;
    vector<Vertex<int>> low_points = {
            {min_x,     min_y,     min_z},
            {min_x,     min_y + a, min_z},
            {min_x + a, min_y + a, min_z},
            {min_x + a, min_y,     min_z},
    };

    vector<Vertex<int>> high_points(low_points);
    for (auto &v: high_points) {
        v.z = max_z;
    }

    array<array<Vertex<int>, 4>, 6> faces;
    faces[4] = {low_points[0], low_points[1], low_points[2], low_points[3]};
    faces[5] = {high_points[0], high_points[1], high_points[2], high_points[3]};
    for (int i = 0; i < 4; i++) {
        faces[i] = {low_points[i], low_points[(i + 1) % 4], high_points[(i + 1) % 4], high_points[i]};
    }

    Kuboid kuboid(faces);
//    kuboid.rotate(-M_PI / 4, M_PI / 4, 0, kuboid.getCenter());
    kuboid.rotate(M_PI_4, 0, 0, kuboid.getCenter());
//    kuboid.drawBounds(img, Black);
//    kuboid.show(img, Red);
    kuboid.onePointProjection(1.5e-3, img, Blue);

    saveImg(img, "one_point_projection.png");
}

void testKuboid() {
    Magick::Image img("700x700", "white");

    vector<Vertex<int>> low_points = {
            {400, 400, 40},
            {400, 600, 40},
            {600, 600, 40},
            {600, 400, 40},
    };

    vector<Vertex<int>> high_points(low_points);
    for (auto &v: high_points) {
        v.z = 100;
    }

    array<array<Vertex<int>, 4>, 6> faces;
    faces[4] = {low_points[0], low_points[1], low_points[2], low_points[3]};
    faces[5] = {high_points[0], high_points[1], high_points[2], high_points[3]};
    for (int i = 0; i < 4; i++) {
        faces[i] = {low_points[i], low_points[(i + 1) % 4], high_points[(i + 1) % 4], high_points[i]};
    }

    Kuboid kuboid(faces);
    kuboid.rotate(M_PI / 8, M_PI / 4, 0, kuboid.getCenter());
//    kuboid.drawBounds(img, Black);
    kuboid.show(img, Blue);

    saveImg(img, "projection.png");
}

void plotAnimation() {
    int a = 300;
    int min_x = 200, min_y = 200, min_z = 100, max_z = 200;
    vector<Vertex<int>> low_points = {
            {min_x,     min_y,     min_z},
            {min_x + a, min_y,     min_z},
            {min_x + a, min_y + a, min_z},
            {min_x,     min_y + a, min_z},

    };

    vector<Vertex<int>> high_points(low_points);
    for (auto &v: high_points) {
        v.z = max_z;
    }

    array<array<Vertex<int>, 4>, 6> faces;
    faces[4] = {low_points[0], low_points[1], low_points[2], low_points[3]};
    faces[5] = {high_points[0], high_points[1], high_points[2], high_points[3]};
    for (int i = 0; i < 4; i++) {
        faces[i] = {low_points[i], low_points[(i + 1) % 4], high_points[(i + 1) % 4], high_points[i]};
    }

    Kuboid kuboid(faces);
    kuboid.rotate(M_PI / 2, 0, 0, kuboid.getCenter());

    int N = 200;
    vector<Magick::Image> frames(N, Magick::Image("700x700", "white"));
    auto center = kuboid.getCenter();
    for (int i = 0; i < N; i++) {
        kuboid.rotate(0, 2 * M_PI / N, 0, center);
        kuboid.onePointProjection(1.3e-3, frames[i], Blue);
    }

    for(auto &frame: frames)
    {
        frame.flip();
        frame.animationDelay(1);
    }

    Magick::writeImages(frames.begin(), frames.end(), "../images/anim.gif");
}

void testWeilerAtherton1() {
    Magick::Image img("300x300", "white");

    vector<Vertex<int>> points = {{20,  20},
                                  {280, 280},
                                  {20,  280},
                                  {280, 20}};
    Polyhedron pol(points);
    pol.drawBounds(img, Blue);

    auto ans = pol.weilerAtherton();
    ans.drawBounds(img, Red);

    saveImg(img, "WeilerAtherton1.png");
}

void testWeilerAtherton2() {
    Magick::Image img("500x500", "white");

    Polyhedron pol = create_star();
    pol.drawBounds(img, Blue);

    auto ans = pol.weilerAtherton();
    ans.drawBounds(img, Red);

    saveImg(img, "WeilerAtherton2.png");
}

void testWeilerAtherton3() {
    Magick::Image img("500x500", "white");

    vector<Vertex<int>> points = {{20,  100}, // A
                                  {170, 170}, // B
                                  {320, 100}, // C
                                  {350, 170}, // D
                                  {210, 90}, // E
                                  {30,  250}, // F
                                  {300, 250}, // G
                                  {100, 50}, // K
    };
    Polyhedron pol(points);
    pol.drawBounds(img, Blue);

    auto ans = pol.weilerAtherton();
    ans.drawBounds(img, Red);

    saveImg(img, "WeilerAtherton3.png");
}

int main() {
    RunTests();
//    draw1();
//    drawBezie();
//    drawClip();
//    testDrawLine();
//    drawCircle();
//    testShowProjection();
    testKuboid();
//    testWeilerAtherton1();
//    testWeilerAtherton2();
//    testWeilerAtherton3();
//    testOnePointProjection();
//    plotAnimation();
    return 0;
}