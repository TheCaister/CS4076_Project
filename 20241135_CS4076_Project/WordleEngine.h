#ifndef WORDLEENGINE_H
#define WORDLEENGINE_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using std::string;
using std::vector;



class WordleEngine{
public:
    enum gameStatus {WORDLE_STOP, WORDLE_PROGRESS, WORDLE_FAIL, WORDLE_SUCCESS};

private:
    // All valid words
    static vector<string> allWords;
    static int triesLeft;
    static void initWords(string filename);


public:
    static string evaluateInput(string input);
    static string targetWord;
    static vector<string> getAllWords();
    static void startWordleGame();
    static string checkGameStatus();
    static void initialiseWordleEngine();
    static gameStatus wordleStatus;
};

#endif // WORDLEENGINE_H
