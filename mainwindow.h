#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMqttClient>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onMessageReceived(const QByteArray &message, const QMqttTopicName &topic);
    void onBrokerStateChanged(QMqttClient::ClientState state);

private:
    Ui::MainWindow *ui;
    QMqttClient *mqttClient;
};

#endif // MAINWINDOW_H
