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


void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    for (int i = 0; i < ts.getNames().size(); i++){
        float *compared = vecToArr(ts.getDataCol(i));
        float m = 0;
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
            std::vector<std::string> vecCor;
            vecCor.push_back(ts.getNames()[i]);
            vecCor.push_back(ts.getNames()[c]);
            ts.addCorrelation(vecCor);
        }
        delete[] compared;
    }
}

std::vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    // TODO Auto-generated destructor stub
}
