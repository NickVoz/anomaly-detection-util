/**
 * AnomalyDetector.h
 *
 * Authors:
 * Shani Shafir, ID: 207071721
 * Nick Vozilov, ID: 318732252
 */

#ifndef ANOMALYDETECTOR_HileName(CSVfileName
#define ANOMALYDETECTOR_H

#include <string>
#include <vector>
#include "timeseries.h"
#include "math.h"

class AnomalyReport {
public:
    const std::string description;
    const long timeStep;
    AnomalyReport(std::string description, long timeStep) : description(description), timeStep(timeStep) {}
};

class TimeSeriesAnomalyDetector {
public:
    virtual void learnNormal(const TimeSeries& ts) = 0;
    virtual std::vector<AnomalyReport> detect(const TimeSeries& ts)=0;
    // destructor
    virtual ~TimeSeriesAnomalyDetector(){}
};

#endif //ANOMALYDETECTOR_H
