/*
 * File:   Artemis_Server_Connection.cpp
 * Author: dvorak
 *
 * Created on September 11, 2011, 12:30 PM
 */

#include "Artemis_Server_Connection.h"
#include "Artemis_Server.h"
namespace Artemis
{
bool string_allowed_char_predicate(char c)
{
	if (!(isalnum(c) || ispunct(c) || iscntrl(c)))
		return true;
	else
		return false;
}

/** Struct to define a xml_writer to string.
 ** Copied directly from the pugixml quickstart */
struct xml_string_writer: pugi::xml_writer
{
	std::string result;
	virtual void write(const void* data, std::size_t size)
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - (XML_WRITER)write of size: " << size << std::endl;
#endif // _DEBUG
		result += static_cast<const char*>(data);
		//result += std::string(static_cast<const char*>(data),
		//	size);
	}
};
//************************************
// Method:    Artemis_Server_Connection
// FullName:  Artemis::Artemis_Server_Connection::Artemis_Server_Connection
// Access:    public
// Returns:
// Qualifier: : strand(io_service), m_socket(io_service)
// Parameter: boost::asio::io_service & io_service
// Parameter: std::map<std::string
// Parameter: std::string> & parameters
//************************************
Artemis_Server_Connection::Artemis_Server_Connection(
		boost::asio::io_service& io_service,
		std::map<std::string, std::string>& parameters,
		Group_Container_Server_Ptr p_groups_and_computers,
		Playlist_Container_Ptr p_playlist_container,
		Group_Playlist_Container_Server_Ptr p_group_playlist,
		Display_Client_Container_Ptr p_display_client_container) :
		strand(io_service)
{
	m_socket.reset(new boost::asio::ip::tcp::socket(io_service));
	Artemis_Server_Connection::parms = parameters;
	Artemis_Server_Connection::groups_and_computers = p_groups_and_computers;
	Artemis_Server_Connection::playlist_container = p_playlist_container;
	Artemis_Server_Connection::group_playlist = p_group_playlist;
	Artemis_Server_Connection::display_client_container = p_display_client_container;
	received_size = 0;
	return_xml.reset(new std::string());
}

//************************************
// Method:    socket
// FullName:  Artemis::Artemis_Server_Connection::socket
// Access:    public
// Returns:   boost::asio::ip::tcp::socket&
// Qualifier:
//************************************
boost::asio::ip::tcp::socket& Artemis_Server_Connection::socket()
{
	return (*Artemis_Server_Connection::m_socket);
}

//************************************
// Method:    start
// FullName:  Artemis::Artemis_Server_Connection::start
// Access:    public
// Returns:   void
// Qualifier:
//************************************
void Artemis_Server_Connection::start()
{
	boost::asio::async_read_until(*m_socket, buffer, ';',
			boost::bind(&Artemis_Server_Connection::handle_read,
					shared_from_this(), boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
}

//************************************
// Method:    handle_read
// FullName:  Artemis::Artemis_Server_Connection::handle_read
// Access:    private
// Returns:   void
// Qualifier:
// Parameter: const boost::system::error_code & error
// Parameter: std::size_t bytes_transferred
//************************************
void Artemis_Server_Connection::handle_read(
		const boost::system::error_code& error,
		std::size_t bytes_transferred)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " == Handle Read" << std::endl << " =============="
			<< std::endl << " - Bytes transferred before error check: "
			<< bytes_transferred << std::endl;
#endif
	if (!error)
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " --> Recieved from IP: " 
			<< m_socket->remote_endpoint().address().to_string() 
			<< std::endl;
#endif // _DEBUG
		std::vector<char> xml;
		xml.reserve(bytes_transferred + 1);
		for (unsigned int i = 0; i < bytes_transferred; i++)
		{
			xml.push_back('5');
		}
		xml.push_back('\0');
		buffer.sgetn(&xml[0], bytes_transferred);
		std::string received_xml;
		for (unsigned int i = 0; i < bytes_transferred; i++)
		{
			received_xml += xml[i];
		}

		boost::shared_ptr<Artemis_Request_Handler> request_handler(
			new Artemis_Request_Handler(groups_and_computers, playlist_container,
			group_playlist, display_client_container));
		request_handler->handle_request(received_xml, return_xml,
			Artemis_Server_Connection::parms);
		try
		{
			if (request_handler->result_status == NO_RESULT)
			{
#ifdef _SHOW_DEBUG_OUTPUT
				std::cout << "	Query generated no results" << std::endl;
#endif // _DEBUG
			}
			else if (request_handler->result_status == SINGLE_RESULT)
			{
#ifdef _SHOW_DEBUG_OUTPUT
				std::cout << "= Artemis_Server_Connection::handle_read()" 
					<< std::endl;
				std::cout << " ->>>> Dest_Ip: " 
					<< m_socket->remote_endpoint().address().to_string() 
					<< std::endl;
				std::cout << " ->>> Socket is currently open: " 
					<< m_socket->is_open() << std::endl;
				std::cout << " - Sending file of size: " 
					<< return_xml->size() << std::endl;
#endif // _DEBUG
				return_xml->append(";");
				boost::asio::async_write(*m_socket,
						boost::asio::buffer(return_xml->c_str(), return_xml->size()),
						strand.wrap(
						boost::bind(&Artemis_Server_Connection::handle_write,
								shared_from_this(),
								boost::asio::placeholders::error,
								boost::asio::placeholders::bytes_transferred)));
				/*boost::thread t(
					boost::bind(&boost::asio::io_service::run, boost::ref(io_service)));
				t.join();*/
				/*boost::asio::async_write(m_socket,
						boost::asio::buffer(reply_xml[0].c_str(),
								reply_xml[0].size()),
						strand.wrap(
								boost::bind(
										&Artemis_Server_Connection::handle_write,
										shared_from_this(),
										boost::asio::placeholders::error,
										boost::asio::placeholders::bytes_transferred)));*/
			}
			//TODO
			// HANDLING FOR MULTIPLE RESULTS
			// I DONT THINK THIS CAN HAPPEN
			/*else if (request_handler.result_status == MULTIPLE_RESULTS)
			 {
			 #ifdef _SHOW_DEBUG_OUTPUT
			 std::cout << "	Query generated MULTIPLE results" << std::endl;
			 std::cout << "	 - Reply_XML: ";
			 for (unsigned int i = 0; i < reply_xml.size(); i++)
			 std::cout << reply_xml[i];
			 std::cout << std::endl;
			 #endif // _DEBUG
			 boost::asio::async_write(m_socket, boost::asio::buffer(reply_xml),
			 strand.wrap(
			 boost::bind(&Artemis_Server_Connection::handle_write,
			 shared_from_this(),
			 boost::asio::placeholders::error)));
			 }*/
		} 
		catch (std::exception& e)
		{
			std::cerr << "Exception during return message:" << std::endl
					<< e.what() << std::endl;
		}
		/*boost::logic::tribool result;
		 boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
		 request_, buffer_.data(), buffer_.data() + bytes_transferred);

		 if (result)
		 {
		 request_handler_.handle_request(request_, reply_);
		 boost::asio::async_write(socket_, reply_.to_buffers(),
		 strand_.wrap(
		 boost::bind(&connection::handle_write, shared_from_this(),
		 boost::asio::placeholders::error)));
		 }
		 else if (!result)
		 {
		 reply_ = reply::stock_reply(reply::bad_request);
		 boost::asio::async_write(socket_, reply_.to_buffers(),
		 strand_.wrap(
		 boost::bind(&connection::handle_write, shared_from_this(),
		 boost::asio::placeholders::error)));
		 }
		 else
		 {
		 socket_.async_read_some(boost::asio::buffer(buffer_),
		 strand_.wrap(
		 boost::bind(&connection::handle_read, shared_from_this(),
		 boost::asio::placeholders::error,
		 boost::asio::placeholders::bytes_transferred)));
		 }*/
	}
	else
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Error encountered: " << error.message() << std::endl;
#endif // _DEBUG
	}
	// If an error occurs then no new asynchronous operations are started. This
	// means that all shared_ptr references to the connection object will
	// disappear and the object will be destroyed automatically after this
	// handler returns. The connection class's destructor closes the socket.
}

