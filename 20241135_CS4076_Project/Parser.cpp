#include "Parser.h"

Parser::Parser() {
    commands = new CommandWords();
}

Parser::~Parser(){
    delete commands;
}

Command* Parser::convertToCommand(string input){
    string word1 = "";
    string word2 = "";
    vector<string> words;

    // Size type is basically an int that's big enough
    // to represent the size of a string.
    string::size_type pos = 0, last_pos = 0;

    // Break "buffer" up by spaces
    //bool finished = false;
    //    while (!finished) {
    //        pos = input.find_first_of(' ', last_pos);	// find and remember first space.
    //        // npos tells you if there is no string found.
    //        if (pos == string::npos ) {			// if we found the last word,
    //            words.push_back(input.substr(last_pos));	// add it to vector "words"
    //            finished = true;				// and finish searching.
    //        } else {					// otherwise add to vector and move on to next word.
    //            words.push_back(input.substr(last_pos, pos - last_pos));
    //            last_pos = pos + 1;
    //        }
    //    }

    pos = input.find_first_of(' ', last_pos);	// find and remember first space.
    // npos tells you if there is no string found.
    if (pos == string::npos ) {
        words.push_back(input);    // Get the first(and only) word
    } else {
        words.push_back(input.substr(0, pos));
        words.push_back(input.substr(pos + 1, input.size() - pos - 1));
    }

    if (words.size() == 1) //was only 1 word entered?
        word1 = words[0]; //get first word
    else if (words.size() >= 2) { //were at least 2 words entered?
        word1 = words[0]; //get first word
        word2 = words[1]; //get second word
    }

    // note: we just ignore the rest of the input line.
    // Now check whether this word is known. If so, create a command with it.
    // If not, create a "nil" command (empty string for unknown command).

    return new Command(word1, word2);
}

string Parser::commandsInString(){
    return commands->validCommandsToString();
}
