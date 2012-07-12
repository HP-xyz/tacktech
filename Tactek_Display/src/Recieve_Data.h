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
public slots:
  void acceptConnection();
  void startRead();
private:
  QTcpServer server;
  QTcpSocket* client;
signals:
    void data_recieved(std::string xml_data);
};


#endif	/* TCPSERVER_H */
