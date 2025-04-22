#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "serialworker.h"
#include "serialworkerfactory.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    qDebug() << "MainWindow running in thread:" << QThread::currentThread();
    ui->setupUi(this);

    for (const QSerialPortInfo &info : QSerialPortInfo::availablePorts()) {
        ui->portComboBox->addItem(info.portName());
    }

    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::connectToPort);

    workerThread = new QThread(this);

    SerialWorkerFactory *factory = new SerialWorkerFactory();
    factory->moveToThread(workerThread);

    connect(workerThread, &QThread::started, factory, &SerialWorkerFactory::createWorker);

    connect(factory, &SerialWorkerFactory::workerReady, this, [=](SerialWorker *w) {
        worker = w;
        connect(worker, &SerialWorker::responseReceived, this, &MainWindow::handleResponse);
        connect(worker, &SerialWorker::logMessage, this, &MainWindow::log);

        worker->moveToThread(workerThread);
        connect(workerThread, &QThread::finished, worker, &QObject::deleteLater);
        factory->deleteLater();
    });

    workerThread->start();

}

MainWindow::~MainWindow()
{
    workerThread->quit();
    workerThread->wait();
    delete ui;
}

void MainWindow::connectToPort()
{
    QString portName = ui->portComboBox->currentText();
    QMetaObject::invokeMethod(worker, "connectToPort", Qt::QueuedConnection,
                              Q_ARG(QString, portName));
}

void MainWindow::on_disconnectButton_clicked()
{
    if (worker) {
        QMetaObject::invokeMethod(worker, "disconnectFromPort", Qt::QueuedConnection);
        log("Disconnect clicked!");
    }
}

void MainWindow::on_resetButton_clicked()
{
    QByteArray cmd;
    cmd.append(static_cast<char>(0x01));
    cmd.append(static_cast<char>(0x03));
    cmd.append(static_cast<char>(0x0C));
    cmd.append(static_cast<char>(0x00));
    QMetaObject::invokeMethod(worker, "sendCommand", Qt::QueuedConnection,
                              Q_ARG(QByteArray, cmd));
    qDebug() << "Reset clicked!";
}

void MainWindow::on_readBDADDRButton_clicked()
{
    QByteArray cmd;
    cmd.append(static_cast<char>(0x01));
    cmd.append(static_cast<char>(0x09));
    cmd.append(static_cast<char>(0x10));
    cmd.append(static_cast<char>(0x00));

    QMetaObject::invokeMethod(worker, "sendCommand", Qt::QueuedConnection,
                              Q_ARG(QByteArray, cmd));
}

void MainWindow::on_writeBDADDRButton_clicked()
{
    QString input = ui->bdaddrLineEdit->text();
    QRegularExpression re("^[0-9A-Fa-f]{12}$");
    if (!re.match(input).hasMatch()) {
        log("Invalid BD_ADDR format.");
        return;
    }

    QByteArray address;
    for (int i = 10; i >= 0; i -= 2) {
        bool ok;
        address.append(static_cast<char>(input.mid(i, 2).toUInt(&ok, 16)));
        if (!ok) {
            log("Invalid BD_ADDR format.");
            return;
        }
    }

    QByteArray cmd;
    cmd.append(static_cast<char>(0x01));
    cmd.append(static_cast<char>(0x01));
    cmd.append(static_cast<char>(0xFC));
    cmd.append(static_cast<char>(0x06));

    cmd.append(address);

    QMetaObject::invokeMethod(worker, "sendCommand", Qt::QueuedConnection,
                              Q_ARG(QByteArray, cmd));
}

void MainWindow::handleResponse(const QByteArray &data)
{
    log("Received: " + data.toHex(' ').toUpper());
}

void MainWindow::log(const QString &text)
{
    ui->logTextEdit->append(text);
}
