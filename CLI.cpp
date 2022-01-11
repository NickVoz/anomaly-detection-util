/**
 * CLI.cpp
 *
 * Authors:
 * Shani Shafir, ID: 207071721
 * Nick Vozilov, ID: 318732252
 */

#include "CLI.h"

CLI::CLI(DefaultIO* dio) : dio(dio){}

void CLI::start(){
    Database db;
    TrainCSVCommand command1 = TrainCSVCommand(dio, &db);
    ChangeThresholdCommand command2 = ChangeThresholdCommand(dio, &db);
    TestCSVCommand command3 = TestCSVCommand(dio, &db);
    DisplayResultsCommand command4 = DisplayResultsCommand(dio, &db);
    AnalyzeResultsCommand command5 = AnalyzeResultsCommand(dio, &db);
    commands.push_back(&command1);
    commands.push_back(&command2);
    commands.push_back(&command3);
    commands.push_back(&command4);
    commands.push_back(&command5);
    while (true) {
        for (Command* i : commands) {
            dio->write(i->description);
        }
        dio->write("6. exit");
        int choice = std::stoi(dio->read());
        if (choice == 6){
            break;
        }
        commands[choice]->execute();

    }


}


CLI::~CLI() {
}

