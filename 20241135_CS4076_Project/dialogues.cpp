#include "dialogues.h"

string Dialogues::welcome = "Welcome to the city of Wordleville, a harsh world"
                            " where your status is determined by your Wordle skills."
                            " If you seek assistance, enter the 'info' command.\n";

string Dialogues::inputError = "Invalid command, please try again.\n";

string Dialogues::noMoreRooms = "No more areas this way. Try another direction.\n";

string Dialogues::printCurrentRoom(string description){
    return "You are currently in: " + description + ". What is your next move?\n";
}
