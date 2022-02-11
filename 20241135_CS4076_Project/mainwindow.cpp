#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMovie>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMovie *movie = new QMovie(":/img/img/night_city.gif");

    // Makking the image fill the available space.
    ui->current_image->setScaledContents( true );

    ui->current_image->setMovie(movie);
    movie->start();

    //ui->current_image->setPixmap(pixmap);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearConsole(){
    ui->outputConsole->clear();
}


void MainWindow::addToConsole(string input){
    qDebug("Hello");
    ui->outputConsole->setText(ui->outputConsole->text() + QString::fromStdString("\n") + QString::fromStdString(input));
}

void MainWindow::overwriteConsole(string input){
    ui->outputConsole->clear();
    addToConsole(input);
}

