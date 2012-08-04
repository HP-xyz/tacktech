/*
 * File:   TCPClient.h
 * Author: G-spot
 *
 * Created on July 11, 2012, 7:23 PM
 */

#ifndef SEND_DATA_H
#define	SEND_DATA_H

#include <QTcpSocket>
#include <iostream>
#include <string>
#include <QObject>

class Send_Data : public QTcpSocket
{
Q_OBJECT
public:
    Send_Data (int, std::string);
    ~Send_Data();
signals:
void error(QTcpSocket::SocketError error);
private:
    int socket_descriptor;
    QTcpSocket *socket;
    std::string data;
private slots:
    void send_data ();
};
#endif //SEND_DATA_H
