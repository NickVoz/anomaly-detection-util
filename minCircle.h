
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include "anomaly_detection_util.h"

using namespace std;



class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
};

float pointDistance(const Point& a, const Point& b);

bool isInside(const Circle& c, const Point& p);
Point calcCenter(const Point& a, const Point& b, const Point& c);
Circle findMinCircle(Point** points, size_t size);
Circle createCircle(const Point& a, const Point& b, const Point& c);
Circle createCircle(const Point& a, const Point& b);
bool isValidCircle(const Circle& c, const vector<Point>& pointVector);
Circle calcTrivCircle(const vector<Point>& pArr);
Circle welzlAlg(vector<Point>& pArr, vector<Point> onCirc, int size);
float pointDistance(const Point& a, const Point& b);

// you can add here additional methods

#endif /* MINCIRCLE_H_ */
