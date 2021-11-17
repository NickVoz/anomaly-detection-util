/**
 * SimpleAnomalyDetector.cpp
 *
 * Authors:
 * Shani Shafir, ID: 207071721
 * Nick Vozilov, ID: 318732252
 */

#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    // TODO Auto-generated constructor stub

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    // TODO Auto-generated destructor stub
}


void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    for (int i = 0; i < ts.getNames().size(); i++) {
        float *compared = vecToArr(ts.getDataCol(i));
        float m = 0.9;
        int c = -1;
        for (int j = i + 1; j < ts.getNames().size(); j++) {
            float *compTo = vecToArr(ts.getDataCol(j));
            float pearsonVal = abs(pearson(compared, compTo, ts.getDataCol(i).size()));
            if (pearsonVal > m) {
                c = j;
                m = pearsonVal;
            }
            delete[] compTo;
        }
        if (c != -1) {
            addCorrelation(ts, i, c, m);
        }
        delete[] compared;
    }
}

std::vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){

}

/* Auxiliary functions */
// adds a correlation to the correlation vector
void SimpleAnomalyDetector::addCorrelation(const TimeSeries& ts, int i, int c, float correlation) {
    correlatedFeatures correlated;
    correlated.feature1 = ts.getNames()[i];
    correlated.feature2 = ts.getNames()[c];
    correlated.correlation = correlation;
    auto col1 = ts.getDataCol(i);
    auto col2 = ts.getDataCol(c);
    Point** pointsArr = colToPoint(col1, col2, col1.size());
    correlated.lin_reg = linear_reg(pointsArr, col1.size());
    correlated.threshold = calculateDist(pointsArr, correlated.lin_reg, col1.size());
    cf.push_back(correlated);
    delete[] pointsArr;
}

// calculates the maximal distance between the correlated Point array and the regression line.
float calculateDist(Point **pPoint, Line line, int size) {
    float distance = 0;
    for (int i = 0; i < size; i++) {
        float temp = distPointLine(line, **(pPoint + i));
        if (temp > distance) {
            distance = temp;
        }
    }
    return float(distance * 1.1);
}

// converts two float vectors into Point pairs, returns an array
Point** colToPoint(std::vector<float> feature1, std::vector<float> feature2, int size) {
    Point** output = new Point*[size];
    for(int i = 0; i < size; i++) {
        *(output + i) = new Point(feature1[i], feature2[i]);
    }
    return output;
}


// Calculates the distance between a given line and a point.
float distPointLine(Line line, Point point) {
    // find perpendicular line.
    float a = -1 / (line.a);
    float b = point.y - a * point.x;
    // find intersection.
    float xInter = (b - line.b) / (line.a - a);
    float yInter = line.f(xInter);
    // calculate distance.
    float xSub = xInter - point.x;
    float ySub = yInter - point.y;
    return sqrt(pow(xSub, 2) + pow(ySub, 2));
}
