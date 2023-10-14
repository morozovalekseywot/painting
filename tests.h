#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <cassert>
#include "polyhedron.h"
#include <Magick++.h>

template<class T>
void assertVectorsEqual(const vector<T> &a, const vector<T> &b){
    assert(std::equal(a.begin(), a.end(), b.begin()));
}

void TestGetCombCoeffs(){
    assertVectorsEqual(getCombCoeffs(1), {1});
    assertVectorsEqual(getCombCoeffs(2), {1, 1});
    assertVectorsEqual(getCombCoeffs(3), {1, 2, 1});
    assertVectorsEqual(getCombCoeffs(4), {1, 3, 3, 1});
}

void TestIsInsideSegment() {
    Segment<int> segm1 = {{0, 2},
                          {0, 5}};
    assert(isInsideSegment(segm1, {0, 3}) == true);
    assert(isInsideSegment(segm1, {0, -2}) == false);
    assert(isInsideSegment(segm1, {0, 7}) == false);

    Segment<int> segm2 = {{2, 0},
                          {5, 0}};
    assert(isInsideSegment(segm2, {3, 0}) == true);
    assert(isInsideSegment(segm2, {-2, 0}) == false);
    assert(isInsideSegment(segm2, {7, 0}) == false);
}

void TestIntersectSegment() {
    assert(intersectSegment({0, 0}, {5, 5}, {2, 3}, {3, 2}).first == true);

    // Parallel
    assert(intersectSegment({0, 0}, {0, 7}, {0, 6}, {0, 8}).first == true);
    assert(intersectSegment({0, 0}, {0, 5}, {0, 6}, {0, 8}).first == false);
    assert(intersectSegment({0, 0}, {5, 5}, {1, 1}, {7, 7}).first == true);
    assert(intersectSegment({5, 5}, {7, 5}, {8, 5}, {9, 5}).first == false);
    assert(intersectSegment({5, 5}, {7, 5}, {6, 5}, {9, 5}).first == true);

    assert(intersectSegment({0, 0}, {5, 5}, {1, 2}, {7, 8}).first == false);
    assert(intersectSegment({2, 2}, {5, 5}, {0, 5}, {3, 4}).first == false);
}

void TestIsConvex() {
    vector<Vertex<int>> points = {{50,  50},
                                  {100, 20},
                                  {150, 200},
                                  {300, 300},
                                  {350, 450},
                                  {200, 300}};
    Polyhedron pol(points);

    assert(pol.isConvex() == false);
    assert(pol.IsSimple() == true);

    std::reverse(points.begin(), points.end());
    Polyhedron pol2(points);

    assert(pol2.isConvex() == false);
    assert(pol2.IsSimple() == true);
}

void TestIsSimple() {
    vector<Vertex<int>> points = {{50,  50},
                                  {100, 20},
                                  {150, 200},
                                  {300, 300},
                                  {350, 450},
                                  {200, 300}};
    Polyhedron pol(points);

    assert(pol.IsSimple() == true);

    vector<Vertex<int>> points2 = {{50,  50},
                                  {100, 20},
                                  {50,  300},
                                  {300, 300},
                                  {350, 450},
                                  {200, 300}};
    Polyhedron pol2(points2);
    assert(pol2.IsSimple() == false);
}

void RunTests() {
    TestGetCombCoeffs();
    TestIsInsideSegment();
    TestIntersectSegment();
    TestIsConvex();
    TestIsSimple();
}