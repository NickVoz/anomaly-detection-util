

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    virtual void learnNormal(const TimeSeries& ts);
    virtual std::vector<AnomalyReport> detect(const TimeSeries& ts);
    void addCorrelation(const TimeSeries& ts, int i, int c, Circle circle, float correlationVal);

};
Circle getCircle(std::vector<float> a, std::vector<float> b);

#endif /* HYBRIDANOMALYDETECTOR_H_ */
