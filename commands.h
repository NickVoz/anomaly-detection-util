/**
 * commands.h
 *
 * Authors:
 * Shani Shafir
 * Nick Vozilov
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <cstring>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

// Auxiliary functions

// Shared Database class, will serve as a shared data structure between all Commands
class Database {
public:
    HybridAnomalyDetector detector;
    std::vector<AnomalyReport> anomalies;
    std::vector<TimeSeries> tsArray;
    bool stopFlg = true;

};


class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}
};


class StandardIO : public DefaultIO {
public:
    std::string read() override {
        std::string str;
        std::cin >> str;
        return str;
    }
    void write(string text) override {
        std::cout << text;
    }
    void write(float f) override { std::cout << f; }
    void read(float* f) override { std::cin >> *f; }
};

/**
  * Command class, utilising the Command design pattern.
  * Receives an IO object and a pointer to the shared database.
  * Different commands will derive from this class with various functionality.
  */
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
    * groups up all adjacent TimeSteps into a single time frame, returns all time frames in the AnomalyReport.
    */
    virtual std::vector<std::vector<long>> formatAnomalyTS(std::vector<AnomalyReport> anomalies) {
        std::vector<std::vector<long>> output;
        std::vector<long> timeFrame;
        timeFrame.push_back(anomalies[0].timeStep);
        long lastIndex = anomalies[0].timeStep;
        bool flg = false;
        for (int i = 1; i < anomalies.size(); i++) {
            // next anomaly matches current anomaly's description
            if (anomalies[i].description == anomalies[i-1].description &&
                anomalies[i].timeStep == anomalies[i-1].timeStep + 1) {
                lastIndex = anomalies[i].timeStep;
                flg = true;
            }
                // next anomaly does not match the previous one, either by time frame or by description
            else if(anomalies[i].description != anomalies[i-1].description ||
                    anomalies[i].timeStep != anomalies[i-1].timeStep + 1) {
                timeFrame.push_back(lastIndex);
                output.push_back(timeFrame);
                timeFrame = std::vector<long>();
                timeFrame.push_back(anomalies[i].timeStep);
                lastIndex = anomalies[i].timeStep;
                flg = false;
            }
        }
        // There was an orphan anomaly which did not terminate in a mismatching pair.
        if (flg) {
            timeFrame.push_back(lastIndex);
            output.push_back(timeFrame);
        }
        return output;
    }

};

// Command 1 - Recieves a .CSV file data from user, input is received line by line(via either local command line or through a socket)
// Performs the TrainCSV method
class TrainCSVCommand : public Command {
public:
    TrainCSVCommand(DefaultIO* dio, Database* db) : Command(dio, db, "1.upload a time series csv file\n"){};
    void execute() override {
        dio->write("Please upload your local train CSV file.\n");
        writeTo("trainFile.csv");
        db->tsArray.push_back(TimeSeries("trainFile.csv")); // train file will be at array[0]
        dio->write("Upload complete.\n");
        dio->write("Please upload your local test CSV file.\n");
        writeTo("testFile.csv");
        db->tsArray.push_back(TimeSeries("testFile.csv"));
        dio->write("Upload complete.\n");
    }
    // Receives a path and writes lines to it.
    void writeTo(char* path) {
        std::ofstream out;
        out.open(path);
        std::string line = dio->read();
        while (line != "done") {
            out << line;
            line = dio->read();
            if (line == "done") {
                break;
            }
            out << "\n";
        }

    }
};
// Command 2 - changes the threshold of the Detection algorithm.
class ChangeThresholdCommand : public Command {
public:
    ChangeThresholdCommand(DefaultIO* dio, Database* db) : Command(dio, db, "2.algorithm settings\n"){};
    void execute() override {
        std::string output = "The current correlation threshold is ";
        output += to_string(db->detector.threshold);
        output += "\n";
        dio->write(output);
        dio->write("Type a new threshold\n");
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
                    dio->write("please choose a value between 0 and 1.\n");
                    continue;
                }
            }
            // user entered value that cannot be converted to float - continue loop
            catch (const std::invalid_argument) {
                dio->write("please choose a value between 0 and 1.\n");
                continue;
            }
        }
        db->detector.threshold = choice;
    }
};

// Command 3 - Perform anomaly detection
class TestCSVCommand : public Command {
public:
    TestCSVCommand(DefaultIO* dio, Database* db) : Command(dio, db, "3.detect anomalies\n"){};
    void execute() override {
        db->detector.learnNormal(db->tsArray[0]);
        db->anomalies = db->detector.detect(db->tsArray[1]);
        dio->write("anomaly detection complete.\n");
    }
};

// Command 4 - Display the anomaly detection results
class DisplayResultsCommand : public Command {
public:
    DisplayResultsCommand(DefaultIO *dio, Database *db) : Command(dio, db, "4.display results\n") {};
    void execute() override {
        for (AnomalyReport i : db->anomalies) {
            std::string output;
            output += to_string(i.timeStep);
            output += "\t";
            output += i.description;
            output += "\n";
            dio->write(output);
        }
        dio->write("Done.\n");
    }
};
// Command 5 - Analyzes the results, compares expected
class AnalyzeResultsCommand : public Command {
public:
    AnalyzeResultsCommand(DefaultIO *dio, Database *db) :
    Command(dio, db, "5.upload anomalies and analyze results\n") {};
    static bool comparator(const std::vector<long>& a, const std::vector<long>& b) {
        return (a.at(0) < b.at(0));
    }
    void execute() override {
        // initialize positive and negative fields.
        int P = 0;
        int N = db->tsArray.at(1).getDataCol(0).size() - 1;

        dio->write("Please upload your local anomalies file.\n");
        std::vector<std::vector<long>> expected;

        std::string line;
        line = dio->read();
        while (line != "done") {
            std::vector<long> sub;
            std::vector<std::string> splitLine = split(line);
            sub.push_back(stol(splitLine.at(0)));
            sub.push_back(stol(splitLine.at(1)));
            expected.push_back(sub);
            P++;
            line = dio->read();
        }
        dio->write("Upload complete.\n");
        sort(expected.begin(), expected.end(), comparator);
        auto detected = formatAnomalyTS(db->anomalies);


        sort(detected.begin(), detected.end(), comparator);
        auto numOfAnomalies = expected.size();
        // Initialize false positive and true positive variables
        size_t start, end, j = 0, FP = 0, TP = 0;
        // The loop will iterate over the number of expected anomalies. Will check if recorded anomalies match
        // expected ones.
        for (int i = 0; i < numOfAnomalies; ++i) {
            start = expected.at(i).at(0); //
            end = expected.at(i).at(1);
            while (j < detected.size() && detected.at(j).at(0) <= end) {
                if (detected.at(j).at(1) < start)
                    ++FP;
                else
                    ++TP;
                ++j;
            }
            N -= (end - start + 1);
        }
        FP += (detected.size() - j);
        float tpRate = floor(1000 * ((float)TP / P)) / 1000;
        float fpRate = floor(1000 * ((float)FP / N)) / 1000;
        dio->write("True Positive Rate: ");
        dio->write(tpRate);
        dio->write("\n");
        dio->write("False Positive Rate: ");
        dio->write(fpRate);
        dio->write("\n");
    }
};

// Command 6 - exits the program.
class ExitCommand : public Command {
public:
    ExitCommand(DefaultIO *dio, Database *db) : Command(dio, db, "6.exit\n") {};
    void execute() override {
        db->stopFlg = false;
    }
};

#endif /* COMMANDS_H_ */
