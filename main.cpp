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
    pol1.FillWithNonZeroWinding(img, Blue);
    pol1.DrawBounds(img, Black);

    pol2.FillWithEvenOddRule(img, Blue);
    pol2.DrawBounds(img, Black);

    cout << "isConvex: " << pol1.isConvex() << '\n';
    cout << "IsSimple: " << pol1.IsSimple() << '\n';

    saveImg(img, "line.png");
}


int main() {
    RunTests();

    draw1();
    return 0;
}