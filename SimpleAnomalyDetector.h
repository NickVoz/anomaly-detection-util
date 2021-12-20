/**
 * SimpleAnomalyDetector.h
 *
 * Authors:
 * Shani Shafir, ID: 207071721
 * Nick Vozilov, ID: 318732252
 */

#ifndef SIMPLEANOMALYDETECTOR_H
#define SIMPLEANOMALYDETECTOR_H

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string>
#include <string.h>
#include <math.h>
#include "minCircle.h"

struct correlatedFeatures {
    std::string feature1,feature2;  // names of the correlated features
    float corrlation = 0;
    Line lin_reg;
    float threshold;
    bool flg = false; // indicates type of correlation
    Circle corrCircle = Circle(Point(0, 0), 0);
};


class SimpleAnomalyDetector: public TimeSeriesAnomalyDetector {
protected:
    void addCorrelation(const TimeSeries& ts, int i, int c, float correlation);
    std::vector<correlatedFeatures> cf;
public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();

    virtual void learnNormal(const TimeSeries& ts);
    virtual std::vector<AnomalyReport> detect(const TimeSeries& ts);

    std::vector<correlatedFeatures> getNormalModel(){
        return cf;
    }

};

/* Auxiliary Functions */
Point** colToPoint(std::vector<float> feature1, std::vector<float> feature2, int size);
float distPointLine(Line a, Point b);
float calculateDist(Point **pPoint, Line line, int size);
int findIndex(std::string searchFor, std::vector<std::string> stringVec, int size);

#endif //SIMPLEANOMALYDETECTOR_H
