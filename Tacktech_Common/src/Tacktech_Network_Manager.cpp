#include "Tacktech_Network_Manager.h"

Tacktech_Network_Manager::Tacktech_Network_Manager(
	boost::asio::io_service &io_service,
	std::map<std::string, std::string>& parameters): strand(io_service)
{
	parms = parameters;
	m_socket.reset(new boost::asio::ip::tcp::socket(io_service));
}

Tacktech_Network_Manager::~Tacktech_Network_Manager()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tacktech_Network_Manager::"
			<< "~Tacktech_Network_Manager()" << std::endl;
#endif // _DEBUG
}

void Tacktech_Network_Manager::start_write(
	boost::shared_ptr<std::string> _xml_string )
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " =Tacktech_Network_Manager::start_write()"
		<< std::endl;
#endif //_DEBUG
	xml_string = _xml_string;
}

void Tacktech_Network_Manager::do_write()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " =Tacktech_Network_Manager::do_write()" 
		<< std::endl;
	std::cout << " ->>> Socket is currently open: " 
		<< m_socket->is_open() << std::endl;
	std::cout << " - Writing size: " << xml_string->size() << std::endl;
#endif //_DEBUG
	xml_string->append(";");
	boost::asio::async_write(*m_socket,
			boost::asio::buffer(xml_string->c_str(), xml_string->size()),
			boost::bind(&Tacktech_Network_Manager::handle_write,
			this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
}
void Tacktech_Network_Manager::handle_write(
		const boost::system::error_code& error,
		std::size_t bytes_transferred)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " == Tacktech_Network_Manager::Handle Write" 
		<< std::endl << " ==============="
			<< std::endl;
	std::cout << " - Bytes_Transferred: " << bytes_transferred 
		<< std::endl;
#endif
	if (error)
	{
		std::cerr << " - Error encountered: " << error.message() 
			<< std::endl;
	}
	else
	{
		/** Start waiting/reading until the return message is recieved */
		read();
	}
}
void Tacktech_Network_Manager::connect(std::string dest_ip,
		std::string dest_port)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tacktech_Network_Manager::connect()" 
		<< std::endl;
	std::cout << " ->> Dest IP:   " << dest_ip << std::endl;
	std::cout << " ->> Dest PORT: " << dest_port << std::endl;
	std::cout << " - Creating resolver" << std::endl;
#endif // _DEBUG
	boost::asio::ip::tcp::resolver resolver(m_socket->get_io_service());
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Creating query" << std::endl;
#endif //_DEBUG
	boost::asio::ip::tcp::resolver::query query(dest_ip, dest_port);
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Creating endpoint_iterator" << std::endl;
#endif //_DEBUG
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator =
			resolver.resolve(query);
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Calling async_connect" << std::endl;
#endif //_DEBUG
	m_socket->get_io_service().post(
			boost::bind(&Tacktech_Network_Manager::do_connect,
					this,
					endpoint_iterator));
}
void Tacktech_Network_Manager::do_connect(
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tacktech_Network_Manager::do_connect()"
			<< std::endl;
#endif // _DEBUG
	boost::asio::async_connect(*m_socket, endpoint_iterator,
			boost::bind(&Tacktech_Network_Manager::handle_connect,
					this, boost::asio::placeholders::error));
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Socket is open: " << m_socket->is_open() << std::endl;
#endif // _DEBUG
}
void Tacktech_Network_Manager::handle_connect(
		const boost::system::error_code &error)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tacktech_Network_Manager::handle_connect()"
			<< std::endl;
#endif // _DEBUG
	if (!error)
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Connected without error" << std::endl;
		std::cout << " - Starting write" << std::endl;
#endif //_DEBUG
		do_write();
	}
	else
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cerr << "Error: " << error.message() << std::endl;
#endif
	}
}
void Tacktech_Network_Manager::do_close()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tacktech_Network_Manager::do_close()" 
		<< std::endl;
#endif // _DEBUG
	m_socket->close();
}

void Tacktech_Network_Manager::read()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tacktech_Network_Manager::read()" << std::endl;
#endif // _DEBUG
	boost::asio::async_read_until(*m_socket, buffer, ';',
		boost::bind(&Tacktech_Network_Manager::handle_read,
		shared_from_this(), boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}

void Tacktech_Network_Manager::handle_read(
	const boost::system::error_code& error, std::size_t bytes_transferred )
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " == Tacktech_Network_Manager::Handle Read" << std::endl << " =============="
		<< std::endl << " - Bytes transferred before error check: "
		<< bytes_transferred << std::endl;
	std::cout << " --> Recieved from IP: " 
		<< m_socket->remote_endpoint().address().to_string() 
		<< std::endl;
#endif // _DEBUG
	/** Constructing the recieved message to std::string*/
	std::vector<char> xml;
	xml.reserve(bytes_transferred + 1);
	for (unsigned int i = 0; i < bytes_transferred; i++)
	{
		xml.push_back('5');
	}
	xml.push_back(NULL);
	buffer.sgetn(&xml[0], bytes_transferred);
	std::string received_xml;
	for (unsigned int i = 0; i < bytes_transferred; i++)
	{
		received_xml += xml[i];
	}
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Closing" << std::endl;
#endif // _DEBUG
	//Initiate graceful connection closure.
	boost::system::error_code ignored_ec;
	Tacktech_Network_Manager::m_socket->shutdown(
		boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Emitting data_recieved signal" << std::endl;
#endif
	emit data_recieved(QString::fromStdString(received_xml));
}
