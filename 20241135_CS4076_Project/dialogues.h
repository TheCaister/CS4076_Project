#ifndef DIALOGUES_H
#define DIALOGUES_H

#include <string>
using std::string;

string printCurrentRoom(string);

struct Dialogues{
private:


public:
    static string printCurrentRoom(string description);
    static string welcome;
    static string inputError;
    static string noMoreRooms;
    static string welcomeWordle;
    static string wordleIncorrectInput;
};

#endif // DIALOGUES_H
