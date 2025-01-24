#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      mqttClient(new QMqttClient(this)) {
    ui->setupUi(this);

    // Configuration du broker MQTT
    mqttClient->setHostname("localhost"); // Adresse du broker
    mqttClient->setPort(1883);            // Port du broker MQTT
    mqttClient->setClientId("IHM_Vitesse");

    // Connexion des signaux
    connect(mqttClient, &QMqttClient::messageReceived, this, &MainWindow::onMessageReceived);
    connect(mqttClient, &QMqttClient::stateChanged, this, &MainWindow::onBrokerStateChanged);

    // Connexion au broker MQTT
    mqttClient->connectToHost();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onMessageReceived(const QByteArray &message, const QMqttTopicName &topic) {
    if (topic.name() == "vitesse") {
        QString vitesse = QString::fromUtf8(message);
        ui->lcdVitesse->display(vitesse); // Affichage de la vitesse dans le LCD
        qDebug() << "Message reçu sur le topic 'vitesse':" << vitesse;
    }
}

void MainWindow::onBrokerStateChanged(QMqttClient::ClientState state) {
    switch (state) {
    case QMqttClient::Disconnected:
        ui->statusLabel->setText("Déconnecté");
        break;
    case QMqttClient::Connecting:
        ui->statusLabel->setText("Connexion en cours...");
        break;
    case QMqttClient::Connected:
        ui->statusLabel->setText("Connecté");
        QMqttTopicFilter topicVitesse("vitesse");
        mqttClient->subscribe(topicVitesse); // Abonnement au topic "vitesse"
        break;
    }
}
