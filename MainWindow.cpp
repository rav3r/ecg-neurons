#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>
#include <QFileDialog>

#include "ecg-annotation/signal.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLoad()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Physionet ECG File"),
                                                    "", tr("Physionet ECG File (*.dat)"));

    if(fileName.isEmpty())
        return;

    Signal signal;
    if(signal.ReadFile(fileName.toStdWString().c_str()))
    {
         QMessageBox::information(this, "Tytul", "Ok, wczytano");
    } else
    {
         QMessageBox::warning(this, "Warning", "Failed to load file " + fileName);
    }
}
