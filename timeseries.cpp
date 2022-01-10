/**
 * timeseries.cpp
 *
 * Authors:
 * Shani Shafir, ID: 207071721
 * Nick Vozilov, ID: 318732252
 */

#include "timeseries.h"
#include <bits/stdc++.h>

//constructor method
TimeSeries::TimeSeries(const char* CSVfileName): fileName(CSVfileName) {
    readFile();
}

//reads data from file, transfers it to a two dimentinal vector.
void TimeSeries::readFile() {
    std::ifstream input(fileName);
    int iter = 1;
    bool flg = false;
    std::string currentLine;
    while (input) {
        input >> currentLine;
        if (iter == 1) {
            colNames = split(currentLine);
            // initialize the data array
            for (int i = 0; i < colNames.size(); i++) {
                dataVec.push_back(std::vector<float>());
            }
            iter = 0;
            continue;
        }
        auto lineData = strToFloat(split(currentLine));
        for (int i = 0; i < lineData.size(); i++) {
            dataVec[i].push_back(lineData[i]);
        }
    }
    input.close();
}
// Splits given string into a vector of substrings. Splits in according to the
// delimiter
std::vector<std::string> split(std::string a, std::string del) {
    std::vector<std::string> output;
    int start = 0;
    int end = a.find(del);
    while (end != -1) {
        auto currentWord = a.substr(start, end - start);
        output.push_back(currentWord);
        start = end + del.size();
        end = a.find(del, start);
        if(end == -1) {
            output.push_back(a.substr(start, a.length()));
        }
    }
    return output;
}
// Converts string vector to float vector
std::vector<float> TimeSeries::strToFloat(std::vector<std::string> currentLine) {
    std::vector<float> output;
    for (std::string x : currentLine) {
        output.push_back(std::stof(x));
    }
    return output;
}



/** Getter functions **/
// Returns the data in the specified column and row number.
float TimeSeries::getData(int col, int index) {
    return dataVec[col][index];
}
// returns a data column.
const std::vector<float> TimeSeries::getDataCol(int col) const {
    return dataVec[col];
}
// returns the name vector.
const std::vector<std::string> TimeSeries::getNames() const {
    return colNames;
}

float* vecToArr(std::vector<float> vec) {
    float *output = new float[vec.size()];
    int index = 0;
    for (float i: vec) {
        *(output + index) = i;
        index++;
    }
    return output;
}