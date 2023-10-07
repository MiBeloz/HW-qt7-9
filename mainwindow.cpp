#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "udpworker.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pb_stop->setEnabled(false);

    udpWorker = new UDPworker(this);
    udpWorker->InitSocket();

    connect(udpWorker, qOverload<QDateTime>(&UDPworker::sig_sendToGUI), this, qOverload<QDateTime>(&MainWindow::Display));
    connect(udpWorker, qOverload<QString>(&UDPworker::sig_sendToGUI), this, qOverload<QString>(&MainWindow::Display));

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [&]{

        QDateTime dateTime = QDateTime::currentDateTime();

        QByteArray dataToSend;
        QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

        outStr << dateTime;

        udpWorker->SendDatagram(dataToSend);
        timer->start(TIMER_DELAY);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pb_start_clicked()
{
    timer->start(TIMER_DELAY);
    ui->pb_start->setEnabled(false);
    ui->pb_stop->setEnabled(true);
}

void MainWindow::Display(QDateTime data)
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }

    ui->te_result->append("Текущее время: " + data.toString() + ". "
                "Принято пакетов " + QString::number(counterPck));
}

void MainWindow::Display(QString data)
{
    counterPck++;
    if(counterPck % 20 == 0){
        ui->te_result->clear();
    }

    QByteArray ba_data = data.toUtf8();

    ui->te_result->append("Принято сообщение от: " + QString::number(BIND_PORT_STR) + ". "
                          "Размер сообщения(байт): " + QString::number(ba_data.capacity()));
}

void MainWindow::on_pb_stop_clicked()
{
    timer->stop();
    ui->pb_start->setEnabled(true);
    ui->pb_stop->setEnabled(false);
}

void MainWindow::on_pb_sendDatagram_clicked()
{
    QByteArray dataToSend;
    QDataStream outStr(&dataToSend, QIODevice::WriteOnly);

    outStr << ui->le_textDatagram->text();

    udpWorker->SendStrDatagram(dataToSend);
}

