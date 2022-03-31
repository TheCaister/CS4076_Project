#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ZorkUL.h"
#include "dialogues.h"
#include "constants.h"

#include <QDebug>
#include <QMovie>
#include <QKeyEvent>
#include <QScrollBar>

using namespace Constants;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->outputConsole->setWordWrap(true);


    ui->moneyLabel->setText(QString::fromStdString("Money: $" + std::to_string(ZorkUL::getMoney())));

    // Making the image fill the available space.
    ui->current_image->setScaledContents( true );

    ui->input->setFocus();

    //Setting stylesheets for buttons
    ui->upButton->setStyleSheet(":hover{color: rgb(183, 110, 255); border: 3px solid rgb(183, 110, 255)}");
    ui->downButton->setStyleSheet(":hover{color: rgb(183, 110, 255); border: 3px solid rgb(183, 110, 255)}");
    ui->leftButton->setStyleSheet(":hover{color: rgb(183, 110, 255); border: 3px solid rgb(183, 110, 255)}");
    ui->rightButton->setStyleSheet(":hover{color: rgb(183, 110, 255); border: 3px solid rgb(183, 110, 255)}");
    ui->quitButton->setStyleSheet(":hover{color: rgb(183, 110, 255); border: 3px solid rgb(183, 110, 255)}");
    ui->infoButton->setStyleSheet(":hover{color: rgb(183, 110, 255); border: 3px solid rgb(183, 110, 255)}");
    ui->interactButton->setStyleSheet(":hover{color: rgb(183, 110, 255); border: 3px solid rgb(183, 110, 255)}");
    ui->roomButton->setStyleSheet(":hover{color: rgb(183, 110, 255); border: 3px solid rgb(183, 110, 255)}");
    ui->invButton->setStyleSheet(":hover{color: rgb(183, 110, 255); border: 3px solid rgb(183, 110, 255)}");

}


MainWindow::~MainWindow()
{
    delete ui;

    if(this->currentMovie != NULL){
        delete this->currentMovie;

    }
}

void MainWindow::clearConsole(){
    ui->outputConsole->clear();
}

// Printing to game console - Supports string and QString
void MainWindow::addStringToConsole(const string &input){
    //qDebug("Hello");
    ui->outputConsole->setText(ui->outputConsole->text() + QString::fromStdString("\n") + QString::fromStdString(input));
}

void MainWindow::addQStringToConsole(const QString &input){
    //qDebug("Hello");
    ui->outputConsole->setText(ui->outputConsole->text() + QString::fromStdString("\n") + input);
}

// Clears the console and prints something
void MainWindow::overwriteConsole(const string &input){
    ui->outputConsole->clear();
    addStringToConsole(input);
}

// Updates the background based on the path parameter
void MainWindow::updateBackground(const string &path){
    const unsigned long long dotIndex = path.find(".");

    // find() returns npos if a "." is not found
    if(dotIndex == string::npos){
        return;
    }


    // Check what type of file "path" is
    // E.g. we have movie.gif and we want to separate the ".gif" part
    // This can be done using substr, using the position of the "." as param1 and
    // the length between the full path and the string leading up to the "." as param2
    string fileType = path.substr(dotIndex, (path.length() - dotIndex));
    //addStringToConsole("File type: " + fileType);

    QString file(QString::fromStdString(path));

    if(fileType.compare(".gif") == 0){
        this->currentMovie = new QMovie(file);

        ui->current_image->setMovie(this->currentMovie);
        this->currentMovie->start();
    }

    else if(fileType.compare(".jpg") == 0 || fileType.compare(".png") == 0){
        QPixmap pixmap(file);
        ui->current_image->setPixmap(pixmap);
    }
}

// Handling what happens when the "enter" key is pressed on the text edit input.
void MainWindow::on_input_textChanged()
{
    // Converting from QString to string and finding the index of "enter" or "\n"
    string input = ui->input->toPlainText().toStdString();
    size_t newlineIndex = input.find('\n');

    // Preventing users from entering several enter lines
    if(newlineIndex == 0){
        ui->input->clear();
        return;
    }

    // Removing the newline from the input to be later passed into parseInput
    input = input.substr(0, newlineIndex);

    // Checks if there are any newlines or if the "enter" key is pressed
    if(newlineIndex != string::npos && input.size() > 0){
        //addStringToConsole("> " + input + "\n");
        this->parseInput(input);
        this->ui->input->clear();
    }
}

// Trying to convert input to a command and printing out the appropriate output.
void MainWindow::parseInput(const string &input){
    Command *command = ZorkUL::getParser()->convertToCommand(input);

    string inputString = "> " + input + "\n\n";

    string output = ZorkUL::processCommand(*command, this);

    // Processes errors
    if(output.compare("") == 0){
        //addStringToConsole(Dialogues::inputError);
        overwriteConsole(inputString + OtherDialogues::inputError);
        return;
    }

    //    addStringToConsole(output);
    overwriteConsole(inputString + output);
    ui->moneyLabel->setText(QString::fromStdString("Money: $" + std::to_string(ZorkUL::getMoney())));

    delete command;

    ui->input->setFocus();
}

void MainWindow::on_upButton_released()
{
    parseInput("go north");
}


void MainWindow::on_rightButton_released()
{
    parseInput("go east");
}


void MainWindow::on_downButton_released()
{
    parseInput("go south");
}


void MainWindow::on_leftButton_released()
{
    parseInput("go west");
}


void MainWindow::on_infoButton_released()
{
    parseInput("info");
}


void MainWindow::on_quitButton_released()
{
    parseInput("quit");
}


void MainWindow::on_invButton_released()
{
    parseInput("check inventory");
}


void MainWindow::on_roomButton_released()
{
    parseInput("check room");
}


void MainWindow::on_interactButton_released()
{
    parseInput("interact");
}

