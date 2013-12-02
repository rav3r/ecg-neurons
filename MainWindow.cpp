#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>
#include <QFileDialog>

#include "ecg-annotation/signal.h"
#include "ecg-annotation/ecgannotation.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->plot->setInteraction(QCP::iRangeDrag, true);
    ui->plot->setInteraction(QCP::iRangeZoom, true);
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
        int dataSize = signal.GetLength();
        double* data = signal.GetData();

        double sr = signal.GetSR();
        float msec = float((double)dataSize / sr);

        QVector<double> x(dataSize), y(dataSize);
        for (int i=0; i<dataSize; ++i)
        {
            x[i] = i * msec / dataSize;
            y[i] = data[i];
        }

        ui->plot->addGraph();
        ui->plot->graph(0)->setData(x, y);

        ui->plot->xAxis->setLabel("time (s)");
        ui->plot->yAxis->setLabel("y");

        ui->plot->xAxis->setRange(0, 2);
        ui->plot->yAxis->setRange(-4, 4);

        EcgAnnotation ann;
        int** qrsAnn = ann.GetQRS(data, dataSize, sr, L"../../filters");

        if (qrsAnn)
        {
            int** ANN = ann.GetPTU(data, dataSize, sr, L"../../filters",
                                qrsAnn, ann.GetQrsNumber());
            if (ANN)
            {
                QPen pen;
                pen.setColor(QColor(255,0,0));

                int annNum = ann.GetEcgAnnotationSize();
                for (int i = 0; i < annNum; i++)
                {
                    int smpl = ANN[i][0];
                    //int type = ANN[i][1];

                    msec = float(((double)smpl / sr));

                    QCPItemLine *arrow = new QCPItemLine(ui->plot);
                    ui->plot->addItem(arrow);
                    arrow->start->setCoords(msec, 5);
                    arrow->end->setCoords(msec, -5);
                    arrow->setPen(pen);
                }
            }
            else
            {
                QMessageBox::warning(this, "Warning", "failed");
            }
        } else
        {
            QMessageBox::warning(this, "Warning", "could not get QRS complexes");
        }

        ui->plot->replot();

    } else
    {
         QMessageBox::warning(this, "Warning", "Failed to load file " + fileName);
    }
}
