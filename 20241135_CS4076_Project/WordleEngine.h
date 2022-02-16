#ifndef WORDLEENGINE_H
#define WORDLEENGINE_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using std::string;
using std::vector;

enum gameStatus {WORDLE_START, WORDLE_PROGRESS, WORDLE_FAIL, WORDLE_SUCCESS};

class WordleEngine{
private:
    // All valid words
    static vector<string> allWords;
    static int triesLeft;
    static void initWords(string filename);


public:
    static bool evaluateInput(string input);
    static vector<string> getAllWords();
    static void startWordleGame();
    static string checkGameStatus();
    static void initialiseWordleEngine();
    static int gameStatus;
};

#endif // WORDLEENGINE_H
