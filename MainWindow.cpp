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
    ui->setupUi(this);

    ui->plot->setInteraction(QCP::iRangeDrag, true);
    ui->plot->setInteraction(QCP::iRangeZoom, true);

    ui->mPlot->setInteraction(QCP::iRangeDrag, true);
    ui->mPlot->setInteraction(QCP::iRangeZoom, true);

    ui->pPlot->setInteraction(QCP::iRangeDrag, true);
    ui->pPlot->setInteraction(QCP::iRangeZoom, true);

    pAnn = 0;
    mAnn = 0;

    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
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

        EcgAnnotation ann;
        int** qrsAnn = ann.GetQRS(data, dataSize, sr, L"filters");

        if (qrsAnn)
        {
            int** ANN = ann.GetPTU(data, dataSize, sr, L"filters",
                                qrsAnn, ann.GetQrsNumber());
            if (ANN)
            {
                fillArrhytmiaPlot(ANN, ui->plot, ann, x, y, signal.GetSR());
                fillMyocardialPlot(ANN, ui->mPlot, ann, x, y, signal.GetSR());
                fillPericarditisPlot(ANN, ui->pPlot, ann, x, y, signal.GetSR());
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

void MainWindow::fillArrhytmiaPlot(int** ANN, QCustomPlot *plot, EcgAnnotation& ann, QVector<double> &x, QVector<double> &y, double sr)
{
    QPen pen;
    pen.setColor(QColor(255,0,0));
    float lastRTime = -1;

    static std::vector<QCPAbstractItem*> items;

    for(int i=0; i<items.size(); i++)
    {
        delete items[i];
    }
    items.clear();

    plot->clearGraphs();
    plot->addGraph();

    plot->graph(0)->setData(x, y);

    plot->xAxis->setLabel("time (s)");
    plot->yAxis->setLabel("y");

    plot->xAxis->setRange(0, 2);
    plot->yAxis->setRange(-4, 4);

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

                QColor color = QColor(!blue ? 255 : 255*(1-time), 255*(1-time), blue ? 255 : 255*(1-time), 128);

                QBrush brush;
                brush.setStyle(Qt::SolidPattern);
                brush.setColor(color);

                QCPItemRect* rect = new QCPItemRect(plot);
                plot->addItem(rect);
                rect->topLeft->setCoords(lastRTime, 4);
                rect->bottomRight->setCoords(msec, -4);
                rect->setBrush(brush);

                items.push_back(rect);

                QCPItemText *text = new QCPItemText(plot);
                plot->addItem(text);
                text->setText(QString::number(msec - lastRTime, 'f', 2)+"s");
                text->position->setCoords((lastRTime+msec)/2, 6.3);

                items.push_back(text);
            }

            lastRTime = msec;
        }
    }

    plot->replot();
}

