#ifndef PARSER_H_
#define PARSER_H_

#include "Command.h"
#include "CommandWords.h"

class Parser {
private:
	CommandWords *commands;

public:
	Parser();
	Command* getCommand();
	void showCommands();
    Command* convertToCommand(string input);
};

#endif /*PARSER_H_*/
