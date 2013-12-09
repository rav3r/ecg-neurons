#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <vector>
#include "fann/fann.h"

class Signal;
class QCustomPlot;
class EcgAnnotation;

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
    void onLearnMyocardial();
    void onLearnPericarditis();

private:
    Ui::MainWindow *ui;

    bool loadLearningSignals(Signal& reference, Signal& distorted);

    // arrhytmia
    bool getRDistances(Signal& signal, std::vector<float>& distances);
    void fillArrhytmiaPlot(int** ANN, QCustomPlot* plot, EcgAnnotation& ann, QVector<double>& x, QVector<double>& y, double sr);

    // myocardial
    void fillMyocardialPlot(int** ANN, QCustomPlot* plot, EcgAnnotation& ann, QVector<double>& x, QVector<double>& y, double sr);

    // pericarditis
    void fillPericarditisPlot(int** ANN, QCustomPlot* plot, EcgAnnotation& ann, QVector<double>& x, QVector<double>& y, double sr);
};

#endif // MAINWINDOW_H
