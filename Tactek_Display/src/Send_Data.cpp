#include "Send_Data.h"

Send_Data::Send_Data (int socket_descriptor, std::string t_data)
{
    socket = new QTcpSocket();
    if (!socket->setSocketDescriptor(socket_descriptor))
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
    socket->write(data.c_str());
    socket->disconnectFromHost();
    socket->waitForDisconnected();
}
