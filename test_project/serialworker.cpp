// serialworker.cpp
#include "serialworker.h"
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QThread>

SerialWorker::SerialWorker(QObject *parent)
    : QObject(parent), serial(new QSerialPort(this)), responseTimer(new QTimer(this))
{
    qDebug() << "SerialWorker created in thread:" << QThread::currentThread();

    responseTimer->setSingleShot(true);
    connect(responseTimer, &QTimer::timeout, this, [this]() {
        if (buffer.size() >= 6) {
            emit responseReceived(buffer);
            emit logMessage(parseHCIResponse(buffer));
            buffer.clear();
        } else {
            emit logMessage("Incomplete HCI response, waiting for more data...");
        }
    });
}

SerialWorker::~SerialWorker()
{
    if (serial->isOpen())
        serial->close();
}

void SerialWorker::connectToPort(const QString &portName)
{
    qDebug() << "connectToPort called in thread:" << QThread::currentThread();

    if (serial->isOpen()) {
        serial->close();
    }

    serial->setPortName(portName);
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        serial->clear();
        QThread::msleep(100);
        connect(serial, &QSerialPort::readyRead, this, &SerialWorker::onReadyRead);
        emit logMessage("Connected to " + portName);
    } else {
        emit logMessage("Failed to connect to " + portName + ": " + serial->errorString());
    }

    QTimer::singleShot(1000, this, [this]() {
        qDebug() << "Manual read check in thread:" << QThread::currentThread();
        if (serial->bytesAvailable() > 0) {
            onReadyRead();
        } else {
            qDebug() << "No bytes available (manual check).";
        }
    });
}

void SerialWorker::disconnectFromPort() {
    if (serial && serial->isOpen()) {
        serial->close();
        emit logMessage("Port disconnected.");
    }
}


void SerialWorker::sendCommand(const QByteArray &data)
{
    qDebug() << "sendCommand called with:" << data.toHex(' ').toUpper();

    if (!serial->isOpen()) {
        emit logMessage("Serial port not open");
        return;
    }


    serial->write(data);
    QString parsed = parseHCICommand(data);
    emit logMessage(parsed);
}


void SerialWorker::onReadyRead()
{
    QByteArray data = serial->readAll();
    qDebug() << "[RAW] Received:" << data.toHex(' ').toUpper();
    buffer += data;
    responseTimer->start(50);
}


QString SerialWorker::parseHCICommand(const QByteArray &data)
{
    if (data.size() < 4) return "Unknown or incomplete HCI command";

    quint8 packetType = static_cast<quint8>(data[0]);

    if (packetType == 0x01) {
        quint16 opcode = static_cast<quint8>(data[2]) << 8 | static_cast<quint8>(data[1]);
        QString name;

        if (opcode == 0x0C03) {
            name = "Reset";
        } else if (opcode == 0x1009) {
            name = "Read_BD_ADDR";
        } else if (opcode == 0xFC01) {
            name = "Write_BD_ADDR";
        } else {
            name = "Unknown";
        }

        QString timestamp = QDateTime::currentDateTime().toString("MM/dd/yy hh:mm:ss.zzz");
        QString result = QString("%1 COM%2@115200 c> %3\n"
                                 "HCI Command\n"
                                 "COM%2@115200\n")
                             .arg(timestamp)
                             .arg(serial->portName().remove("COM"))
                             .arg(name);

        result += QString("[%1 %2 %3 ]")
                      .arg(QString::number(static_cast<quint8>(data[1]), 16).rightJustified(2, '0').toUpper())
                      .arg(QString::number(static_cast<quint8>(data[2]), 16).rightJustified(2, '0').toUpper())
                      .arg(QString::number(static_cast<quint8>(data[3]), 16).rightJustified(2, '0').toUpper());

        if (opcode == 0xFC01 && data.size() >= 10) {
            result += QString(": %1 %2 %3 %4 %5 %6")
            .arg(QString::number(static_cast<quint8>(data[4]), 16).rightJustified(2, '0').toUpper())
                .arg(QString::number(static_cast<quint8>(data[5]), 16).rightJustified(2, '0').toUpper())
                .arg(QString::number(static_cast<quint8>(data[6]), 16).rightJustified(2, '0').toUpper())
                .arg(QString::number(static_cast<quint8>(data[7]), 16).rightJustified(2, '0').toUpper())
                .arg(QString::number(static_cast<quint8>(data[8]), 16).rightJustified(2, '0').toUpper())
                .arg(QString::number(static_cast<quint8>(data[9]), 16).rightJustified(2, '0').toUpper());
        }

        result += QString("\nopcode = 0x%1 (%2, \"%3\")")
                      .arg(QString::number(opcode, 16).toUpper())
                      .arg(opcode)
                      .arg(name);

        if (opcode == 0xFC01 && data.size() >= 10) {
            QString bdaddr = QString("%1%2%3%4%5%6")
            .arg(QString::number(static_cast<quint8>(data[9]), 16).rightJustified(2, '0'))
                .arg(QString::number(static_cast<quint8>(data[8]), 16).rightJustified(2, '0'))
                .arg(QString::number(static_cast<quint8>(data[7]), 16).rightJustified(2, '0'))
                .arg(QString::number(static_cast<quint8>(data[6]), 16).rightJustified(2, '0'))
                .arg(QString::number(static_cast<quint8>(data[5]), 16).rightJustified(2, '0'))
                .arg(QString::number(static_cast<quint8>(data[4]), 16).rightJustified(2, '0'));
            result += QString("\nBD_ADDR = \"%1\"").arg(bdaddr.toUpper());
        }

        return result;
    }

    return "Unknown HCI packet type";
}


