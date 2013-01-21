/*
 * File:   TCPServer.h
 * Author: G-spot
 *
 * Created on July 5, 2012, 12:18 AM
 */

#ifndef RECIEVE_DATA_H
#define	RECIEVE_DATA_H

#include <QtNetwork>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

class Recieve_Data: public QObject
{
Q_OBJECT
public:
	Recieve_Data(std::map<std::string, std::string> params);
	~Recieve_Data();
private:
	QTcpServer server;
	QTcpSocket* client;
	QString data;
	std::map<std::string, std::string> parameters;
signals:
	void data_recieved(std::string xml_data);
private slots:
	void acceptConnection();
	void startRead();
	void recieve_complete();
};

#endif	/* RECIEVE_DATA_H */
