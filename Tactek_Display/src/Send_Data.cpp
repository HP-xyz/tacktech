#include "Recieve_Data.h"

Send_Data::Send_Data (int socket_descriptor, std::string t_data, QObject *parent): QObject(parent)
{
    socket = new QTcpSocket();
    if (!socket->setSocketDescriptor(socket_sdescriptor))
    {
        emit error(socket->error());
        return;
    }
    data = t_data;
    connect(socket, SIGNAL(connected()), this, SLOT(send_data()));
}

Send_Data::~Send_Data ()
{
    delete socket;
}

void Send_Data::send_data()
{
    socket.write(data);
}