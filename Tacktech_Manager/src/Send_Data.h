#ifndef SEND_DATA_H
#define SEND_DATA_H

#include <QTcpSocket>
#include <string>
#include "Start_Send_Data_Thread.h"

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

class Send_Data : public QTcpSocket
{
Q_OBJECT
    public:
        Send_Data(const QString&, quint16, std::string&);
        ~Send_Data();

    private:
		QTcpSocket *socket;
        std::string xml_string;
    signals:
		void upload_complete(Send_Data*);
	private slots:
		void send_data_to_server();
};

#endif // SEND_DATA_H
