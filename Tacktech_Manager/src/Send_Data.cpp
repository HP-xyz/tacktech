#include "Send_Data.h"
#include <QHostAddress>

Send_Data::Send_Data(QObject* parent): QObject(parent)
{
  connect(&client, SIGNAL(connected()),
    this, SLOT(startTransfer()));
  connect(&client, SIGNAL(bytesWritten(quint16)),
	  this, SLOT(show_bytes_written(quint16)));
}

Send_Data::~Send_Data()
{
  client.close();
}

void Send_Data::start(QString address, quint16 port, std::string& xml)
{
  QHostAddress addr(address);
  client.connectToHost(addr, port);

  block = QString::fromStdString(xml);
  //QDataStream out (&block, QIODevice::WriteOnly);
  //out.setVersion(QDataStream::Qt_4_8);
  //std::cout << qPrintable(QString::fromStdString(xml)) << std::endl;
  //out << QString::fromStdString(xml);
}

void Send_Data::startTransfer()
{
  client.write(block.toStdString().c_str());
  client.disconnect();
  client.waitForDisconnected();
}

void Send_Data::show_bytes_written( qint64 bytes)
{
	std::cout << "Bytes written: " << bytes << std::endl; 
}
