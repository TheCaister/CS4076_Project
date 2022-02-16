#include "CommandWords.h"

vector<string> CommandWords::validCommands;

/**
 * Constructor - initialise the command words.
 */
CommandWords::CommandWords() {
	// Populate the vector if we haven't already.
	if (validCommands.empty()) {
		validCommands.push_back("go");
		validCommands.push_back("quit");
		validCommands.push_back("info");
		validCommands.push_back("map");
		validCommands.push_back("take");
		validCommands.push_back("put");
        validCommands.push_back("teleport");
	}
}

/**
 * Check whether a given String is a valid command word.
 * Return true if it is, false if it isn't.
 **/
bool CommandWords::isCommand(string aString) {
	for (unsigned int i = 0; i < validCommands.size(); i++)
	{
		if (validCommands[i].compare(aString) == 0)
			return true;
	}
	// if we get here, the string was not found in the commands
	return false;
}

/*
 * Print all valid commands to System.out.
 */
void CommandWords::showAll() {
	//Loops through validCommands and prints each to the screen.
	for (unsigned int i = 0; i < validCommands.size(); i++)
	{
		cout << validCommands[i]<< "  ";
	}
	cout << endl;
}

vector<string> CommandWords::getValidCommands(){
    return validCommands;
}

// Puts all valid commands in a single string(used for printing)
string CommandWords::validCommandsToString(){
    string allValidCommands = "";

    //Loops through validCommands and prints each to the screen.
    for (unsigned int i = 0; i < validCommands.size() - 1; i++)
    {
        allValidCommands += validCommands.at(i) + ", ";
    }

    allValidCommands += validCommands.at(validCommands.size() - 1);

    return allValidCommands;
}
