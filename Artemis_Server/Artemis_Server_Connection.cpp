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
		std::map<std::string, std::string>& parameters)
	:	strand(io_service),
		m_socket(io_service)
{
	Artemis_Server_Connection::parms = parameters;
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
    return Artemis_Server_Connection::m_socket;
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
	m_socket.async_read_some(boost::asio::buffer(Artemis_Server_Connection::buffer),
		boost::bind(&Artemis_Server_Connection::handle_read, shared_from_this(),
		boost::asio::placeholders::error,
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
    const boost::system::error_code& error, std::size_t bytes_transferred)
{
#ifdef _DEBUG
	std::cout << " == Handle Read" << std::endl
        << " ==============" << std::endl;
#endif
    if (!error)
    {
#ifdef _DEBUG
		std::cout << "Buffer_Data -> " << Artemis_Server_Connection::buffer.data() << std::endl;
        std::cout << "Bytes_Transferred -> " << bytes_transferred << std::endl;
#endif

        try
        {
			Artemis_Request_Handler request_handler;
			std::string message;
			std::size_t pos = reply_xml.size();
			for (unsigned int i = 0; i < bytes_transferred; i++)
				message += buffer[i];
			request_handler.handle_request(message, reply_xml,
				Artemis_Server_Connection::parms);
            //Artemis_SQL_Handler sql_handle(Artemis_Server_Connection::parms);
            //sql_handle.run_query("SELECT * FROM `User`");
            // 
			if (request_handler.result_status == NO_RESULT)
			{
#ifdef _DEBUG
				std::cout << "	Query generated no results" << std::endl;
#endif // _DEBUG
			}
			else if (request_handler.result_status == SINGLE_RESULT)
			{
#ifdef _DEBUG
				std::cout << "	Query generated ONE result" << std::endl;
				std::cout << "	 - Reply_XML: ";
				for (unsigned int i = 0; i < reply_xml[pos].size(); i++)
				{
					std::cout << reply_xml[pos][i];
				}
				std::cout << std::endl;
#endif // _DEBUG
				boost::asio::async_write(m_socket, boost::asio::buffer(reply_xml[pos]),
					strand.wrap(
					boost::bind(&Artemis_Server_Connection::handle_write,
					shared_from_this(),
					boost::asio::placeholders::error,
					reply_xml,
					pos)));
			}
			//TODO
			// HANDLING FOR MULTIPLE RESULTS
			// I DONT THINK THIS CAN HAPPEN
			/*else if (request_handler.result_status == MULTIPLE_RESULTS)
			{
#ifdef _DEBUG
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
        catch(std::exception& e)
        {
            std::cerr << "Exception during query execution:" << std::endl <<
                    e.what() << std::endl;
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
void Artemis_Server_Connection::handle_write(const boost::system::error_code& error,
	std::vector<std::vector<char> > &replies,
	std::size_t position)
{
#ifdef _DEBUG
	std::cout << " == Handle Write" << std::endl
              << " ===============" << std::endl
			  << "  -- reply_count = " << position << std::endl;
#endif
	replies.erase(replies.begin() + position);
    if (!error)
    {
        // Initiate graceful connection closure.
        boost::system::error_code ignored_ec;
        Artemis_Server_Connection::m_socket.shutdown(
                                                   boost::asio::ip::tcp::
                                                   socket::shutdown_both,
                                                   ignored_ec);
    }

    // No new asynchronous operations are started. This means that all shared_ptr
    // references to the connection object will disappear and the object will be
    // destroyed automatically after this handler returns. The connection class's
    // destructor closes the socket.
}
}