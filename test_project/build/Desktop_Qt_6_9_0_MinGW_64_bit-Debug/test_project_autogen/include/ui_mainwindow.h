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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *resetButton;
    QPushButton *readBDADDRButton;
    QPushButton *writeBDADDRButton;
    QComboBox *portComboBox;
    QTextEdit *logTextEdit;
    QLineEdit *bdaddrLineEdit;
    QPushButton *connectButton;
    QPushButton *disconnectButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        resetButton = new QPushButton(centralwidget);
        resetButton->setObjectName("resetButton");
        resetButton->setGeometry(QRect(50, 120, 121, 41));
        readBDADDRButton = new QPushButton(centralwidget);
        readBDADDRButton->setObjectName("readBDADDRButton");
        readBDADDRButton->setGeometry(QRect(50, 170, 121, 41));
        writeBDADDRButton = new QPushButton(centralwidget);
        writeBDADDRButton->setObjectName("writeBDADDRButton");
        writeBDADDRButton->setGeometry(QRect(50, 220, 121, 41));
        portComboBox = new QComboBox(centralwidget);
        portComboBox->setObjectName("portComboBox");
        portComboBox->setGeometry(QRect(50, 10, 81, 31));
        logTextEdit = new QTextEdit(centralwidget);
        logTextEdit->setObjectName("logTextEdit");
        logTextEdit->setGeometry(QRect(400, 70, 321, 411));
        bdaddrLineEdit = new QLineEdit(centralwidget);
        bdaddrLineEdit->setObjectName("bdaddrLineEdit");
        bdaddrLineEdit->setGeometry(QRect(200, 220, 151, 41));
        connectButton = new QPushButton(centralwidget);
        connectButton->setObjectName("connectButton");
        connectButton->setGeometry(QRect(50, 70, 121, 41));
        disconnectButton = new QPushButton(centralwidget);
        disconnectButton->setObjectName("disconnectButton");
        disconnectButton->setGeometry(QRect(50, 270, 121, 41));
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        resetButton->setText(QCoreApplication::translate("MainWindow", "Reset", nullptr));
        readBDADDRButton->setText(QCoreApplication::translate("MainWindow", "Read BD_ADDR", nullptr));
        writeBDADDRButton->setText(QCoreApplication::translate("MainWindow", "Write BD_ADDR", nullptr));
        connectButton->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        disconnectButton->setText(QCoreApplication::translate("MainWindow", "Disconnect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
