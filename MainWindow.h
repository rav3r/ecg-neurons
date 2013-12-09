#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <vector>
#include "fann/fann.h"

class Signal;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onLoad();
    void onLearnArrhytmia();
    void onLearnMyocardial();
    void onLearnPericarditis();

private:
    Ui::MainWindow *ui;

    bool loadLearningSignals(Signal& reference, Signal& distorted);

    // arrhytmia
    bool getRDistances(Signal& signal, std::vector<float>& distances);
    fann *arrhytmiaAnn;
};

#endif // MAINWINDOW_H
