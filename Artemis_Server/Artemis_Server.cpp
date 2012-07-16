#include "Artemis_Server.h"
namespace Artemis
{

//************************************
// Method:    read_config
// FullName:  Artemis::Artemis_Server::read_config
// Access:    private
// Returns:   void
// Qualifier:
//************************************
void Artemis_Server::read_config()
{
    std::ifstream config("server.ini");
    if (!config)
    {
        std::cerr << " == Error -> No config found" << std::endl;
    }

    Artemis_Server::options.insert("*");

    try
    {
#ifdef _DEBUG
        std::cout << "Config file read: " << std::endl <<
                  "===================" << std::endl;
#endif
        for (boost::program_options::detail::config_file_iterator
                i(config, options), e; i != e; ++i)
        {
#ifdef _DEBUG
            std::cout << i->string_key << " " << i->value[0] << std::endl;
#endif
            parameters[i->string_key] = i->value[0];
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

//************************************
// Method:    Artemis_Server
// FullName:  Artemis::Artemis_Server::Artemis_Server
// Access:    public
// Returns:
// Qualifier: : thread_pool_size(p_thread_pool_size), signals(io_service), acceptor(io_service), new_connection(), request_handler()
// Parameter: std::size_t p_thread_pool_size
//************************************
Artemis_Server::Artemis_Server(std::size_t p_thread_pool_size)
    : thread_pool_size(p_thread_pool_size),
    signals(io_service),
    acceptor(io_service),
    new_connection(),
    request_handler()
{
#ifdef _DEBUG
    std::cout << "=Artemis_Server::Artemis_Server()" << std::endl
              << "====================================================" << std::endl;
#endif // _DEBUG
    Artemis_Server::read_config();
    signals.add(SIGINT);
    signals.add(SIGTERM);
#if defined(SIGQUIT)
    signals.add(SIGQUIT);
#endif // defined(SIGQUIT)
    signals.async_wait(boost::bind(&Artemis_Server::handle_stop, this));

    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(parameters["general.server_ip"],
            parameters["general.listen_port"]);
#ifdef _DEBUG
    std::cout << "Artemis Server" << std::endl
              << "==============" << std::endl
              << " -- Server IP:   " << parameters["general.server_ip"] << std::endl
              << " -- Server PORT: " << parameters["general.listen_port"] << std::endl
              << " -- Creating endpoint" << std::endl;
#endif // _DEBUG
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
#ifdef _DEBUG
    std::cout << " -- Opening acceptor" << std::endl;
#endif // _DEBUG
    acceptor.open(endpoint.protocol());
    //acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
#ifdef _DEBUG
    std::cout << " -- Binding to endpoint" << std::endl;
#endif // _DEBUG
    acceptor.bind(endpoint);
#ifdef _DEBUG
    std::cout << " -- Starting to listen" << std::endl;
#endif // _DEBUG
    acceptor.listen();

#ifdef _DEBUG
    std::cout << " -- Calling start_accept()" << std::endl;
#endif // _DEBUG
    start_accept();
}

//************************************
// Method:    run
// FullName:  Artemis::Artemis_Server::run
// Access:    public
// Returns:   void
// Qualifier:
//************************************
void Artemis_Server::run()
{
    std::cout << " =Artemis_Server::run()" << std::endl
              << " ======================" << std::endl;
    //Create a pool of threads to run all of the io_services
    std::vector<boost::shared_ptr<boost::thread> > threads;
    for (std::size_t i = 0; i < Artemis_Server::thread_pool_size; ++i)
    {
        boost::shared_ptr<boost::thread>
        thread(new boost::thread(boost::bind(&boost::asio::io_service::run,
                                             &io_service)));
        threads.push_back(thread);
    }

    // Wait for all threads in pool to exit
    for (std::size_t i = 0; i < threads.size(); ++i)
        threads[i]->join();
}

//************************************
// Method:    start_accept
// FullName:  Artemis::Artemis_Server::start_accept
// Access:    private
// Returns:   void
// Qualifier:
//************************************
void Artemis_Server::start_accept()
{
    new_connection.reset(new Artemis_Server_Connection(io_service,
                         parameters));
    acceptor.async_accept(new_connection->socket(),
                          boost::bind(&Artemis_Server::handle_accept,
                                      this, boost::asio::placeholders::error));
}

//************************************
// Method:    handle_accept
// FullName:  Artemis::Artemis_Server::handle_accept
// Access:    private
// Returns:   void
// Qualifier:
// Parameter: const boost::system::error_code & error_code
//************************************
void Artemis_Server::handle_accept(const boost::system::error_code& error_code)
{
    if (!error_code)
    {
        new_connection->start();
    }

    start_accept();
}

//************************************
// Method:    handle_stop
// FullName:  Artemis::Artemis_Server::handle_stop
// Access:    private
// Returns:   void
// Qualifier:
//************************************
void Artemis_Server::handle_stop()
{
    io_service.stop();
}
}
