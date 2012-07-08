#include "Send_Data.h"

/** @brief ~Send_Data
  *
  * Default Constructor
  */
 Send_Data::~Send_Data()
{

}

/** @brief Send_Data
  *
  * Default Destructor
  */
 Send_Data::Send_Data()
{

}
/** @brief send_data_to_server
  *
  * Sets up the connection settings and
  * calls the run function if the class
  * is not already running.
  */
void Send_Data::send_data_to_server(
    const QString &p_host_name,
     quint16 p_port, std::string &p_xml_string)
{
#ifdef _DEBUG
	std::cout << "= Send_Data::send_data_to_server()" << std::endl;
	std::cout << " - p_xml_string: " << p_xml_string << std::endl;
#endif // _DEBUG
    QMutexLocker locker(&mutex);
    xml_string = p_xml_string;
    this->host_name = p_host_name;
    this->port = p_port;
    if (!isRunning())
	{
#ifdef _DEBUG
		std::cout << " - Is not running -> starting" << std::endl;
#endif // _DEBUG
        start();
	}
    else
	{
#ifdef _DEBUG
		std::cout << " - Is running -> waiting" << std::endl;
#endif // _DEBUG
        cond.wakeOne();
	}
}

/** @brief run
  *
  * Run fuction. Starts the actual connection
  * to the server and sends the data
  */
void Send_Data::run()
{
#ifdef _DEBUG
	std::cout << "= Send_Data::run()" << std::endl;
#endif // _DEBUG
    mutex.lock();
    QString server_name = host_name;
    quint16 server_port = port;
    mutex.unlock();

    //while (!quit)
    //{
#ifdef _DEBUG
		std::cout << " - Not QUIT" << std::endl;
#endif // _DEBUG
        //TODO
        //ADD CONFIGURABLE TIMEOUT HERE
        const int timeout = 1000 * 1000;
        QTcpSocket socket;
        socket.connectToHost(server_name, server_port);

        emit status_changed("Connecting to server");
        if (!socket.waitForConnected(timeout))
        {
#ifdef _DEBUG
			std::cout << " - Error encountered" << std::endl;
			std::cout << "  - Error: " << qPrintable(socket.errorString())
				<< std::endl;
#endif // _DEBUG
            emit error(socket.error(), socket.errorString());
            return;
        }
        QByteArray block;
        QDataStream out (&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);

#ifdef _DEBUG
		std::cout << " - xml_string: " << xml_string << std::endl;
		std::cout << " - QSTring - xml_string: " << qPrintable(QString::fromStdString(xml_string)) << std::endl;
#endif // _DEBUG

        out << (quint16)0;
        out << QString::fromStdString(xml_string);
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));

        connect (&socket, SIGNAL(disconnected()),
                 &socket, SLOT(deleteLater()));
#ifdef _DEBUG
		std::cout << " - Writing to server" << std::endl;
		std::cout << " - Block contains: " << std::endl;
		for (int i = 0 ; i < block.size(); i++)
		{
			std::cout << block.at(i);
		}
		std::cout << std::endl;
#endif // _DEBUG
        emit status_changed("Writing to server");
        socket.write(block);
        socket.disconnectFromHost();
    //}
}

