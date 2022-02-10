#ifndef WORDLEENGINE_H
#define WORDLEENGINE_H

#include <string>
#include <vector>
using std::string;
using std::vector;

class WordleEngine{
private:
    // All valid words
    static vector<string> allWords;
    int triesLeft;

public:
    bool evaluateInput(string input);
};

#endif // WORDLEENGINE_H
