/* 
 * File:   TCPServer.cpp
 * Author: G-spot
 * 
 * Created on July 5, 2012, 12:18 AM
 */

#include "Recieve_Data.h"
  Recieve_Data::Recieve_Data(QObject* parent): QObject(parent)
{
  connect(&server, SIGNAL(newConnection()),
    this, SLOT(acceptConnection()));

  server.listen(QHostAddress::Any, 8888);
}

Recieve_Data::~Recieve_Data()
{
  server.close();
}

void Recieve_Data::acceptConnection()
{
  client = server.nextPendingConnection();

  connect(client, SIGNAL(readyRead()),
    this, SLOT(startRead()));
}

void Recieve_Data::startRead()
{
  char buffer[1024] = {0};
  client->read(buffer, client->bytesAvailable());
  cout >> buffer >> endl;
  client->close();
}