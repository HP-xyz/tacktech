#include "Start_Send_Data_Thread.h"

Start_Send_Data_Thread::Start_Send_Data_Thread 
	(int p_socket_descriptor, 
	 std::string xml, 
	 QObject *parent)
	 : QThread(parent)
{
	socket_descriptor = p_socket_descriptor;
	xml_string = xml;
}

Start_Send_Data_Thread::~Start_Send_Data_Thread()
{

}

void Start_Send_Data_Thread::run()
{
#ifdef _DEBUG
	std::cout << "= Start_Send_Data_Thread::run()" << std::endl;
#endif // _DEBUG
	QTcpSocket socket;
	if (!socket.setSocketDescriptor(socket_descriptor))
	{
		emit error(socket.error());
		return;
	}
	
	socket.write(xml_string.c_str());
	socket.disconnectFromHost();
	socket.waitForDisconnected();
}
