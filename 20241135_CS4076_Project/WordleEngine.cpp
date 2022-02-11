#include "WordleEngine.h"
#include <iostream>
#include <fstream>
#include <QTextStream>
#include <QFile>
#include <QDebug>

using std::ifstream;
using std::getline;
using std::cout;
using std::endl;

vector<string> WordleEngine::allWords;

void WordleEngine::initWords(string filename){
    if(filename.empty()){
        string text;

        QFile file(":/text/wordle_words.txt");
        QTextStream in(&file);
        if (file.open(QFile::ReadOnly | QFile::Text)){
            while(!in.atEnd()){
                qDebug() << in.readLine();
                this->allWords.push_back(in.readLine().toStdString());
            }

        }

        file.close();
    }
}

WordleEngine::WordleEngine(){
    initWords("");
    this->triesLeft = 5;
}

bool WordleEngine::evaluateInput(string input){
    return false;
}

vector<string> WordleEngine::getAllWords(){
    return this->allWords;
}
