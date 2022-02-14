#include "WordleEngine.h"
#include <iostream>
#include <fstream>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <mainwindow.h>

using std::ifstream;
using std::getline;
using std::cout;
using std::endl;

vector<string> WordleEngine::allWords;
int WordleEngine::triesLeft;
int WordleEngine::gameStatus;

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
    WordleEngine::triesLeft = 5;
    WordleEngine::gameStatus = 0;
}

bool WordleEngine::evaluateInput(string input){
    return false;
}

vector<string> WordleEngine::getAllWords(){
    return WordleEngine::allWords;
}

void WordleEngine::startWordleGame(){
    string targetWord = allWords.at(rand() % allWords.size());

}
