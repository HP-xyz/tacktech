/*
 * File:   TCPServer.h
 * Author: G-spot
 *
 * Created on July 5, 2012, 12:18 AM
 */

#ifndef SEND_DATA_H
#define	SEND_DATA_H

#include <QtNetwork>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <iostream>
#include <string>

class Recieve_Data: public QObject
{
Q_OBJECT
public:
  Recieve_Data(QObject * parent = 0);
  ~Recieve_Data();
private:
  QTcpServer server;
  QTcpSocket* client;
  QString data;
signals:
    void data_recieved(std::string xml_data);
private slots:
    void acceptConnection();
    void startRead();
    void recieve_complete();
};


#endif	/* TCPSERVER_H */
