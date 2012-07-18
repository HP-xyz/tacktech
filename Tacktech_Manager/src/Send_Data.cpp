#include "Send_Data.h"

/** @brief ~Send_Data
  *
  * Constructor
  */
Send_Data::Send_Data(const QString &p_host_name,
	quint16 p_port, std::string &p_xml_string)
{
	xml_string = p_xml_string;
	socket = new QTcpSocket();
	connect(socket, SIGNAL(connected()), this, SLOT(send_data_to_server()));
	socket->connectToHost(p_host_name, p_port);
}

/** @brief Send_Data
  *
  * Default Destructor
  */
 Send_Data::~Send_Data()
 {
#ifdef _DEBUG
	 std::cout << "destructor of send data" << std::endl;
#endif // _DEBUG
	 socket->close();
 }

/** @brief send_data_to_server
  *
  * Sets up the connection settings and
  * calls the run function if the class
  * is not already running.
  */
void Send_Data::send_data_to_server()
{
#ifdef _DEBUG
	std::cout << "= Send_Data::send_data_to_server()" << std::endl;
	std::cout << " - Sending file of size: " << xml_string.size() << std::endl;
#endif // _DEBUG
	Start_Send_Data_Thread *ssdt = new Start_Send_Data_Thread(socket->socketDescriptor(), xml_string, this);
	connect(ssdt, SIGNAL(finished()), ssdt, SLOT(deleteLater()));
	connect(ssdt, SIGNAL(finished()), this, SIGNAL(upload_complete(this)));
	ssdt->start();
}
