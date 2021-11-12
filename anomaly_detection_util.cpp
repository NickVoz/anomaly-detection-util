/**
 * anomaly_detection_util.cpp
 *
 * Authors:
 * Shani Shafir, ID: 207071721
 * Nick Vozilov, ID: 318732252
 */

#include <iostream>
#include <cmath>
#include "anomaly_detection_util.h"

// Auxiliary functions
// calculates the mean of a given variable
float mean(float* x, int size) {
    float result = 0;
    for (int i = 0; i < size; ++i) {
        result += *(x + i);
    }
    return result/size;
}
// calculates absolute value
float absVal(float x) {
    if (x < 0) {
        return -x;
    }
    return x;
}
/** Primary Functions **/
// returns the variance of the variable X
float var(float* x, int size) {
    float result = 0;
    float meanSq = mean(x, size) * mean(x, size);
    for (int i = 0; i < size; ++i) {
        result += pow(*(x + i), 2);
    }
    return result/size - meanSq;
}
// return the covariance of variables X and Y
float cov(float* x, float* y, int size){
    float result = 0;
    float meanX =  mean(x, size);
    float meanY = mean(y, size);
    for (int i = 0; i < size; ++i) {
        result += (*(x + i) - meanX) * (*(y + i) - meanY);
    }
    return result / size;
}
// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size) {
    float sDivX = sqrt(var(x, size));
    float sDivY = sqrt(var(y, size));
    return cov(x, y, size) / (sDivX * sDivY);
}
// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size) {
    float a = 0;
    float b = 0;
    // arrays for x and y values of points
    float* xArr = new float[size];
    float* yArr = new float[size];
    for (int i = 0; i < size; ++i) {
        xArr[i] = (*(*(points + i))).x;
        yArr[i] = (*(*(points + i))).y;
    }
    a = cov(xArr, yArr, size) / var(xArr, size);
    b = mean(yArr, size) - a * mean(xArr, size);
    delete[](xArr);
    delete[](yArr);
    return Line(a, b);
}
// returns the deviation between point p and the line equation of the points
float dev(Point p, Point** points, int size) {
    Line regResult = linear_reg(points, size);
    return dev(p, regResult);
}
// returns the deviation between point p and the line
float dev(Point p, Line l) {
    return absVal(l.f(p.x) - p.y);
}