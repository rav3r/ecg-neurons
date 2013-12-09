#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>
#include <QFileDialog>

#include <iostream>

#include "ecg-annotation/signal.h"
#include "ecg-annotation/ecgannotation.h"

static wchar_t anncodes [51][10] =  { L"notQRS", L"N",       L"LBBB",    L"RBBB",     L"ABERR", L"PVC",
                                      L"FUSION", L"NPC",     L"APC",     L"SVPB",     L"VESC",  L"NESC",
                                      L"PACE",   L"UNKNOWN", L"NOISE",   L"q",        L"ARFCT", L"Q",
                                      L"STCH",   L"TCH",     L"SYSTOLE", L"DIASTOLE", L"NOTE",  L"MEASURE",
                                      L"P",      L"BBB",     L"PACESP",  L"T",        L"RTM",   L"U",
                                      L"LEARN",  L"FLWAV",   L"VFON",    L"VFOFF",    L"AESC",  L"SVESC",
                                      L"LINK",   L"NAPC",    L"PFUSE",   L"(",        L")",     L"RONT",

                                      //user defined beats//
                                      L"(p",     L"p)",      L"(t",      L"t)",       L"ECT",
                                      L"r",      L"R",       L"s",       L"S"
                                    };

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    arrhytmiaAnn = 0;

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

    float lastRTime = -1;

    static std::vector<QCPAbstractItem*> items;

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
        ui->plot->clearGraphs();
        ui->plot->addGraph();
        for(int i=0; i<items.size(); i++)
        {
            delete items[i];
        }
        items.clear();

        ui->plot->graph(0)->setData(x, y);

        ui->plot->xAxis->setLabel("time (s)");
        ui->plot->yAxis->setLabel("y");

        ui->plot->xAxis->setRange(0, 2);
        ui->plot->yAxis->setRange(-4, 4);

        EcgAnnotation ann;
        int** qrsAnn = ann.GetQRS(data, dataSize, sr, L"filters");

        if (qrsAnn)
        {
            int** ANN = ann.GetPTU(data, dataSize, sr, L"filters",
                                qrsAnn, ann.GetQrsNumber());
            if (ANN)
            {
                QPen pen;
                pen.setColor(QColor(255,0,0));

                int annNum = ann.GetEcgAnnotationSize();
                for (int i = 0; i < annNum; i++)
                {
                    int smpl = ANN[i][0];
                    int type = ANN[i][1];

                    float msec = float(((double)smpl / sr));

                    if(anncodes[type][0] == L'R')
                    {
                        if(lastRTime > 0)
                        {
                            float time = 60.0f/(msec - lastRTime);
                            bool blue = false;

                            time -= 75;
                            time /= 20.0f;
                            if(time < 0) { time = -time; blue = true; }
                            if(time > 1) time = 1;

                            std::cout << time << "->" << (msec - lastRTime) << "\n";

                            QColor color = QColor(!blue ? 255 : 255*(1-time), 255*(1-time), blue ? 255 : 255*(1-time), 128);

                            QBrush brush;
                            brush.setStyle(Qt::SolidPattern);
                            brush.setColor(color);

                            QCPItemRect* rect = new QCPItemRect(ui->plot);
                            rect->topLeft->setCoords(lastRTime, 4);
                            rect->bottomRight->setCoords(msec, -4);
                            rect->setBrush(brush);

                            items.push_back(rect);

                            /*QCPItemLine *arrow = new QCPItemLine(ui->plot);
                            ui->plot->addItem(arrow);
                            arrow->start->setCoords(lastRTime, 6);
                            arrow->end->setCoords(msec, 6);
                            arrow->setHead(QCPLineEnding::esLineArrow);
                            arrow->setTail(QCPLineEnding::esLineArrow);

                            items.push_back(arrow);*/

                            QCPItemText *text = new QCPItemText(ui->plot);
                            ui->plot->addItem(text);
                            text->setText(QString::number(msec - lastRTime, 'g', 2)+"s");
                            text->position->setCoords((lastRTime+msec)/2, 6.3);

                            items.push_back(text);
                        }

                        lastRTime = msec;
                    }
                }
            }
            else
            {
                QMessageBox::warning(this, "Warning", "Unknown error");
            }
        } else
        {
            QMessageBox::warning(this, "Warning", "Could not get QRS complexes");
        }

        ui->plot->replot();

    } else
    {
         QMessageBox::warning(this, "Warning", "Failed to load file " + fileName);
    }
}

