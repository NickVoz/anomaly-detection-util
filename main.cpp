#include "commands.h"
#include "CLI.h"


int main() {
    StandardIO sio;
    CLI cli(&sio);
    cli.start();
}
