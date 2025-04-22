#ifndef SERIALWORKERFACTORY_H
#define SERIALWORKERFACTORY_H

#include <QObject>
#include "serialworker.h"

class SerialWorkerFactory : public QObject
{
    Q_OBJECT
public:
    explicit SerialWorkerFactory(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void workerReady(SerialWorker *worker);

public slots:
    void createWorker()
    {
        SerialWorker *worker = new SerialWorker();
        emit workerReady(worker);
    }
};

#endif // SERIALWORKERFACTORY_H
