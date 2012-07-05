#ifndef ARTEMIS_SERVER_H
#define ARTEMIS_SERVER_H

#include "Artemis_Server_Connection.h"
#include "Artemis_Request_Handler.h"
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
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

namespace Artemis
{

class Artemis_Server : private boost::noncopyable
{
public:

    /** Initialize server to lister on specified TCP address and port*/
    explicit Artemis_Server(std::size_t p_thread_pool_size);

    /** Runs the server's io_service loop*/
    void run();

private:
    /** Initialize an asynchronous accept operation */
    void start_accept();
    /** Handles completion of an asynchronous accept operation */
    void handle_accept(const boost::system::error_code& error_code);
    /** Handles a request to stop the server */
    void handle_stop();
    /** The number of threads that will call io_service::run()*/
    std::size_t thread_pool_size;
    /** The io_serivce used to perform asynchronous operations */
    boost::asio::io_service io_service;
    /** The signal_set is used to register process termination notifications */
    boost::asio::signal_set signals;
    /** Acceptor used to lister for incoming connections */
    boost::asio::ip::tcp::acceptor acceptor;
    /** The next connection to be accepted */
    //boost::shared_ptr<Artemis::Artemis_Server_Connection> new_connection;
    Artemis_Server_Connection_ptr new_connection;
    /** The handler for all incoming requests*/
    Artemis_Request_Handler request_handler;

    std::map<std::string, std::string> parameters;
    std::set<std::string> options;

    /** Reads a config file*/
    void read_config();

};
}
#endif //ARTEMIS_SERVER_H
