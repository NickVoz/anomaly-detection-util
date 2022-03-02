/**
 * timeseries.h
 *
 * Authors:
 * Shani Shafir
 * Nick Vozilov
 */

#ifndef TIMESERIES_H
#define TIMESERIES_H

#include <iostream>
#include <algorithm>
#include <vector>

class TimeSeries {
    const char* fileName;
    std::vector<std::vector<float>> dataVec;
    std::vector<std::string> colNames;

private:
    void readFile();
    std::vector<float> strToFloat(std::vector<std::string> currentLine);
public:
    TimeSeries(const char* CSVfileName);
    float getData(int col, int index);
    const std::vector<std::string> getNames() const;
    const std::vector<float> getDataCol(int col) const;
};
std::vector<std::string> split(std::string a, std::string del = ",");

float* vecToArr(std::vector<float>);

#endif //TIMESERIES_H
