
#include "HybridAnomalyDetector.h"
#define MAX 0.9
#define MIN 0.5

HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

void HybridAnomalyDetector::learnNormal(const TimeSeries& ts) {
    // Initialize correlatedFeatures vector with all proper correlations(>0.9)
    SimpleAnomalyDetector::learnNormal(ts);
    //
    for (int i = 0; i < ts.getNames().size(); i++) {
        float *compared = vecToArr(ts.getDataCol(i));
        int c = -1;
        float m = MIN;
        for (int j = i + 1; j < ts.getNames().size(); j++) {
            float *compTo = vecToArr(ts.getDataCol(j));
            float pearsonVal = abs(pearson(compared, compTo, ts.getDataCol(i).size()));
            if (pearsonVal < MAX && pearsonVal > m) {
                c = j;
                m = pearsonVal;
            }
            // check if current pearson value exceeds maximal threshold, if so, exit the loop
            else if (pearsonVal > MAX) {
                delete[] compTo;
                c = -1;
                break;
            }
            delete[] compTo;

        }
        if (c != -1) {
            Circle corrCircle = getCircle(ts.getDataCol(i), ts.getDataCol(c));
            addCorrelation(ts, i, c, corrCircle, m);
        }
        delete[] compared;
    }

}
std::vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries& ts) {
    auto output = SimpleAnomalyDetector::detect(ts);
    for(int i = 0; i < cf.size(); i++) {
        if (!cf[i].flg) {
            continue;
        }
        std::string featureName1 = cf[i].feature1;
        std::string featureName2 = cf[i].feature2;
        int index1 = findIndex(featureName1, ts.getNames(), ts.getNames().size());
        int index2 = findIndex(featureName2, ts.getNames(), ts.getNames().size());
        std::vector<float> col1 = ts.getDataCol(index1);
        std::vector<float> col2 = ts.getDataCol(index2);
        Point** pointArr = colToPoint(col1, col2, col1.size());
        int c = -1;
        for (int j = 0; j < col1.size(); ++j) {
            if (!isInside(cf[i].corrCircle, **(pointArr + j))) {
                c = j + 1;
            }
        }
        if (c != -1) {
            std::string temp;
            temp.append(featureName1).append("-").append(featureName2);
            AnomalyReport anomaly(temp, c);
            output.push_back(anomaly);
        }
        for (int i = 0; i < col1.size(); ++i) {
            delete[] *(pointArr + i);
        }
    }
    return output;
}
void HybridAnomalyDetector::addCorrelation(const TimeSeries& ts, int i, int c, Circle circle, float correlationVal) {
    correlatedFeatures correlated;
    correlated.feature1 = ts.getNames()[i];
    correlated.feature2 = ts.getNames()[c];
    correlated.corrlation = correlationVal;
    correlated.corrCircle = Circle(circle.center, circle.radius * 1.1);
    correlated.flg = true;
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

Circle getCircle(std::vector<float> a, std::vector<float> b) {
    Point** points = colToPoint(a, b, a.size());
    Circle output = findMinCircle(points, a.size());
    // delete points array
    for (int i = 0; i < a.size(); ++i) {
        delete[] *(points + i);
    }
    return output;
}
