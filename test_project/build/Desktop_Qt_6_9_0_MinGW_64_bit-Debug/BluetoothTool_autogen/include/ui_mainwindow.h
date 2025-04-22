/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *resetBtn;
    QPushButton *readBtn;
    QPushButton *writeBtn;
    QListView *listView;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        resetBtn = new QPushButton(centralwidget);
        resetBtn->setObjectName("resetBtn");
        resetBtn->setGeometry(QRect(50, 70, 121, 41));
        readBtn = new QPushButton(centralwidget);
        readBtn->setObjectName("readBtn");
        readBtn->setGeometry(QRect(50, 120, 121, 41));
        writeBtn = new QPushButton(centralwidget);
        writeBtn->setObjectName("writeBtn");
        writeBtn->setGeometry(QRect(50, 170, 121, 41));
        listView = new QListView(centralwidget);
        listView->setObjectName("listView");
        listView->setGeometry(QRect(190, 70, 561, 451));
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        resetBtn->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        readBtn->setText(QCoreApplication::translate("MainWindow", "Read BD_ADDR", nullptr));
        writeBtn->setText(QCoreApplication::translate("MainWindow", "Write BD_ADDR", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
