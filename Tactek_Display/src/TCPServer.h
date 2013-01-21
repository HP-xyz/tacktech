/* 
 * File:   TCPServer.h
 * Author: G-spot
 *
 * Created on July 5, 2012, 12:18 AM
 */
/*
 * 
 * #include <QtNetwork>
 #include <QObject>
 #include <QTcpServer>
 #include <QTcpSocket>

 class TCPServer : public QObject
 {
 Q_OBJECT
 public:
 TCPServer(QObject * parent = 0);
 ~TCPServer();
 public slots:
 void acceptConnection();
 void startRead();
 private:
 QTcpServer server;
 QTcpSocket* client;
 };
 * 
 */

#ifndef TCPSERVER_H
#define	TCPSERVER_H

#include <QtNetwork>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class TCPServer: public QObject
{
Q_OBJECT
public:
	TCPServer(QObject * parent = 0);
	~TCPServer();
public slots:
	void acceptConnection();
	void startRead();
private:
	QTcpServer server;
	QTcpSocket* client;
};

#endif	/* TCPSERVER_H */
