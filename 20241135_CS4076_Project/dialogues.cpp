#include "dialogues.h"

const string Dialogues::welcome = "Welcome to the city of Wordleville, a harsh world"
                                  " where your status is determined by your Wordle skills."
                                  " If you seek assistance, enter the 'info' command.\n";

const string Dialogues::inputError = "Invalid command, please try again.\n";

const string Dialogues::noMoreRooms = "No more areas this way. Try another direction.\n";

const string Dialogues::welcomeWordle = "Ohh snap! It's a Wordle challenge! Input your attempt right now!!\n";

const string Dialogues::wordleIncorrectInput = "Please input a 5 letter word.\n";

const string Dialogues::wordleSuccess = "Congratulations! You have guessed the correct word!\n";

const string Dialogues::wordleOutOfAttempts = "Oof! You have run out of attempts!\n";

string Dialogues::printCurrentRoom(string description){
    return "You are currently in: " + description + ". What is your next move?\n";
}

string Dialogues::printAttemptsLeft(int attemptsLeft){
    return "Keep it up! You have " + std::to_string(attemptsLeft) + " tries left.\n";
}
