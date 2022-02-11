#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->current_image->pixmap()
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

