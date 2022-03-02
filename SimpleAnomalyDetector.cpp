/**
 * SimpleAnomalyDetector.cpp
 *
 * Authors:
 * Shani Shafir
 * Nick Vozilov
 */

#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
}

/**
  * Receives a TimeSeries object(derived from CSV file) and learns correlations between different values in the table.
  */
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    for (int i = 0; i < ts.getNames().size(); i++) {
        float *compared = vecToArr(ts.getDataCol(i));
        float m = threshold;
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

/**
  * Detects anomalies in a received TimeSeries object.
  */
std::vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
    std::vector<AnomalyReport> output;
    for(int i = 0; i < cf.size(); i++) {
        if (cf[i].flg) {
            continue;
        }
        std::string featureName1 = cf[i].feature1;
        std::string featureName2 = cf[i].feature2;
        int index1 = findIndex(featureName1, ts.getNames(), ts.getNames().size());
        int index2 = findIndex(featureName2, ts.getNames(), ts.getNames().size());
        std::vector<float> col1 = ts.getDataCol(index1);
        std::vector<float> col2 = ts.getDataCol(index2);
        Point** pointArr = colToPoint(col1, col2, col1.size());
        Line regFunc = cf[i].lin_reg;
        int c = -1;
        for (int j = 0; j < ts.getDataCol(0).size(); j++) {
            float yExpected = regFunc.f(pointArr[j]->x);
            float yResult = (*(pointArr + j))->y;
            float distance = abs(yExpected - yResult);
            if(distance > cf[i].threshold) {
                c = j + 1;
                std::string temp;
                temp.append(featureName1).append("-").append(featureName2);
                AnomalyReport anomaly(temp, c);
                output.push_back(anomaly);
            }
        }
        for (int i = 0; i < col1.size(); ++i) {
            delete[] *(pointArr + i);
        }
    }
    return output;
}

/* Auxiliary functions */
// adds a correlation to the correlation vector
void SimpleAnomalyDetector::addCorrelation(const TimeSeries& ts,
                                           int i, int c, float correlation) {
    correlatedFeatures correlated;
    correlated.feature1 = ts.getNames()[i];
    correlated.feature2 = ts.getNames()[c];
    correlated.corrlation = correlation;
    auto col1 = ts.getDataCol(i);
    auto col2 = ts.getDataCol(c);
    Point **pointsArr = colToPoint(col1, col2, col1.size());
    correlated.lin_reg = linear_reg(pointsArr, col1.size());
    correlated.threshold = calculateDist(pointsArr, correlated.lin_reg, col1.size());
    cf.push_back(correlated);
    for (int i = 0; i < col1.size(); ++i) {
        delete[] *(pointsArr + i);
    }
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
    return distance * 1.1;
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
    return abs(line.f(point.x) - point.y);
}

// finds the index of the desired string in a vector.
int findIndex(std::string searchFor, std::vector<std::string> stringVec, int size) {
    for (int i = 0; i < size; i++) {
        if (0 == stringVec[i].compare(searchFor)) {
            return i;
        }
    }
}
