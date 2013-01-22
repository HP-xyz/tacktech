#include "Send_Data.h"

/** @brief ~Send_Data
 *
 * Constructor
 */
Send_Data::Send_Data()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "Send_Data::Send_Data()" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
}

void Send_Data::send_new_data( 
	const QString& p_host_name,
	quint16 p_port,
	std::string& p_xml_string)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "Send_Data::send_new_data()" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	xml_string = p_xml_string;
	socket = new QTcpSocket();
	connect(socket, SIGNAL(connected()), this, SLOT(send_data_to_server()));
	connect(socket, SIGNAL(readyRead()), this, SLOT(startRead()));
	connect(socket, SIGNAL(readChannelFinished()), this,
		SLOT(recieve_complete()));
	socket->connectToHost(p_host_name, p_port);
}


/** @brief Send_Data
 *
 * Default Destructor
 */
Send_Data::~Send_Data()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "Send_Data::~Send_Data()" << std::endl;
#endif // _DEBUG
}

/** @brief send_data_to_server
 *
 * Sets up the connection settings and
 * calls the run function if the class
 * is not already running.
 */
void Send_Data::send_data_to_server()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Send_Data::send_data_to_server()" << std::endl;
	std::cout << " - Sending file of size: " << xml_string.size() << std::endl;
#endif // _DEBUG
	Start_Send_Data_Thread *send_data_thread = (
		new Start_Send_Data_Thread(
			socket->socketDescriptor(), xml_string, this));
	send_data_thread->start();
	connect(send_data_thread, SIGNAL(finished()),
		this, SLOT(send_complete()));
	connect(send_data_thread, SIGNAL(finished()),
		this, SLOT(deleteLater()));
	connect(send_data_thread, SIGNAL(finished()),
		this, SLOT(startRead()));
}

void Send_Data::send_complete()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Send_Data::send_complete()" << std::endl;
#endif
}

void Send_Data::startRead()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Send_Data::startRead()" << std::endl;
	std::cout << "  - Bytes available: " << socket->bytesAvailable()
		<< std::endl;
#endif
	QByteArray buffer = socket->readAll();
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Read size: " << buffer.size() << std::endl;
#endif
	data += QString(buffer);
}

void Send_Data::recieve_complete()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "Data recieved size: " << data.size() << std::endl;
#endif //_DEBUG
	emit data_recieved(data.toStdString());
	data = QString();
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Disconnecting from host" << std::endl;
#endif // _DEBUG
	socket->disconnectFromHost();
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Waiting for disconnect" << std::endl;
#endif // _DEBUG
	socket->waitForDisconnected();
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Disconnected" << std::endl;
#endif // _DEBUG
	socket->close();
}
