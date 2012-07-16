/*
 * File:   Artemis_Server_Connection.h
 * Author: dvorak
 *
 * Created on September 11, 2011, 12:30 PM
 */

#ifndef ARTEMIS_SERVER_CONNECTION_H
#define	ARTEMIS_SERVER_CONNECTION_H
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/foreach.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/logic/tribool.hpp>
#include "boost/tuple/tuple.hpp"
#include <boost/lexical_cast.hpp>
#include "Artemis_Request_Handler.h"
#include <pugixml.hpp>
#include <exception>
#include <vector>
#include <list>

namespace Artemis
{

class Artemis_Server_Connection
    : public boost::enable_shared_from_this<Artemis_Server_Connection>,
private boost::noncopyable

{
public:

    /** Constructs a connection withe the given io_service */
    explicit Artemis_Server_Connection(boost::asio::io_service& io_service,
                                       std::map<std::string, std::string>&);

    /** Get the socket associated with the connection*/
    boost::asio::ip::tcp::socket& socket();
    /** Start the first asynchronous operation for the connection */
    void start();
private:
    /** Handle completion of a read operation */
    void handle_read(const boost::system::error_code& error,
                     std::size_t bytes_transferred);
    /** Handle completion of a write operation */
    void handle_write(const boost::system::error_code& error,
                      std::vector<std::vector<char> >&, std::size_t);
    /** Strand to ensure the connections handlers are not called concurrently */
    boost::asio::io_service::strand strand;
    /** Socket for the connection */
    boost::asio::ip::tcp::socket m_socket;
    /** Buffer for incoming data */
    boost::array<char, 8192> buffer;
    /** Generated reply, XML */
    std::vector<std::vector<char> > reply_xml;

    std::string received_xml;

    std::map<std::string, std::string> parms;

    long long received_size;
};
typedef boost::shared_ptr<Artemis_Server_Connection>
Artemis_Server_Connection_ptr;
}
#endif	/* ARTEMIS_SERVER_CONNECTION_H */

