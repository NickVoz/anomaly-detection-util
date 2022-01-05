

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
	Command(DefaultIO* dio, Database* db):dio(dio), db(db){}
	virtual void execute()=0;
	virtual ~Command(){}
};

// implement here your command classes
class TrainCSVCommand: public Command {
public:
    TrainCSVCommand(DefaultIO* dio, Database* db) : Command(dio, db){};
    void execute() override {
        dio->write("Please upload your local train CSV file.");
        std::string fileName = dio->read();
        TimeSeries ts = TimeSeries(fileName.c_str());
        db->detector.learnNormal(ts);
    }
};


#endif /* COMMANDS_H_ */
