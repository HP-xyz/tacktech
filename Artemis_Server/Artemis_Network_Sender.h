#ifndef ARTEMIS_NETWORK_SENDER_H
#define ARTEMIS_NETWORK_SENDER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/program_options.hpp>
#include <boost/config.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <sstream>
#include <exception>
#include <fstream>

class Artemis_Network_Sender
{
public:
	/** Starts a connection */
	Artemis_Network_Sender(
		boost::asio::io_service&,
		boost::asio::ip::tcp::resolver::iterator);

	~Artemis_Network_Sender(void);

	/** Begins the write operation of the xml_string provided */
	void write(std::string xml_string);
	/** Closes the socket */
	void close();
private:
	boost::asio::io_service& m_io_service;
	boost::asio::ip::tcp::socket m_socket;

	void handle_connect(const boost::system::error_code&);
	void do_write(std::string xml_string);
	void handle_write(const boost::system::error_code& error);
	void do_close();
};
#endif // ARTEMIS_NETWORK_SENDER_H