void MainWindow::fillMyocardialPlot(int** ANN, QCustomPlot *plot, EcgAnnotation& ann, QVector<double> &x, QVector<double> &y, double sr)
{
    QPen pen;
    pen.setColor(QColor(255,0,0));
    float lastTTime = -1;

    static std::vector<QCPAbstractItem*> items;

    for(int i=0; i<items.size(); i++)
    {
        delete items[i];
    }
    items.clear();

    plot->clearGraphs();
    plot->addGraph();

    plot->graph(0)->setData(x, y);

    plot->xAxis->setLabel("time (s)");
    plot->yAxis->setLabel("y");

    plot->xAxis->setRange(0, 2);
    plot->yAxis->setRange(-4, 4);

    float lastN, lastQ, lastR;
    bool foundN=false, foundQ=false;

    int currXPos = 0;
    float lastQTime, lastNTime, lastRTime;

    int annNum = ann.GetEcgAnnotationSize();
    for (int i = 0; i < annNum; i++)
    {
        int smpl = ANN[i][0];
        int type = ANN[i][1];

        float msec = float(((double)smpl / sr));

        /*
        QCPItemText *text = new QCPItemText(plot);
        plot->addItem(text);
        text->setText(QString::fromWCharArray(anncodes[type]));
        text->position->setCoords(msec, 7.3);*/

        if(anncodes[type][0] == L'(' && anncodes[type][1] == L't')
        {
            lastTTime = msec;

            while(currXPos < x.size()-1 && x[currXPos] < msec)
                currXPos++;
        }
        if(anncodes[type][0] == L't' && anncodes[type][1] == L')')
        {
            if(lastTTime > 0)
            {

                int firstXPos = currXPos;
                while(currXPos < x.size()-1 && x[currXPos] < msec)
                    currXPos++;

                float first = y[firstXPos];
                float second = y[currXPos];

                float mid = y[(firstXPos + currXPos)/2];

                mid -= (first+second)/2;

                float f = -mid/0.2f;
                if(f < 0) f = 0;
                if(f > 1) f = 1;

                float qr_perc = (lastN - lastQ) / (lastR - lastQ);

                float alpha = 0;

                if(mAnn != 0)
                {
                    fann_type *calc_out;
                    fann_type input[3];

                    input[0] = mid;
                    input[1] = lastRTime - lastQTime;
                    input[2] = qr_perc;
                    calc_out = fann_run(mAnn, input);
                    alpha = (calc_out[0] + 1)/2.0f;

                    if(alpha > 1) alpha = 1;
                    if(alpha < 0) alpha = 0;
                }

                QColor color = QColor(255, 0, 0, 128*alpha);

                QBrush brush;
                brush.setStyle(Qt::SolidPattern);
                brush.setColor(color);
                QPen pen;
                pen.setColor(color);

                QCPItemRect* rect = new QCPItemRect(plot);
                rect->topLeft->setCoords(lastNTime, 4);
                rect->bottomRight->setCoords(msec, -4);
                rect->setBrush(brush);
                QPen nonePen;
                nonePen.setColor(QColor(255,255,255,0));
                rect->setPen(nonePen);
                plot->addItem(rect);

                items.push_back(rect);


                brush.setColor(QColor(255,255,255,0));
                rect = new QCPItemRect(plot);
                rect->topLeft->setCoords(lastTTime, 4);
                rect->bottomRight->setCoords(msec, -4);
                rect->setBrush(brush);
                plot->addItem(rect);

                items.push_back(rect);

                QCPItemText *text = new QCPItemText(plot);
                plot->addItem(text);
                text->setText(QString::number(mid, 'f', 2));
                text->position->setCoords((lastTTime+msec)/2, 4.3);

                items.push_back(text);

                text = new QCPItemText(plot);
                plot->addItem(text);
                text->setText("T");
                text->position->setCoords((lastTTime+msec)/2, 5);

                items.push_back(text);
            }

            lastTTime = -1;
        }
        if(anncodes[type][0] == L'N')
        {
            while(currXPos < x.size()-1 && x[currXPos] < msec)
                currXPos++;

            lastN = y[currXPos];
            lastNTime = msec;
        }
        if(anncodes[type][0] == L'Q')
        {
            while(currXPos < x.size()-1 && x[currXPos] < msec)
                currXPos++;

            lastQ = y[currXPos];
            lastQTime = msec;

            std::cout << "LAST Q: "<<lastQ<<"\n";
        }
        if(anncodes[type][0] == L'R')
        {
            while(currXPos < x.size()-1 && x[currXPos] < msec)
                currXPos++;

            lastR = y[currXPos];
            lastRTime = msec;

            if(lastNTime < lastQTime && lastQTime < lastRTime && lastRTime - lastNTime < 1.0f)
            {


            float qr_perc = (lastN - lastQ) / (lastR - lastQ);

            //std::cout <<lastN<<" "<<lastQ<<" "<<lastR<<" "<<  qr_perc<<"\n";

            QBrush brush;
            brush.setStyle(Qt::SolidPattern);
            brush.setColor(QColor(255,255,255,20));


            QCPItemRect* rect = new QCPItemRect(plot);
            rect->topLeft->setCoords(lastNTime, 4);
            rect->bottomRight->setCoords(msec, -4);
            rect->setBrush(brush);
            plot->addItem(rect);

            items.push_back(rect);

            QCPItemText *text = new QCPItemText(plot);
            plot->addItem(text);
            text->setText(QString::number(msec - lastQTime, 'f', 2)+"s");
            text->position->setCoords((lastNTime+msec)/2, -4.3);

            items.push_back(text);

            text = new QCPItemText(plot);
            plot->addItem(text);
            text->setText(QString::number(qr_perc*100, 'f', 2)+"%");
            text->position->setCoords((lastNTime+msec)/2, 4.3);

            items.push_back(text);

            text = new QCPItemText(plot);
            plot->addItem(text);
            text->setText("Q");
            text->position->setCoords((lastNTime+msec)/2, 5);

            items.push_back(text);
            }
        }
    }

    plot->replot();
}