//************************************
// Method:    handle_write
// FullName:  Artemis::Artemis_Server_Connection::handle_write
// Access:    private
// Returns:   void
// Qualifier:
// Parameter: const boost::system::error_code & error
// Parameter: std::vector<std::vector<char> > & replies
// Parameter: std::size_t position
//************************************
void Artemis_Server_Connection::handle_write(
		const boost::system::error_code& error,
		std::vector<std::vector<char> > &replies, std::size_t position)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " == Artemis_Server_Connection::Handle Write" 
		<< std::endl << " ==============="
			<< std::endl << "  -- reply_count = " << position << std::endl;
#endif
	replies.erase(replies.begin() + position);
	if (!error)
	{
		// Initiate graceful connection closure.
		boost::system::error_code ignored_ec;
		Artemis_Server_Connection::m_socket->shutdown(
				boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
		return_xml.reset(new std::string());
		m_socket.reset();
	}

	// No new asynchronous operations are started. This means that all shared_ptr
	// references to the connection object will disappear and the object will be
	// destroyed automatically after this handler returns. The connection class's
	// destructor closes the socket.
}

void Artemis_Server_Connection::handle_write(
		const boost::system::error_code& error, std::size_t bytes_transferred)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " == Artemis_Server_Connection::Handle Write" 
		<< std::endl << " ==============="
			<< std::endl;
	std::cout << " - Bytes_Transferred: " << bytes_transferred << std::endl;
#endif
	if (!error)
	{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Closing connection " << bytes_transferred << std::endl;
#endif
		// Initiate graceful connection closure.
		boost::system::error_code ignored_ec;
		Artemis_Server_Connection::m_socket->shutdown(
				boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
		m_socket->close();
	}
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Connection Closed " << bytes_transferred << std::endl;
#endif
	return_xml.reset(new std::string());
	// No new asynchronous operations are started. This means that all shared_ptr
	// references to the connection object will disappear and the object will be
	// destroyed automatically after this handler returns. The connection class's
	// destructor closes the socket.
}

std::string Artemis_Server_Connection::get_binary_file(std::string filename)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Upload_Data_Container::get_binary_file()" << std::endl;
#endif // _DEBUG
	std::string file_encoded;
	std::string test;
	std::ifstream file(filename.c_str(), std::ios::binary);
	if (file.is_open())
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - File is open" << std::endl;
		file.seekg(0, std::ios::end);
		std::cout << " - Tellg(): " << file.tellg() << std::endl;
		file.seekg(0, std::ios::beg);
#endif // _DEBUG
		std::stringstream *file_in = new std::stringstream();
		*file_in << file.rdbuf();
		std::cout << "file_in size: " << file_in->str().size() << std::endl;
		*file_in << std::ends;
		base64::encoder E;
		std::stringstream file_out;
		E.encode(*file_in, file_out);
		delete file_in;
		file_out << std::ends;
		std::cout << "file_out size: " << file_out.str().size() << std::endl;
		file_encoded = file_out.str();
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Encoded filesize: " << file_encoded.size()
				<< std::endl;
#endif // _DEBUG
		file.close();
	}
	return file_encoded;
}
}
