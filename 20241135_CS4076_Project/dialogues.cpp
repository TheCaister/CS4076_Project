#include "dialogues.h"


string Dialogues::printCurrentRoom(string& description){
    return "You are currently in: " + description + ". What is your next move?\n";
}

string Dialogues::printAttemptsLeft(int& attemptsLeft){
    return "Keep it up! You have " + std::to_string(attemptsLeft) + " tries left.\n";
}
