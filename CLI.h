

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

/**
  * CLI class declaration
  */

class CLI {
	DefaultIO* dio;
    std::vector<Command*> commands;
    Database db;
public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */
