#include "minCircle.h"
float pointDistance(const Point& a, const Point& b) {
    return sqrt(pow(a.x -b.x, 2) + pow(a.y - b.y, 2));
}
bool isInside(const Circle& c, const Point& p) {
    return pointDistance(c.center, p) <= c.radius;
}
// Calculates
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
Circle createCircle(const Point& a, const Point& b, const Point& c) {
    Point center = calcCenter(a, b, c);
    Circle output = Circle(center, pointDistance(center, a));
    return output;
}
Circle createCircle(const Point& a, const Point& b) {
    Point center = Point((a.x + b.x) / 2, (a.y + b.y) / 2);
    Circle output = Circle(center, pointDistance(center, a));
    return output;
}
bool isValidCircle(const Circle& c, const vector<Point>& pointVector) {
    for (const Point& p : pointVector) {
        if (!isInside(c, p)){
            return false;
        }
    }
    return true;
}
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

    // To check if MEC can be determined
    // by 2 points only
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {

            Circle c = createCircle(pArr[i], pArr[j]);
            if (isValidCircle(c, pArr))
                return c;
        }
    }
    return createCircle(pArr[0], pArr[1], pArr[2]);
}

Circle welzlAlg(vector<Point>& pArr, vector<Point> onCirc, int size) {
    // Base case when all points processed or |onCirc| = 3
    if (size == 0 || onCirc.size() == 3) {
        return calcTrivCircle(onCirc);
    }

    // Pick a random point randomly
    int idx = rand() % size;
    Point p = pArr[idx];

    // Put the picked point at the end of pArr
    // since it's more efficient than
    // deleting from the middle of the vector
    swap(pArr[idx], pArr[size - 1]);

    // Get the MEC circle d from the
    // set of points pArr - {p}
    Circle d = welzlAlg(pArr, onCirc, size - 1);

    // If d contains p, return d
    if (isInside(d, p)) {
        return d;
    }

    // Otherwise, must be on the boundary of the MEC
    onCirc.push_back(p);

    // Return the MEC for pArr - {p} and onCirc U {p}
    return welzlAlg(pArr, onCirc, size - 1);
}

Circle findMinCircle(Point** points, size_t size){
    vector<Point> pointVec(*points, *points + size); // converts array to vector
    random_shuffle(pointVec.begin(), pointVec.end());
    Circle output =  welzlAlg(pointVec, vector<Point>(), pointVec.size());
    return output;
}
