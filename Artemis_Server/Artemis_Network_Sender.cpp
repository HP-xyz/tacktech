#include "Artemis_Network_Sender.h"

Artemis_Network_Sender::Artemis_Network_Sender(
	boost::asio::io_service &p_io_service,
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator) :
	m_io_service(p_io_service), m_socket(p_io_service)
{
#ifdef _DEBUG
	std::cout << "= Artemis_Network_Sender::Artemis_Network_Sender()" << std::endl;
#endif // _DEBUG
	boost::asio::async_connect(m_socket, endpoint_iterator,
		boost::bind(&Artemis_Network_Sender::handle_connect,
		this, boost::asio::placeholders::error));
}


Artemis_Network_Sender::~Artemis_Network_Sender(void)
{
}

void Artemis_Network_Sender::do_write( std::string xml_string )
{
#ifdef _DEBUG
	std::cout << "= Artemis_Network_Sender::do_write()" << std::endl;
#endif // _DEBUG
	boost::asio::async_write(m_socket,
		boost::asio::buffer(xml_string, xml_string.size()),
		boost::bind(&Artemis_Network_Sender::handle_write, this, 
		boost::asio::placeholders::error));
}

void Artemis_Network_Sender::handle_write( 
	const boost::system::error_code& error )
{
#ifdef _DEBUG
	std::cout << "= Artemis_Network_Sender::handle_write()" << std::endl;
#endif // _DEBUG
	if(!error)
	{
#ifdef _DEBUG
		std::cout << " - Sent successfully" << std::endl;
#endif // _DEBUG
	}
	else
	{
		std::cerr << "Error: " << error.message() << std::endl;
		do_close();
	}
}

void Artemis_Network_Sender::do_close()
{
#ifdef _DEBUG
	std::cout << "= Artemis_Network_Sender::do_close()" << std::endl;
#endif // _DEBUG
	m_socket.close();
}

void Artemis_Network_Sender::write( const std::string xml_string)
{
#ifdef _DEBUG
	std::cout << "= Artemis_Network_Sender::write()" << std::endl;
#endif // _DEBUG
	m_io_service.post(
		boost::bind(&Artemis_Network_Sender::do_write, this, xml_string));
}

void Artemis_Network_Sender::close()
{
#ifdef _DEBUG
	std::cout << "= Artemis_Network_Sender::close()" << std::endl;
#endif // _DEBUG
	m_io_service.post(boost::bind(&Artemis_Network_Sender::do_close, this));
}

void Artemis_Network_Sender::handle_connect( const boost::system::error_code &error )
{
#ifdef _DEBUG
	std::cout << "= Artemis_Network_Sender::handle_connect()" << std::endl;
#endif // _DEBUG
	if(error)
	{
		std::cerr << "Error: " << error.message() << std::endl; 
	}
}


