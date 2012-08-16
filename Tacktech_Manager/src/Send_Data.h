#ifndef SEND_DATA_H
#define SEND_DATA_H

#include <QTcpSocket>
#include <string>
#include <boost/shared_ptr.hpp>
#include "Start_Send_Data_Thread.h"

#ifdef _SHOW_DEBUG_OUTPUT
#include <iostream>
#endif // _DEBUG
class Send_Data: public QTcpSocket
{
Q_OBJECT
public:
	Send_Data();
	~Send_Data();
	void send_new_data(const QString&, quint16, std::string&);
private:
	QTcpSocket *socket;
	std::string xml_string;
	QString data;
signals:
	void data_recieved(std::string xml_data);
private slots:
	void send_data_to_server();
	void startRead();
	void recieve_complete();
	void send_complete();
};

#endif // SEND_DATA_H