void MainWindow::fillPericarditisPlot(int** ANN, QCustomPlot *plot, EcgAnnotation& ann, QVector<double> &x, QVector<double> &y, double sr)
{
    QPen pen;
    pen.setColor(QColor(255,0,0));

    static std::vector<QCPAbstractItem*> items;

    for(int i=0; i<items.size(); i++)
    {
        delete items[i];
    }
    items.clear();

    plot->clearGraphs();
    plot->addGraph();

    plot->graph(0)->setData(x, y);

    plot->xAxis->setLabel("time (s)");
    plot->yAxis->setLabel("y");

    plot->xAxis->setRange(0, 2);
    plot->yAxis->setRange(-4, 4);

    int lastP = 0, lastQ = 0;
    int lastS = 0;

    int currXPos = 0;

    int annNum = ann.GetEcgAnnotationSize();
    for (int i = 0; i < annNum; i++)
    {
        int smpl = ANN[i][0];
        int type = ANN[i][1];

        float msec = float(((double)smpl / sr));

        if(anncodes[type][0] == L'p' && anncodes[type][1] == L')')
        {
            while(currXPos < x.size()-1 && x[currXPos] < msec)
                currXPos++;

            lastP = currXPos;
        }
        if(anncodes[type][0] == L'N')
        {
            while(currXPos < x.size()-1 && x[currXPos] < msec)
                currXPos++;

            lastQ = currXPos;
        }
        if(anncodes[type][0] == L')')
        {
            while(currXPos < x.size()-1 && x[currXPos] < msec)
                currXPos++;

            lastS = currXPos;
        }
        if(anncodes[type][0] == L'(' && anncodes[type][1] == L't')
        {
            while(currXPos < x.size()-1 && x[currXPos] < msec)
                currXPos++;

            int lastT = currXPos;

            if(lastP < lastQ && lastQ < lastS && lastS < lastT && x[lastT] - x[lastP] < 0.5f)
            {
                float midPQ = 0;
                for(int i=lastP; i<=lastQ; i++)
                {
                    midPQ += y[i];
                }
                midPQ /= (lastQ - lastP)+1;

                float midST = 0;
                for(int i=lastS; i<=lastT; i++)
                {
                    midST += y[i];
                }
                midST /= (lastT - lastS)+1;

                float alpha = 0;

                if(pAnn != 0)
                {
                    fann_type *calc_out;
                    fann_type input[1];

                    input[0] = midST - midPQ;
                    calc_out = fann_run(pAnn, input);
                    alpha = (calc_out[0] + 1)/2.0f;

                    if(alpha > 1) alpha = 1;
                    if(alpha < 0) alpha = 0;

                    std::cout <<input[0] << " -- A:"<< alpha <<"\n";
                }

                QBrush brush;
                brush.setStyle(Qt::SolidPattern);
                brush.setColor(QColor(255,0,0, 200*alpha));

                QPen pen;
                pen.setColor(brush.color());

                QCPItemRect* rect = new QCPItemRect(plot);
                rect->topLeft->setCoords(x[lastP], 4);
                rect->bottomRight->setCoords(x[lastT], -4);
                rect->setBrush(brush);
                plot->addItem(rect);

                items.push_back(rect);

                rect->setPen(pen);

                brush.setColor(QColor(255,255,255,20));

                rect = new QCPItemRect(plot);
                rect->topLeft->setCoords(x[lastP], 3.5);
                rect->bottomRight->setCoords(x[lastQ], -3.5);
                rect->setBrush(brush);
                plot->addItem(rect);

                items.push_back(rect);

                rect = new QCPItemRect(plot);
                rect->topLeft->setCoords(x[lastS], 3.5);
                rect->bottomRight->setCoords(x[lastT], -3.5);
                rect->setBrush(brush);
                plot->addItem(rect);

                items.push_back(rect);

                QCPItemText *text = new QCPItemText(plot);
                plot->addItem(text);
                text->setText("PQ");
                text->position->setCoords((x[lastP]+x[lastQ])/2, -4.3);

                items.push_back(text);

                text = new QCPItemText(plot);
                plot->addItem(text);
                text->setText("ST");
                text->position->setCoords((x[lastS]+x[lastT])/2, -4.3);

                items.push_back(text);

                text = new QCPItemText(plot);
                plot->addItem(text);
                text->setText(QString::number(midPQ, 'f', 2));
                text->position->setCoords((x[lastP]+x[lastQ])/2, 4.3);

                items.push_back(text);

                text = new QCPItemText(plot);
                plot->addItem(text);
                text->setText(QString::number(midST, 'f', 2));
                text->position->setCoords((x[lastS]+x[lastT])/2, 4.3);

                items.push_back(text);

                text = new QCPItemText(plot);
                plot->addItem(text);
                text->setText(QString("ST - PQ = ")+QString::number(midST - midPQ, 'f', 2));
                text->position->setCoords((x[lastP]+x[lastT])/2, 5);

                items.push_back(text);
            }
        }
    }

    plot->replot();
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

/*void MainWindow::onLearnArrhytmia()
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
}*/

void MainWindow::onLearnMyocardial()
{
    QString trainFileName = QFileDialog::getOpenFileName(this, tr("Open training file"),
                                                    "", tr("Training file (*.data)"));

    if(trainFileName.isEmpty())
        return;

    if(mAnn != 0)
         fann_destroy(mAnn);

    const unsigned int num_input = 3;
    const unsigned int num_output = 1;
    const unsigned int num_layers = 3;
    const unsigned int num_neurons_hidden = 5;
    const float desired_error = 0.001f;
    const unsigned int max_epochs = 50000;
    const unsigned int epochs_between_reports = 0;

     mAnn= fann_create_standard(num_layers, num_input,
        num_neurons_hidden, num_output);

     fann_set_activation_function_hidden(mAnn, FANN_SIGMOID_SYMMETRIC);
     fann_set_activation_function_output(mAnn, FANN_SIGMOID_SYMMETRIC);

     QByteArray ba = trainFileName.toLocal8Bit();

    fann_train_on_file(mAnn, ba.data(), max_epochs,
        epochs_between_reports, desired_error);

    QMessageBox::information(this, "Training Complete", QString("Training error = ") + QString::number(fann_get_MSE(mAnn)));

    ui->tabWidget->setTabEnabled(1, true);
}

void MainWindow::onLearnPericarditis()
{
    QString trainFileName = QFileDialog::getOpenFileName(this, tr("Open training file"),
                                                    "", tr("Training file (*.data)"));

    if(trainFileName.isEmpty())
        return;

    if(pAnn != 0)
         fann_destroy(pAnn);

    const unsigned int num_input = 1;
    const unsigned int num_output = 1;
    const unsigned int num_layers = 3;
    const unsigned int num_neurons_hidden = 4;
    const float desired_error = 0.001f;
    const unsigned int max_epochs = 50000;
    const unsigned int epochs_between_reports = 0;

     pAnn= fann_create_standard(num_layers, num_input,
        num_neurons_hidden, num_output);

     fann_set_activation_function_hidden(pAnn, FANN_SIGMOID_SYMMETRIC);
     fann_set_activation_function_output(pAnn, FANN_SIGMOID_SYMMETRIC);

     QByteArray ba = trainFileName.toLocal8Bit();

     std::cout << ba.data();

    fann_train_on_file(pAnn, ba.data(), max_epochs,
        epochs_between_reports, desired_error);

    QMessageBox::information(this, "Training Complete", QString("Training error = ") + QString::number(fann_get_MSE(pAnn)));

    ui->tabWidget->setTabEnabled(2, true);
}

void MainWindow::onLoadMyocardialANN()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open ann file"),
                                                    "", tr("ANN file (*.net)"));

    if(filename.isEmpty())
        return;

    if(mAnn != 0)
         fann_destroy(mAnn);

   QByteArray ba = filename.toLocal8Bit();

    mAnn =  fann_create_from_file(ba.data());

    ui->tabWidget->setTabEnabled(1, true);
}

void MainWindow::onSaveMyocardialANN()
{
    if(mAnn == 0)
        return;

    QString filename = QFileDialog::getSaveFileName(this, tr("Save ann file"),
                                                    "", tr("ANN file (*.net)"));
    if(filename.isEmpty())
        return;

    QByteArray ba = filename.toLocal8Bit();

    fann_save(mAnn, ba.data());
}

void MainWindow::onLoadPericarditisANN()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open ann file"),
                                                    "", tr("ANN file (*.net)"));

    if(filename.isEmpty())
        return;

    if(pAnn != 0)
         fann_destroy(pAnn);

   QByteArray ba = filename.toLocal8Bit();

    pAnn =  fann_create_from_file(ba.data());

    ui->tabWidget->setTabEnabled(2, true);
}

void MainWindow::onSavePericarditisANN()
{
    if(pAnn == 0)
        return;

    QString filename = QFileDialog::getSaveFileName(this, tr("Save ann file"),
                                                    "", tr("ANN file (*.net)"));
    if(filename.isEmpty())
        return;

    QByteArray ba = filename.toLocal8Bit();

    fann_save(pAnn, ba.data());
}
