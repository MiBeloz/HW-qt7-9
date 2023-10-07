#ifndef UDPWORKER_H
#define UDPWORKER_H

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QDateTime>


#define BIND_PORT 12345
#define BIND_PORT_STR 54321

class UDPworker : public QObject
{
    Q_OBJECT
public:
    explicit UDPworker(QObject *parent = nullptr);
    void InitSocket(void);
    void ReadDatagram(QNetworkDatagram datagram);
    void ReadStrDatagram(QNetworkDatagram datagram);
    void SendDatagram(QByteArray data);
    void SendStrDatagram(QByteArray data);

private slots:
    void readPendingDatagrams(void);

private:
    QUdpSocket* serviceUdpSocket;
    QUdpSocket *pDataUdpSocket;

signals:
    void sig_sendToGUI(QDateTime data);
    void sig_sendToGUI(QString data);
};

#endif // UDPWORKER_H
