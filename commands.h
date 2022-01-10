

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

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
};

// you may edit this class
class Command{
protected:
	DefaultIO* dio;
    std::string description;
    Database *db;
public:
	Command(DefaultIO* dio, Database* db, std::string desc):dio(dio), db(db), description(desc){}
	virtual void execute()=0;
	virtual ~Command(){}
};

// implement here your command classes
// Command 1
class TrainCSVCommand : public Command {
public:
    TrainCSVCommand(DefaultIO* dio, Database* db) : Command(dio, db, "upload a time series csv file"){};
    void execute() override {
        dio->write("Please upload your local train CSV file.");
        std::string learnFileName = dio->read();
        db->tsArray.push_back(TimeSeries(learnFileName.c_str())); // train file will be at array[0]
        dio->write("Upload complete.");
        dio->write("Please upload your local test CSV file.");
        std::string testFileName = dio->read();
        db->tsArray.push_back(TimeSeries(testFileName.c_str()));
        dio->write("Upload complete.");
    }
};
// Command 2
class ChangeThresholdCommand : public Command {
public:
    ChangeThresholdCommand(DefaultIO* dio, Database* db) : Command(dio, db, "algorithm settings"){};
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
    TestCSVCommand(DefaultIO* dio, Database* db) : Command(dio, db, "detect anomalies"){};
    void execute() override {
        db->detector.learnNormal(db->tsArray[0]);
        db->anomalies = db->detector.detect(db->tsArray[1]);
        dio->write("anomaly detection complete.");
    }
};

// Command 4
class DisplayResultsCommand : public Command {
public:
    DisplayResultsCommand(DefaultIO *dio, Database *db) : Command(dio, db, "display results") {};
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

std::vector<std::vector<int>> readExpectedAnomalies(std::string fileName) {
    std::ifstream input(fileName);
    std::string currentLine;
    std::vector<std::string> line;
    std::vector<std::vector<int>> output;
    while (input) {
        input >> currentLine;
        if (currentLine == "done") {
            break;
        }
        line = split(currentLine);
        std::vector<int> lineConverted;
        lineConverted.push_back(std::stoi(line[0]));
        lineConverted.push_back(std::stoi(line[1]));
        output.push_back(lineConverted);
    }
    return output;
}

#endif /* COMMANDS_H_ */
