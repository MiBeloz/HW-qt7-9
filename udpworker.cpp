#include "udpworker.h"


UDPworker::UDPworker(QObject *parent) : QObject(parent){}

/*!
 * @brief Метод инициализирует UDP сервер
 */
void UDPworker::InitSocket()
{
    serviceUdpSocket = new QUdpSocket(this);
    pDataUdpSocket = new QUdpSocket(this);
    /*
     * Соединяем присваиваем адрес и порт серверу и соединяем функцию
     * обраотчик принятых пакетов с сокетом
     */
    serviceUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT);
    pDataUdpSocket->bind(QHostAddress::LocalHost, BIND_PORT_STR);

    connect(serviceUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);
    connect(pDataUdpSocket, &QUdpSocket::readyRead, this, &UDPworker::readPendingDatagrams);
}

/*!
 * @brief Метод осуществляет обработку принятой датаграммы
 */
void UDPworker::ReadDatagram(QNetworkDatagram datagram)
{
    QByteArray data;
    data = datagram.data();

    QDataStream inStr(&data, QIODevice::ReadOnly);
    QDateTime dateTime;
    inStr >> dateTime;

    emit sig_sendToGUI(dateTime);
}

void UDPworker::ReadStrDatagram(QNetworkDatagram datagram)
{
    QByteArray data;
    data = datagram.data();

    QDataStream inStr(&data, QIODevice::ReadOnly);
    QString dateStr;
    inStr >> dateStr;

    emit sig_sendToGUI(dateStr);
}

/*!
 * @brief Метод осуществляет опередачу датаграммы
 */
void UDPworker::SendDatagram(QByteArray data)
{
    /*
     *  Отправляем данные на localhost и задефайненный порт
     */
    serviceUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT);
}

void UDPworker::SendStrDatagram(QByteArray data)
{
    pDataUdpSocket->writeDatagram(data, QHostAddress::LocalHost, BIND_PORT_STR);
}

/*!
 * @brief Метод осуществляет чтение датаграм из сокета
 */
void UDPworker::readPendingDatagrams(void)
{
    /*
     *  Производим чтение принятых датаграмм
     */
    if (sender() == serviceUdpSocket){
        while(serviceUdpSocket->hasPendingDatagrams()){
                QNetworkDatagram datagram = serviceUdpSocket->receiveDatagram();
                ReadDatagram(datagram);
        }
    }
    if (sender() == pDataUdpSocket){
        while(pDataUdpSocket->hasPendingDatagrams()){
                QNetworkDatagram datagram = pDataUdpSocket->receiveDatagram();
                ReadStrDatagram(datagram);
        }
    }
}
