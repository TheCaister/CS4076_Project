#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

using std::string;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void clearConsole();
    void addStringToConsole(const string &input);
    void addQStringToConsole(const QString &input);
    void overwriteConsole(const string &input);
    void updateBackground(const string &path);
    void parseInput(const string &input);
    QMovie* currentMovie;

private slots:
    void on_input_textChanged();

    void on_upButton_released();

    void on_rightButton_released();

    void on_downButton_released();

    void on_leftButton_released();

    void on_infoButton_released();

    void on_quitButton_released();

    void on_invButton_released();

    void on_roomButton_released();

    void on_interactButton_released();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
