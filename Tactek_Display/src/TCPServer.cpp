/* 
 * File:   TCPServer.cpp
 * Author: G-spot
 * 
 * Created on July 5, 2012, 12:18 AM
 */

#include "TCPServer.h"
/*
 TCPServer::TCPServer(QObject* parent): QObject(parent)
 {
 connect (&server, SIGNAL(newConneciton()), this, SLOT(acceptConnection()));
 }

 TCPServer::~TCPServer()
 {
 server.close();
 }

 TCPServer::acceptConnection()
 {
 client = server.nextPendingConnection();
 connect (client, SIGNAL(readyRead()), this, SLOT(startRead()));
 }

 TCPServer::startRead()
 {
 char buffer[1024] = {0};
 client->read(buffer, client->bytesAvailable());
 client->close();
 }
 */
TCPServer::TCPServer(QObject* parent) :
		QObject(parent)
{
	connect(&server, SIGNAL(newConnection()), this, SLOT(acceptConnection()));

	server.listen(QHostAddress::Any, 8888);
}

TCPServer::~TCPServer()
{
	server.close();
}

void TCPServer::acceptConnection()
{
	client = server.nextPendingConnection();

	connect(client, SIGNAL(readyRead()), this, SLOT(startRead()));
}

void TCPServer::startRead()
{
	char buffer[1024] =
	{ 0 };
	client->read(buffer, client->bytesAvailable());
	client->close();
}