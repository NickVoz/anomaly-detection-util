/**
 * commands.h
 *
 * Authors:
 * Shani Shafir, ID: 207071721
 * Nick Vozilov, ID: 318732252
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

// Auxiliary functions


class Database {
public:
    string csv;
    HybridAnomalyDetector detector;
    std::vector<AnomalyReport> anomalies;
    std::vector<TimeSeries> tsArray;

};


class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	// you may add additional methods here
};
// you may add here helper classes
class StandardIO : public DefaultIO {
public:
    virtual void write(string text) override {
        std::cout << text << std::endl;
    }
    virtual string read() override {
        string output;
        std::cin >> output;
        return output;
    }
    virtual void write(float f) override {}
    virtual void read(float* f) override {}
};

// you may edit this class
class Command{
protected:
	DefaultIO* dio;
    Database *db;
public:
	Command(DefaultIO* dio, Database* db, std::string desc):dio(dio), db(db), description(desc){}
	virtual void execute()=0;
	virtual ~Command(){}
    std::string description;
    /*
 * returns all the time frames in the anomaly report
 */
    virtual std::vector<std::vector<long>> formatAnomalyTS(std::vector<AnomalyReport> anomalies) {
        std::vector<std::vector<long>> output;
        std::vector<long> timeFrame;
        timeFrame.push_back(anomalies[0].timeStep);
        long lastIndex = anomalies[0].timeStep;
        for (int i = 1; i < anomalies.size(); i++) {
            // next anomaly matches current anomaly's description
            if (anomalies[i].description == anomalies[i-1].description &&
                anomalies[i].timeStep == anomalies[i-1].timeStep + 1) {
                lastIndex = anomalies[i].timeStep;
            }
                // next anomaly does not match the previous one, either by time frame or by description
            else if(anomalies[i].description != anomalies[i-1].description ||
                    anomalies[i].timeStep != anomalies[i-1].timeStep + 1) {
                timeFrame.push_back(lastIndex);
                output.push_back(timeFrame);
                timeFrame = std::vector<long>();
                timeFrame.push_back(anomalies[i].timeStep);
                lastIndex = anomalies[i].timeStep;
            }
        }
        return output;
    }

/**
 * reads the expected anomaly time frames from external file
 * @param fileName path to file
 * @return vector containinig time frames
 */
    virtual std::vector<std::vector<long>> readExpectedAnomalies(std::string fileName) {
        std::ifstream input(fileName);
        std::string currentLine;
        std::vector<std::string> line;
        std::vector<std::vector<long>> output;
        while (input) {
            input >> currentLine;
            if (currentLine == "done") {
                break;
            }
            line = split(currentLine);
            std::vector<long> lineConverted;
            lineConverted.push_back(std::stoi(line[0]));
            lineConverted.push_back(std::stoi(line[1]));
            output.push_back(lineConverted);
        }
        return output;
    }
/**
 * Calculates true positive and false positive values based on received input.
 * @param expected expected anomalies
 * @param detected detected anomalies
 * @return vector. vec[0] = FP, vec[1] = TP
 */
    virtual std::vector<long> getFPTP(std::vector<std::vector<long>> expected, std::vector<std::vector<long>> detected) {
        long FP = 0;
        long TP = 0;
        std::vector<long> output;
        for (std::vector<long> i : detected) {
            bool flg = true;
            for (std::vector<long> j : expected) {
                if ((i[0] >=  j[0] && i[0] <= j[1]) || (i[1] >=  j[0] && i[1] <= j[1])) {
                    TP += 1;
                    flg = false;
                }
            }
            if (flg) {
                FP++;
            }
        }
        output.push_back(FP);
        output.push_back(TP);
        return output;
    }

};

// implement here your command classes
// Command 1
class TrainCSVCommand : public Command {
public:
    TrainCSVCommand(DefaultIO* dio, Database* db) : Command(dio, db, "1. upload a time series csv file"){};
    void execute() override {
        dio->write("Please upload your local train CSV file.");
        writeTo("train_file.csv");
        db->tsArray.push_back(TimeSeries("train_file.csv")); // train file will be at array[0]
        dio->write("Upload complete.");
        dio->write("Please upload your local test CSV file.");
        writeTo("test_file.csv");
        db->tsArray.push_back(TimeSeries("test_file.csv"));
        dio->write("Upload complete.");
    }
    void writeTo(std::string path) {
        std::ofstream out;
        out.open(path);
        std::string line = dio->read();
        while (line != "done") {
            out << line + '\n';
            line = dio->read();
        }
        out.close();
    }
};
// Command 2
class ChangeThresholdCommand : public Command {
public:
    ChangeThresholdCommand(DefaultIO* dio, Database* db) : Command(dio, db, "2. algorithm settings"){};
    void execute() override {
        std::string output = "The current correlation threshold is ";
        output += to_string(db->detector.threshold);
        dio->write(output);
        float choice;
        // This loop runs until the user enters a valid input.
        while(true){
            std::string input = dio->read();
            // try converting string to float
            try {
                choice = std::stof(input); // convert users' choice to float
                if (choice < 1 && choice > 0) {
                    break;
                }
                else {
                    dio->write("please choose a value between 0 and 1.");
                    continue;
                }
            }
                // user entered value that cannot be converted to float - continue loop
            catch (const std::invalid_argument) {
                dio->write("please choose a value between 0 and 1.");
                continue;
            }
        }
        db->detector.threshold = choice;

    }

};

// Command 3
class TestCSVCommand : public Command {
public:
    TestCSVCommand(DefaultIO* dio, Database* db) : Command(dio, db, "3. detect anomalies"){};
    void execute() override {
        db->detector.learnNormal(db->tsArray[0]);
        db->anomalies = db->detector.detect(db->tsArray[1]);
        dio->write("anomaly detection complete.");
    }
};

// Command 4
class DisplayResultsCommand : public Command {
public:
    DisplayResultsCommand(DefaultIO *dio, Database *db) : Command(dio, db, "4. display results") {};
    void execute() override {
        std::string output;
        for (AnomalyReport i : db->anomalies) {
            output += i.timeStep;
            output += "\t";
            output += i.description;
            dio->write(output);
        }
        dio->write("Done.");
    }
};
// Command 5
class AnalyzeResultsCommand : public Command {
public:
    AnalyzeResultsCommand(DefaultIO *dio, Database *db) :
    Command(dio, db, "5. upload anomalies and analyze results") {};
    void execute() override {
        dio->write("Please upload your local anomalies file.");
        std::string path = dio->read();
        dio->write("Upload complete.");
        std::vector<std::vector<long>> expected = readExpectedAnomalies(path);
        auto detected = formatAnomalyTS(db->anomalies);
        std::vector<long> results = getFPTP(expected, detected);
        long p = expected.size();
        long N = db->tsArray[1].getDataCol(0).size();
        for (std::vector<long> i : expected) {
            N -= i[1] - i[0] + 1;
        }
        double tpRate = ((double) results[1]) / ((double) p);
        tpRate *= 1000;
        tpRate = floor(tpRate);
        tpRate /= 1000;
        double fpRate = ((double) results[0]) / ((double) N);
        fpRate *= 1000;
        fpRate = floor(tpRate);
        fpRate /= 1000;
        std::string outputString = "True Positive Rate: ";
        outputString += tpRate;
        dio->write(outputString);
        outputString = "False Positive Rate: ";
        outputString +=  fpRate;
        dio->write(outputString);
    }
};


#endif /* COMMANDS_H_ */
