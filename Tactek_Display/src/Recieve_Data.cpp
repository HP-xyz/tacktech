/*
 * File:   TCPServer.cpp
 * Author: G-spot
 *
 * Created on July 5, 2012, 12:18 AM
 */

#include "Recieve_Data.h"
  Recieve_Data::Recieve_Data(QObject* parent): QObject(parent)
{
#ifdef _DEBUG
    std::cout << "= Recieve_Data::Recieve_Data()" << std::endl;
#endif
  connect(&server, SIGNAL(newConnection()),
    this, SLOT(acceptConnection()));

  server.listen(QHostAddress::Any, 9001);
}

Recieve_Data::~Recieve_Data()
{
  server.close();
}

void Recieve_Data::acceptConnection()
{
#ifdef _DEBUG
    std::cout << "= Recieve_Data::acceptConnection()" << std::endl;
#endif
  client = server.nextPendingConnection();
  connect(client, SIGNAL(readyRead()),
    this, SLOT(startRead()));
}

void Recieve_Data::startRead()
{
#ifdef _DEBUG
    std::cout << "= Recieve_Data::startRead()" << std::endl;
#endif
  char buffer[8092] = {0};
  client->read(buffer, client->bytesAvailable());
  std::cout << buffer << std::endl;
  client->close();
  std::string data = buffer;
  emit data_recieved(data);
}
