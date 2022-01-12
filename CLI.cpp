/**
 * CLI.cpp
 *
 * Authors:
 * Shani Shafir, ID: 207071721
 * Nick Vozilov, ID: 318732252
 */

#include "CLI.h"

CLI::CLI(DefaultIO* dio) : dio(dio) {
    this->dio = dio;
    commands.push_back(new TrainCSVCommand(dio, &db));
    commands.push_back(new ChangeThresholdCommand(dio, &db));
    commands.push_back(new TestCSVCommand(dio, &db));
    commands.push_back(new DisplayResultsCommand(dio, &db));
    commands.push_back(new AnalyzeResultsCommand(dio, &db));
    commands.push_back(new ExitCommand(dio, &db));
}

void CLI::start() {
    while (db.stopFlg) {
        dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:");
        for (Command* i : commands) {
            dio->write(i->description);
        }
        try {
            int choice = std::stoi(dio->read());
            commands[choice - 1]->execute();
        }
        catch (const exception& e) {
            dio->write("Please choose an option from 1 to 6.");
        }
    }
}


CLI::~CLI() {
    for (auto& command : commands) {
        delete command;
    }
}

