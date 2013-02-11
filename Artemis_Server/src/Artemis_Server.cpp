#include "Artemis_Server.h"
namespace Artemis
{
	class Alerts
	{
	private:
		static bool warning_sent(std::string computer_name, boost::gregorian::date current_date)
		{
			std::string filename = computer_name;
			filename += "_WARNING_";
			filename += boost::gregorian::to_simple_string(current_date);
			std::ifstream file(filename.c_str());
			if (file.good())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		static bool critical_sent(std::string computer_name, boost::gregorian::date current_date)
		{
			std::string filename = computer_name;
			filename += "_CRITICAL_";
			filename += boost::gregorian::to_simple_string(current_date);
			std::ifstream file(filename.c_str());
			if (file.good())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	public:
		static void check_alerts(Display_Client_Container_Ptr p_display_client_container, std::map<std::string, std::string> parameters)
		{
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << "=Alerts::check_alerts" << std::endl;
#endif // _SHOW_DEUG_OUTPUT
			while (true)
			{//This loop should run forever
				for (std::vector<Display_Client_Ptr>::iterator it = p_display_client_container->get_display_client_container()->begin();
					it != p_display_client_container->get_display_client_container()->end(); ++it)
				{//We iterate over all the Display_Clients in the container

					/* First we get the current time and current date */
					std::string current_time_str = boost::posix_time::to_iso_string(boost::posix_time::second_clock::universal_time());
					current_time_str = current_time_str.substr(current_time_str.find("T") + 1);
					boost::posix_time::time_duration current_time(
						boost::lexical_cast<int,std::string>(current_time_str.substr(0, 2)),
						boost::lexical_cast<int,std::string>(current_time_str.substr(2, 2)),
						boost::lexical_cast<int,std::string>(current_time_str.substr(4)),
						0);
					current_time = current_time + boost::posix_time::minutes(5);
					std::string current_date_str = boost::posix_time::to_iso_string(boost::posix_time::second_clock::universal_time());
					current_date_str = current_date_str.substr(0, current_date_str.find("T"));
					boost::gregorian::date current_date(boost::gregorian::from_undelimited_string(current_date_str));

					/* Now the checks start */
#ifdef _SHOW_DEBUG_OUTPUT
					std::cout << " # " << it->get()->get_identification() << " last ping: " 
						<< (it->get()->get_last_ping() - boost::posix_time::second_clock::universal_time()).invert_sign() << std::endl;
#endif // _SHOW_DEUG_OUTPUT
					if (((it->get()->get_last_ping() - boost::posix_time::second_clock::universal_time()).invert_sign() <
						boost::posix_time::time_duration(0,boost::lexical_cast<int, std::string>(parameters["alerts.critical_minutes"]),0,0)))
					{
						if (((it->get()->get_last_ping() - boost::posix_time::second_clock::universal_time()).invert_sign() >
							boost::posix_time::time_duration(0,boost::lexical_cast<int, std::string>(parameters["alerts.warning_minutes"]),0,0)))
						{//We need to send a WARNING alert
							if (!warning_sent(it->get()->get_identification(), current_date))
							{
#ifdef _SHOW_DEBUG_OUTPUT
								std::cout << "  # Ping has been longer than " << parameters["alerts.warning_minutes"] << " minutes, sending WARNING" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
								std::string filename = it->get()->get_identification();
								filename += "_WARNING_";
								filename += boost::gregorian::to_simple_string(current_date);
								std::ofstream mail_file;
								mail_file.open(filename.c_str());
								mail_file << "To: ";
								mail_file << parameters["Tacktech.admin"] + "\n";
								mail_file << "Subject: Warning for " + it->get()->get_identification() + "\n\n";
								mail_file << "The display '" + it->get()->get_identification() + "' has not pinged in " + parameters["alerts.warning_minutes"] + " minutes. ";
								mail_file.close();
								std::string sendmail_command = "sendmail -t < " + filename;
#ifdef _SHOW_DEBUG_OUTPUT
								std::cout << "  !! Sending: " << sendmail_command << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
								system(sendmail_command.c_str());
							}
						}
					}
					else
					{//We need to send a CRITICAL alert
						if (!critical_sent(it->get()->get_identification(), current_date))
						{
#ifdef _SHOW_DEBUG_OUTPUT
							std::cout << "  # Ping has been longer than " << parameters["alerts.critical_minutes"] << " minutes, sending CRITICAL" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
							std::string filename = it->get()->get_identification();
							filename += "_CRITICAL_";
							filename += boost::gregorian::to_simple_string(current_date);
							std::ofstream mail_file;
							mail_file.open(filename.c_str());
							mail_file << "To: ";
							mail_file << parameters["Tacktech.admin"] + "\n";
							mail_file << "Subject: Warning for " + it->get()->get_identification() + "\n\n";
							mail_file << "The display '" + it->get()->get_identification() + "' has not pinged in " + parameters["alerts.warning_minutes"] + " minutes. ";
							mail_file.close();
							std::string sendmail_command = "sendmail -t < " + filename;
#ifdef _SHOW_DEBUG_OUTPUT
							std::cout << "  !! Sending: " << sendmail_command << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
							system(sendmail_command.c_str());
						}
					}			
				}
				boost::this_thread::sleep_for(boost::chrono::milliseconds(boost::lexical_cast<int, std::string>(parameters["alerts.check_frequency"])));
			}
		}
	};
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
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << "Config file read: " << std::endl << "==================="
				<< std::endl;
#endif
		for (boost::program_options::detail::config_file_iterator i(config,
				options), e; i != e; ++i)
		{
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << i->string_key << " " << i->value[0] << std::endl;
#endif
			parameters[i->string_key] = i->value[0];
		}
	} catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
}

/** Function reads the stored values of group_playlist, playlist, and 
 ** computers_and_groups into the class variables from stored XML files */
void Artemis_Server::initialize_variables()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Artemis_Server::initialize_variables()" << std::endl;
#endif // _DEBUG
	playlist_container.reset(new Playlist_Container());
	display_client_container.reset(new Display_Client_Container());
	filelist.reset(new Filelist(parameters));
}

