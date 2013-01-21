#include "Start_Send_Data_Thread.h"

Start_Send_Data_Thread::Start_Send_Data_Thread(int p_socket_descriptor,
		std::string xml, QObject *parent) :
		QThread(parent)
{
	socket_descriptor = p_socket_descriptor;
	xml_string = xml;
}

Start_Send_Data_Thread::~Start_Send_Data_Thread()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Start_Send_Data_Thread::~Start_Send_Data_Thread()"
			<< std::endl;
#endif // _DEBUG
}

void Start_Send_Data_Thread::run()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Start_Send_Data_Thread::run()" << std::endl;
#endif // _DEBUG
	QTcpSocket socket;
	connect(&socket, SIGNAL(bytesWritten(qint64)), this,
			SLOT(bytes_written(qint64)));
	if (!socket.setSocketDescriptor(socket_descriptor))
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Error: " << socket.error() << std::endl;
#endif // _DEBUG
		emit error(socket.error());
		return;
	}
	/** Here we add the delimiter for the end of the stream */
	xml_string += ";";
	/** The following line does the actual sending */
	qint64  bytes_written_var =
		socket.write(xml_string.c_str(), xml_string.size());
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Sending data " << std::endl;
	std::cout << " - Filesize: " << xml_string.size() << std::endl;
	std::cout << " - Bytes to write: " << socket.bytesToWrite() << std::endl;
	std::cout << " - Waiting for bytes written" << std::endl;
#endif // _DEBUG
	socket.waitForBytesWritten();
}

void Start_Send_Data_Thread::bytes_written(qint64 bytes)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "Bytes written: " << bytes << std::endl;
#endif // _DEBUG
}
