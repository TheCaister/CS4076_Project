#include "WordleEngine.h"
#include <iostream>
#include <fstream>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <mainwindow.h>
#include <unordered_map>
#include <cctype>
#include "dialogues.h"

using std::ifstream;
using std::getline;
using std::cout;
using std::endl;

vector<string> WordleEngine::allWords;
int WordleEngine::triesLeft;
WordleEngine::gameStatus WordleEngine::wordleStatus = WordleEngine::WORDLE_STOP;
string WordleEngine::targetWord;

bool WordleEngine::isEqualIgnoreCase(char first, char second){
    return tolower(first) == tolower(second);
}

void WordleEngine::initWords(string filename){
    if(filename.empty()){
        string text;

        QFile file(":/text/wordle_words.txt");
        QTextStream in(&file);
        if (file.open(QFile::ReadOnly | QFile::Text)){
            while(!in.atEnd()){
                //qDebug() << in.readLine();
                WordleEngine::allWords.push_back(in.readLine().toStdString());
            }

        }

        file.close();
    }
}

void WordleEngine::initialiseWordleEngine(){
    initWords("");
    WordleEngine::wordleStatus = WordleEngine::WORDLE_PROGRESS;
}

string WordleEngine::evaluateInput(const string &input){
    string output = "";
    std::unordered_map<char, int> letter_counts;
    string correctWord = WordleEngine::targetWord;
    int correctLettersAmount = 0;

    if(input.size() != correctWord.size()){
        return "";
    }

    // Initialising the letter_counts map, pairing each letter with 1
    for(int i = 0; i < (int) correctWord.size(); i++){
        char currentChar = tolower(correctWord.at(i));
        // If the letter does not exist yet, add it and pair it with the int 1
        // Otherwise, just add 1 to that entry.
        if(letter_counts.count(currentChar)){
            letter_counts[currentChar] += 1;
        }
        else{
            letter_counts.insert({currentChar, 1});
        }
    }

    for(int i = 0; i < (int) input.size(); i++){
        char currentInputLetter = input.at(i);

        // If the character is at the correct index
        if(WordleEngine::isEqualIgnoreCase(input.at(i), correctWord.at(i))){
            letter_counts[currentInputLetter] -= 1;
            output.push_back('[');
            output.push_back(currentInputLetter);
            output.push_back(']');
            correctLettersAmount++;

        }
        // If the character exists but is in the wrong index
        // In this case, we also check the letter_counts map to see if there are any of that particular letter remaining.
        else if(letter_counts[currentInputLetter] > 0){
            letter_counts[currentInputLetter] -= 1;
            output.push_back('{');
            output.push_back(currentInputLetter);
            output.push_back('}');
        }
        else{
            output.push_back(currentInputLetter);
        }
    }

    output = output + " DEV: " + correctWord + '\n';

    // If the guess is correct, set status to success
    if(correctLettersAmount == (int) correctWord.size()){
        output += Dialogues::wordleSuccess;
        WordleEngine::wordleStatus = WordleEngine::WORDLE_SUCCESS;
    }
    else{
        WordleEngine::triesLeft--;

        if(WordleEngine::triesLeft == 0){
            output += Dialogues::wordleOutOfAttempts;
            WordleEngine::wordleStatus = WordleEngine::WORDLE_STOP;
        }
        else{
            output += Dialogues::printAttemptsLeft(WordleEngine::triesLeft);
        }
    }

    return output;
}

vector<string> WordleEngine::getAllWords(){
    return WordleEngine::allWords;
}

void WordleEngine::startWordleGame(){
    WordleEngine::triesLeft = 5;
    // Get a random word
    WordleEngine::targetWord = allWords.at(rand() % allWords.size());
}
