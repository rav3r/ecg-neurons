/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot/qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoad_Physionet_ECG_File;
    QAction *MainWindow_2;
    QAction *actionArrhytmia;
    QAction *actionMyocardial;
    QAction *actionPericarditis;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_2;
    QCustomPlot *plot;
    QWidget *tab_2;
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout_3;
    QCustomPlot *mPlot;
    QWidget *tab_3;
    QGridLayout *gridLayout_4;
    QVBoxLayout *verticalLayout_4;
    QCustomPlot *pPlot;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuLearn;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(600, 460);
        actionLoad_Physionet_ECG_File = new QAction(MainWindow);
        actionLoad_Physionet_ECG_File->setObjectName(QStringLiteral("actionLoad_Physionet_ECG_File"));
        MainWindow_2 = new QAction(MainWindow);
        MainWindow_2->setObjectName(QStringLiteral("MainWindow_2"));
        actionArrhytmia = new QAction(MainWindow);
        actionArrhytmia->setObjectName(QStringLiteral("actionArrhytmia"));
        actionMyocardial = new QAction(MainWindow);
        actionMyocardial->setObjectName(QStringLiteral("actionMyocardial"));
        actionPericarditis = new QAction(MainWindow);
        actionPericarditis->setObjectName(QStringLiteral("actionPericarditis"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_2 = new QGridLayout(tab);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        plot = new QCustomPlot(tab);
        plot->setObjectName(QStringLiteral("plot"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(plot->sizePolicy().hasHeightForWidth());
        plot->setSizePolicy(sizePolicy);
        plot->setMaximumSize(QSize(4096, 4096));

        verticalLayout_2->addWidget(plot);


        gridLayout_2->addLayout(verticalLayout_2, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayout_3 = new QGridLayout(tab_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        mPlot = new QCustomPlot(tab_2);
        mPlot->setObjectName(QStringLiteral("mPlot"));

        verticalLayout_3->addWidget(mPlot);


        gridLayout_3->addLayout(verticalLayout_3, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        gridLayout_4 = new QGridLayout(tab_3);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        pPlot = new QCustomPlot(tab_3);
        pPlot->setObjectName(QStringLiteral("pPlot"));

        verticalLayout_4->addWidget(pPlot);


        gridLayout_4->addLayout(verticalLayout_4, 0, 0, 1, 1);

        tabWidget->addTab(tab_3, QString());

        gridLayout->addWidget(tabWidget, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuLearn = new QMenu(menuBar);
        menuLearn->setObjectName(QStringLiteral("menuLearn"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuLearn->menuAction());
        menuFile->addAction(actionLoad_Physionet_ECG_File);
        menuLearn->addAction(actionMyocardial);
        menuLearn->addAction(actionPericarditis);

        retranslateUi(MainWindow);
        QObject::connect(actionLoad_Physionet_ECG_File, SIGNAL(triggered()), MainWindow, SLOT(onLoad()));
        QObject::connect(actionMyocardial, SIGNAL(triggered()), MainWindow, SLOT(onLearnMyocardial()));
        QObject::connect(actionPericarditis, SIGNAL(triggered()), MainWindow, SLOT(onLearnPericarditis()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "EGC Doctor Helper", 0));
        actionLoad_Physionet_ECG_File->setText(QApplication::translate("MainWindow", "Load Physionet ECG File", 0));
        MainWindow_2->setText(QApplication::translate("MainWindow", "MainWindow", 0));
        actionArrhytmia->setText(QApplication::translate("MainWindow", "Arrhytmia", 0));
        actionMyocardial->setText(QApplication::translate("MainWindow", "Myocardial", 0));
        actionPericarditis->setText(QApplication::translate("MainWindow", "Pericarditis", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Arrhytmia", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Myocardial", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Pericarditis", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
        menuLearn->setTitle(QApplication::translate("MainWindow", "Learn", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
