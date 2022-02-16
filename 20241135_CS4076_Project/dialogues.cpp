#include "dialogues.h"

string Dialogues::welcome = "Welcome to the city of Wordleville, a harsh world\n"
                 "where your status is determined by your Wordle skills.";

string Dialogues::inputError = "Invalid command, please try again.";

string Dialogues::printCurrentRoom(string description){
    return "You are currently in: " + description + ". what is your next move?";
}
