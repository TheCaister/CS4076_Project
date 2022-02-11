#ifndef WORDLEENGINE_H
#define WORDLEENGINE_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using std::string;
using std::vector;

class WordleEngine{
private:
    // All valid words
    static vector<string> allWords;
    int triesLeft;
    void initWords(string filename);

public:
    WordleEngine();
    bool evaluateInput(string input);
    vector<string> getAllWords();
};

#endif // WORDLEENGINE_H
