/**
 * minCircle.cpp
 *
 * Authors:
 * Shani Shafir, ID: 207071721
 * Nick Vozilov, ID: 318732252
 */

#include "minCircle.h"

// Calculates distance between two points
float pointDistance(const Point& a, const Point& b) {
    return sqrt(pow((a.x -b.x), 2) + pow((a.y - b.y), 2));
}
// Checks if a point is inside of a circle
bool isInside(const Circle& c, const Point& p) {
    return pointDistance(c.center, p) <= c.radius;
}
// Calculates the center of a circle formed by 3 different points.
Point calcCenter(const Point& a, const Point& b, const Point& c) {
    float x = (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
    float y = (c.x - a.x) * (c.x - a.x) + (c.y - a.y) * (c.y - a.y);
    float z = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    Point output = Point(
            ((c.y - a.y) * x - (b.y - a.y) * y) / (2 * z) + a.x,
            ((b.x - a.x) * y - (c.x - a.x) * x) / (2 * z) + a.y
    );
    return output;
}
// Generates a circle object using given 3 points
Circle createCircle(const Point& a, const Point& b, const Point& c) {
    Point center = calcCenter(a, b, c);
    Circle output = Circle(center, pointDistance(center, a));
    return output;
}
// Generates a circle object using two points
Circle createCircle(const Point& a, const Point& b) {
    Point center = Point((a.x + b.x) / 2, (a.y + b.y) / 2);
    Circle output = Circle(center, pointDistance(center, a));
    return output;
}
// checks whether each point in a given vector of points is contained inside the circle.
bool isInsideCircle(const Circle& c, const vector<Point>& pointVector) {
    for (const Point& p : pointVector) {
        if (!isInside(c, p)){
            return false;
        }
    }
    return true;
}
// Calculates circle based on received point vector.
Circle calcTrivCircle(const vector<Point>& pArr) {
    if (pArr.empty()) {
        return Circle(Point(0, 0), 0);
    }
    else if (pArr.size() == 1) {
        return Circle(pArr[0], 0 );
    }
    else if (pArr.size() == 2) {
        return createCircle(pArr[0], pArr[1]);
    }
    return createCircle(pArr[0], pArr[1], pArr[2]);
}

Circle welzlAlg(Point** points, vector<Point> onCirc, int size) {
    // Base case when all points processed or |onCirc| = 3
    if (size == 0 || onCirc.size() == 3) {
        return calcTrivCircle(onCirc);
    }

    // Pick the last point as the excluded point
    Point p = *(*(points + size - 1));


    // Get the minimal circle from the array of points, excluding removed point
    Circle d = welzlAlg(points, onCirc, size - 1);

    // Check if returned circle contains the removed point after getting back from recursion
    if (isInside(d, p)) {
        return d;
    }

    // If the program reaches this place, means that the Point is outside the returned circle, add it to the vector of
    // circumference points.
    onCirc.push_back(p);

    // Return the minimal circle for the remaining points without this one, and this point as a circumference point.
    return welzlAlg(points, onCirc, size - 1);
}
// Invokes the recursive Welzl algorithm function.
Circle findMinCircle(Point** points, size_t size){
    return welzlAlg(points, vector<Point>(), size);
}
