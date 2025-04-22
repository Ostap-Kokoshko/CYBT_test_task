#ifndef SERIALWORKER_H
#define SERIALWORKER_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>


class SerialWorker : public QObject
{
    Q_OBJECT
public:
    explicit SerialWorker(QObject *parent = nullptr);
    ~SerialWorker();

public slots:
    void connectToPort(const QString &portName);
    void sendCommand(const QByteArray &data);
    void disconnectFromPort();

signals:
    void responseReceived(const QByteArray &data);
    void logMessage(const QString &text);

private slots:
    void onReadyRead();

private:
    QString parseHCICommand(const QByteArray &data);
    QString parseHCIResponse(const QByteArray &data);

    QSerialPort *serial;
    QByteArray buffer;
    QTimer *responseTimer;
};

#endif // SERIALWORKER_H