void Artemis_Server::store_variables()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Artemis_Server::store_variables()" << std::endl;
#endif // _DEBUG
	/* Save playlist to file */
	pugi::xml_document playlist_document;
	//playlist_document.load(playlist->get_playlists_xml().c_str());
	playlist_document.save_file("./playlist.xml");
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Playlist stored successfully" << std::endl;
#endif // _DEBUG

}

//************************************
// Method:    Artemis_Server
// FullName:  Artemis::Artemis_Server::Artemis_Server
// Access:    public
// Returns:
// Qualifier: : thread_pool_size(p_thread_pool_size), signals(io_service), acceptor(io_service), new_connection(), request_handler()
// Parameter: std::size_t p_thread_pool_size
//************************************
Artemis_Server::Artemis_Server(std::size_t p_thread_pool_size) :
		thread_pool_size(p_thread_pool_size), signals(io_service), acceptor(
				io_service), new_connection()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Artemis_Server::Artemis_Server()" << std::endl
			<< "===================================================="
			<< std::endl;
#endif // _DEBUG
	Artemis_Server::read_config();
	initialize_variables();
	signals.add(SIGINT);
	signals.add(SIGTERM);
#if defined(SIGQUIT)
	signals.add(SIGQUIT);
#endif // defined(SIGQUIT)
	signals.async_wait(boost::bind(&Artemis_Server::handle_stop, this));

	boost::asio::ip::tcp::resolver resolver(io_service);
	boost::asio::ip::tcp::resolver::query query(parameters["general.server_ip"],
			parameters["general.listen_port"]);
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "Artemis Server" << std::endl << "==============" << std::endl
			<< " -- Server IP:   " << parameters["general.server_ip"]
			<< std::endl << " -- Server PORT: "
			<< parameters["general.listen_port"] << std::endl
			<< " -- Creating endpoint" << std::endl;
#endif // _DEBUG
	boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " -- Opening acceptor" << std::endl;
#endif // _DEBUG
	acceptor.open(endpoint.protocol());
	//acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " -- Binding to endpoint" << std::endl;
#endif // _DEBUG
	acceptor.bind(endpoint);
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " -- Starting to listen" << std::endl;
#endif // _DEBUG
	acceptor.listen();

#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " -- Calling start_accept()" << std::endl;
#endif // _DEBUG
	boost::thread check_alerts(Alerts::check_alerts, display_client_container, parameters);
	start_accept();
}

Artemis_Server::~Artemis_Server()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Artemis_Server::~Artemis_Server()" << std::endl;
#endif //_DEBUG
	//store_variables();
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
		boost::shared_ptr<boost::thread> thread(
				new boost::thread(
						boost::bind(&boost::asio::io_service::run,
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
#ifdef _SHOW_DEBUG_OUPUT
	std::cout << "=start_accept()" << std::endl;
	std::cout << " - display_client_container.get() ---> " << display_client_container.get() << std::endl;
#endif // _SHOW_DEBUG_OUPUT
	new_connection.reset(
			new Artemis_Server_Connection(io_service, parameters,
					playlist_container, display_client_container, filelist));
	acceptor.async_accept(new_connection->socket(),
			boost::bind(&Artemis_Server::handle_accept, this,
					boost::asio::placeholders::error));
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
	store_variables();
}

}
