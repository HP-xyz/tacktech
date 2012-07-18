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

    client = 0;
    server.listen(QHostAddress::Any, 9001);
    data = "";
}

Recieve_Data::~Recieve_Data()
{
#ifdef _DEBUG
    std::cout << "= Recieve_Data::~Recieve_Data()" << std::endl;
#endif
    server.close();
    delete client;
}

void Recieve_Data::acceptConnection()
{
#ifdef _DEBUG
    std::cout << "= Recieve_Data::acceptConnection()" << std::endl;
#endif
    client = server.nextPendingConnection();
    connect(client, SIGNAL(readyRead()),
            this, SLOT(startRead()));
    connect(client, SIGNAL(readChannelFinished()),
        this, SLOT(recieve_complete()));
}

void Recieve_Data::startRead()
{
#ifdef _DEBUG
    std::cout << "= Recieve_Data::startRead()" << std::endl;
#endif
    QByteArray buffer = client->readAll();
#ifdef _DEBUG
    std::cout << " - Read size: " << buffer.size() << std::endl;
#endif
    data += QString(buffer);
}

void Recieve_Data::recieve_complete()
{
#ifdef _DEBUG
    std::cout << "Data recieved size: " << data.size() << std::endl;
#endif //_DEBUG
    emit data_recieved(data.toStdString());
}
