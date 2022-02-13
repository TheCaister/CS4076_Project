#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMovie>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->input->setFocusPolicy(Qt::StrongFocus);
    ui->outputConsole->setWordWrap(true);

    QMovie *movie = new QMovie(":/img/img/night_city.gif");

    // Makking the image fill the available space.
    ui->current_image->setScaledContents( true );

    ui->current_image->setMovie(movie);
    movie->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearConsole(){
    ui->outputConsole->clear();
}

// Printing to game console - Supports string and QString
void MainWindow::addStringToConsole(string input){
    //qDebug("Hello");
    ui->outputConsole->setText(ui->outputConsole->text() + QString::fromStdString("\n") + QString::fromStdString(input));
}

void MainWindow::addQStringToConsole(QString input){
    //qDebug("Hello");
    ui->outputConsole->setText(ui->outputConsole->text() + QString::fromStdString("\n") + input);
}

void MainWindow::overwriteConsole(string input){
    ui->outputConsole->clear();
    addStringToConsole(input);
}

void MainWindow::updateBackground(string path){

    // find returns npos if a "." is not found
    if(path.find(".") == string::npos){
        return;
    }

    int dotIndex = path.find(".");

    // Check what type of file "path" is
    // E.g. we have movie.gif and we want to separate the "movie" part
    // This can be done using substr, using the position of the "." as param1 and
    // the length between the full path and the string leading up to the "." as param2
    string fileType = path.substr(dotIndex, (path.length() - dotIndex));
    addStringToConsole("File type: " + fileType);

    QString file(QString::fromStdString(path));

    if(fileType.compare(".gif") == 0){
        QMovie *movie = new QMovie(file);

        // Making the image fill the available space.
        ui->current_image->setScaledContents( true );

        ui->current_image->setMovie(movie);
        movie->start();
    }

    else if(fileType.compare(".jpg") == 0 || fileType.compare(".png") == 0){
        QPixmap pixmap(file);
        ui->current_image->setPixmap(pixmap);
    }
}

// Handling what happens when the "enter" key is pressed on the text edit input.
void MainWindow::on_input_textChanged()
{
    string input = ui->input->toPlainText().toStdString();
    size_t newlineIndex = input.find('\n');

    // Removing the newline from the string
    input = input.substr(0, newlineIndex);

    // Checks if there are any newlines or if the "enter" key is pressed
    if(newlineIndex != string::npos && input.size() > 0){
        addStringToConsole(input);
        ui->input->clear();
    }
}

