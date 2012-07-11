#ifndef SEND_DATA_H
#define SEND_DATA_H

#include <QThread>
#include <QTcpSocket>
#include <QSharedPointer>
#include <string>
#include <QByteArray>
#include <QDataStream>
#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

class Send_Data : public QObject
{
   Q_OBJECT
public:
	Send_Data(QObject* parent = 0);
	~Send_Data();
	void start(QString address, quint16 port, std::string&);
public slots:
	void startTransfer();
	void show_bytes_written(qint64);
private:
	QTcpSocket client;
	QString block;
};

#endif // SEND_DATA_H
