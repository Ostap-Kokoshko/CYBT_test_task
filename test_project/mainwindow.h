#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include "serialworker.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_resetButton_clicked();
    void on_readBDADDRButton_clicked();
    void on_writeBDADDRButton_clicked();
    void handleResponse(const QByteArray &data);
    void on_disconnectButton_clicked();

private:
    Ui::MainWindow *ui;
    QThread *workerThread;
    SerialWorker *worker;

    void log(const QString &text);
    void connectToPort();
};