QString SerialWorker::parseHCIResponse(const QByteArray &data)
{
    if (data.size() < 6) return "Unknown or incomplete HCI response";

    quint8 eventCode = static_cast<quint8>(data[0]);

    if (eventCode == 0x0E) {
        quint8 numPackets = static_cast<quint8>(data[2]);
        quint16 opcode = static_cast<quint8>(data[4]) << 8 | static_cast<quint8>(data[3]);
        quint8 status = static_cast<quint8>(data[5]);

        QString name;
        if (opcode == 0x0C03) {
            name = "Reset";
        } else if (opcode == 0x1009) {
            name = "Read_BD_ADDR";
        } else if (opcode == 0xFC01) {
            name = "Write_BD_ADDR";
        } else {
            name = "Unknown";
        }

        QString statusStr = (status == 0x00) ? "Success" : "Failure";
        QString timestamp = QDateTime::currentDateTime().toString("MM/dd/yy hh:mm:ss.zzz");

        QString result = QString("%1 COM%2@115200 <c %3\n"
                                 "HCI Command Complete Event\n"
                                 "COM%2@115200\n"
                                 "[0E %4 ]: ")
                             .arg(timestamp)
                             .arg(serial->portName().remove("COM"))
                             .arg(name)
                             .arg(QString::number(data.size() - 1, 16).rightJustified(2, '0').toUpper());

        for (int i = 2; i < data.size(); i++) {
            result += QString("%1 ")
            .arg(QString::number(static_cast<quint8>(data[i]), 16).rightJustified(2, '0').toUpper());
        }

        result += QString("\nevent = 0x0E (14,\"Command Complete\")\n"
                          "Num_HCI_Command_Packets = 0x%1 (%2)\n"
                          "Command_Opcode = 0x%3 (%4, \"%5\")\n"
                          "Status = 0x%6 (%7, \"%8\")")
                      .arg(QString::number(numPackets, 16).toUpper())
                      .arg(numPackets)
                      .arg(QString::number(opcode, 16).toUpper())
                      .arg(opcode)
                      .arg(name)
                      .arg(QString::number(status, 16).toUpper())
                      .arg(status)
                      .arg(statusStr);

        if (opcode == 0x1009 && data.size() >= 12) {
            QString bdaddr = QString("%1%2%3%4%5%6")
            .arg(QString::number(static_cast<quint8>(data[11]), 16).rightJustified(2, '0'))
                .arg(QString::number(static_cast<quint8>(data[10]), 16).rightJustified(2, '0'))
                .arg(QString::number(static_cast<quint8>(data[9]), 16).rightJustified(2, '0'))
                .arg(QString::number(static_cast<quint8>(data[8]), 16).rightJustified(2, '0'))
                .arg(QString::number(static_cast<quint8>(data[7]), 16).rightJustified(2, '0'))
                .arg(QString::number(static_cast<quint8>(data[6]), 16).rightJustified(2, '0'));
            result += QString("\nBD_ADDR = \"%1\"").arg(bdaddr.toUpper());
        }

        return result;
    }

    return "Unknown HCI event";
}
