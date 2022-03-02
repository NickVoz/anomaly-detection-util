/**
 * CLI.cpp
 *
 * Authors:
 * Shani Shafir
 * Nick Vozilov
 */

#include "CLI.h"

/**
 * CLI class consturctor, receives IO object(either server IO or local
 * command line IO and pushes all the displayed commands to a command queue.
 */
CLI::CLI(DefaultIO* dio) : dio(dio) {
    this->dio = dio;
    commands.push_back(new TrainCSVCommand(dio, &db));
    commands.push_back(new ChangeThresholdCommand(dio, &db));
    commands.push_back(new TestCSVCommand(dio, &db));
    commands.push_back(new DisplayResultsCommand(dio, &db));
    commands.push_back(new AnalyzeResultsCommand(dio, &db));
    commands.push_back(new ExitCommand(dio, &db));
}

/**
 * Starts the CLI program
 */
void CLI::start() {
    while (db.stopFlg) {
        dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
        for (Command* i : commands) {
            dio->write(i->description);
        }
        try {
            int choice = std::stoi(dio->read());
            commands.at(choice - 1)->execute();
        }
        catch (const exception& e) {
            dio->write("Please choose an option from 1 to 6.\n");
        }
    }
}

/**
 * Destructor
 */
CLI::~CLI() {
    for (auto& command : commands) {
        delete command;
    }
}

