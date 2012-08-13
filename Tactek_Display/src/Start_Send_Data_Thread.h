#ifndef START_SEND_DATA_THREAD_H
#define START_SEND_DATA_THREAD_H

#include <QThread>
#include "Send_Data.h"

class Start_Send_Data_Thread: public QThread
{
Q_OBJECT
public:
	Start_Send_Data_Thread(int socket_descriptor, std::string xml,
			QObject *parent);
	~Start_Send_Data_Thread();

	void run();signals:
	void error(QTcpSocket::SocketError error);
private:
	int socket_descriptor;
	std::string xml_string;
private slots:
	void bytes_written(qint64);
};

#endif