bool MainWindow::loadLearningSignals(Signal& referenceSignal, Signal& distortedSignal)
{
    QString referenceFileName = QFileDialog::getOpenFileName(this, tr("Open Reference Physionet ECG File"),
                                                    "", tr("Physionet ECG File (*.dat)"));

    if(referenceFileName.isEmpty())
        return false;

    if(!referenceSignal.ReadFile(referenceFileName.toStdWString().c_str()))
    {
        QMessageBox::warning(this, "Warning", "Failed to load file " + referenceFileName);
        return false;
    }

    QString distortedFileName = QFileDialog::getOpenFileName(this, tr("Open Distorted Physionet ECG File"),
                                                    "", tr("Physionet ECG File (*.dat)"));

    if(distortedFileName.isEmpty())
        return false;

    if(!distortedSignal.ReadFile(distortedFileName.toStdWString().c_str()))
    {
        QMessageBox::warning(this, "Warning", "Failed to load file " + distortedFileName);
        return false;
    }

    return true;
}

bool MainWindow::getRDistances(Signal& signal, std::vector<float>& distances)
{
    int dataSize = signal.GetLength();
    double* data = signal.GetData();

    double sr = signal.GetSR();
    float msec = float((double)dataSize / sr);

    EcgAnnotation ann;
    int** qrsAnn = ann.GetQRS(data, dataSize, sr, L"filters");

    distances.clear();

    float lastRTime = -1;

    if (qrsAnn)
    {
        int** ANN = ann.GetPTU(data, dataSize, sr, L"filters",
                            qrsAnn, ann.GetQrsNumber());
        if (ANN)
        {
            int annNum = ann.GetEcgAnnotationSize();
            for (int i = 0; i < annNum; i++)
            {
                int smpl = ANN[i][0];
                int type = ANN[i][1];

                msec = float(((double)smpl / sr));

                if(anncodes[type][0] == L'R')
                {
                    if(lastRTime >= 0)
                    {
                        distances.push_back(msec - lastRTime);
                    }

                    lastRTime = msec;
                }
            }
        }
        else
        {
            QMessageBox::warning(this, "Warning", "Unknown error");
            return false;
        }
    } else
    {
        QMessageBox::warning(this, "Warning", "Could not get QRS complexes");
        return false;
    }

    return true;
}

void MainWindow::onLearnArrhytmia()
{
    Signal referenceSignal, distortedSignal;
    if(!loadLearningSignals(referenceSignal, distortedSignal))
        return;

    std::vector<float> referenceDistances;
    if(!getRDistances(referenceSignal, referenceDistances))
        return;

    std::vector<float> distortedDistances;
    if(!getRDistances(distortedSignal, distortedDistances))
        return;

    const unsigned int num_input = 1;
    const unsigned int num_output = 1;
    const unsigned int num_layers = 3;
    const unsigned int num_neurons_hidden = 3;
    const float desired_error = (const float) 0.001;
    const unsigned int max_epochs = 10000;
    const unsigned int epochs_between_reports = 1000;

    arrhytmiaAnn = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);
    fann_set_activation_function_hidden(arrhytmiaAnn, FANN_LINEAR);
    fann_set_activation_function_output(arrhytmiaAnn, FANN_LINEAR);

    int dataCount = referenceDistances.size() + distortedDistances.size();

    fann_train_data data;
    data.num_data = dataCount;
    data.num_input = 1;
    data.num_output = 1;
    data.input = new fann_type*[dataCount];
    data.output = new fann_type*[dataCount];

    for(int i=0; i<referenceDistances.size(); i++)
    {
        data.input[i] = new fann_type[1];
        data.input[i][0] = referenceDistances[i];
        data.output[i] = new fann_type[1];
        data.output[i][0] = 0.0f;
    }

    for(int i=referenceDistances.size(); i<dataCount; i++)
    {
        data.input[i] = new fann_type[1];
        data.input[i][0] = distortedDistances[i-referenceDistances.size()];
        data.output[i] = new fann_type[1];
        data.output[i][0] = 1.0f;
    }

    fann_train_on_data(arrhytmiaAnn, &data, max_epochs, epochs_between_reports, desired_error);
}

void MainWindow::onLearnMyocardial()
{
}

void MainWindow::onLearnPericarditis()
{
}
