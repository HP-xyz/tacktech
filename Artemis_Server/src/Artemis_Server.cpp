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

/** Function reads the stored values of group_playlist, playlist, and 
 ** computers_and_groups into the class variables from stored XML files */
void Artemis_Server::initialize_variables()
{
#ifdef _DEBUG
    std::cout << "= Artemis_Server::initialize_variables()"
              << std::endl;
#endif // _DEBUG
	groups_and_computers.reset(new Group_Container());
	playlist.reset(new Playlist_Container());
	group_playlist.reset(new Group_Playlist_Container());
    /* Read playlist from file */
    pugi::xml_document playlist_document;
    playlist_document.load_file("./playlist.xml");
    pugi::xml_node root_node = playlist_document.child("Playlist");
#ifdef _DEBUG
    std::cout << " - Root Item:" << std::endl;
    root_node.print(std::cout);
#endif // _DEBUG
    for (pugi::xml_node playlist_item = root_node.child("Playlist_Item");
            playlist_item;
            playlist_item = playlist_item.next_sibling("Playlist_Item"))
    {
#ifdef _DEBUG
        std::cout << " - Playlist Item:" << std::endl;
        playlist_item.print(std::cout);
#endif // _DEBUG
        for(pugi::xml_node item_node = playlist_item.child("Item");
                item_node;
                item_node = item_node.next_sibling("Item"))
        {
            playlist->add_filename(
                playlist_item.attribute("Playlist_Name").as_string(),
                item_node.child_value("Filename"),
                boost::lexical_cast<int>(item_node.child_value("Pause")));
        }
    }

    /* Read groups_and_computers from file */
    pugi::xml_document groups_and_computers_document;
    groups_and_computers_document.load_file("./groups_and_computers.xml");
    root_node = groups_and_computers_document.child("Groups_And_Computers");
#ifdef _DEBUG
    std::cout << " - Root Item:" << std::endl;
    root_node.print(std::cout);
#endif // _DEBUG
    for (pugi::xml_node group_item = root_node.child("Group_Item");
            group_item;
            group_item = group_item.next_sibling("Group_Item"))
    {
#ifdef _DEBUG
// TODO (HP#1#):
        std::cout << " - Group Item:" << std::endl;
        group_item.print(std::cout);
#endif // _DEBUG
        for (pugi::xml_node computer_item = group_item.child("Computer_Item");
                computer_item;
                computer_item = computer_item.next_sibling("Computer_Item"))
        {
            groups_and_computers->add_computer_name(
                group_item.attribute("Group_Name").as_string(),
                computer_item.child_value("Computer"));
        }
    }

    /* Read group_playlist from file */
    pugi::xml_document group_playlist_document;
    group_playlist_document.load_file("./group_playlist.xml");
    root_node = group_playlist_document.child("Group_Playlist");
#ifdef _DEBUG
    std::cout << " - Root Item:" << std::endl;
    root_node.print(std::cout);
#endif // _DEBUG
    for (pugi::xml_node group_playlist_item =
                root_node.child("Group_Playlist_Item");
            group_playlist_item;
            group_playlist_item =
                group_playlist_item.next_sibling("Group_Playlist_Item"))
    {
#ifdef _DEBUG
        std::cout << " - Group_Playlist Item:" << std::endl;
        group_playlist_item.print(std::cout);
#endif // _DEBUG
        group_playlist->connect_group_to_playlist(
            group_playlist_item.child_value("Group_Name"),
            group_playlist_item.child_value("Playlist_Name"));
    }
}

void Artemis_Server::store_variables()
{
#ifdef _DEBUG
	std::cout << "= Artemis_Server::store_variables()"
		<< std::endl;
#endif // _DEBUG
	/* Save playlist to file */
	/*pugi::xml_document playlist_document;
	pugi::xml_node root_node_playlist =
		playlist_document.append_child("Playlist");
	for (int i = 0; i < playlist->get_playlist().uniqueKeys().size(); i++)
	{
		pugi::xml_node playlist_node =
			root_node_playlist.append_child("Playlist_Item");
		playlist_node.append_attribute("Playlist_Name") =
			playlist->get_playlist().uniqueKeys().at(i).toStdString()
			.c_str();
		for (int j = 0; j < playlist->get_playlist().values(
			playlist->get_playlist().uniqueKeys().at(i)).size(); j++)
		{
			pugi::xml_node item_node = playlist_node.append_child("Item");
			pugi::xml_node filename_node = item_node.append_child(
				"Filename");
			pugi::xml_node pause_node = item_node.append_child(
				"Pause");
			pugi::xml_node filename_pcdata =
				filename_node.append_child(pugi::node_pcdata);
			pugi::xml_node pause_pcdata =
				pause_node.append_child(pugi::node_pcdata);

			filename_pcdata.set_value(playlist->get_playlist().values(
				playlist->get_playlist().uniqueKeys().at(i))
				.at(j).first.toStdString().c_str());
			pause_pcdata.set_value(boost::lexical_cast<std::string>(
				playlist->get_playlist().values(
				playlist->get_playlist().uniqueKeys().at(i))
				.at(j).second).c_str());
		}
	}
	playlist_document.save_file("./playlist.xml");*/

	/* Save groups_and_computers to file */
	/*pugi::xml_document groups_and_computers_document;
	pugi::xml_node root_node_groups_and_computers =
		groups_and_computers_document.append_child("Groups_And_Computers");

	for (int i = 0;
		i < groups_and_computers->
		get_groups_and_computers().uniqueKeys().size(); i++)
	{
		pugi::xml_node group_node = root_node_groups_and_computers
			.append_child("Group_Item");
		group_node.append_attribute("Group_Name") =
			groups_and_computers->get_groups_and_computers().uniqueKeys()
			.at(i).toStdString().c_str();
		for (int j = 0;
			j < groups_and_computers->
			get_groups_and_computers().values(
			groups_and_computers->get_groups_and_computers()
			.uniqueKeys().at(i)).size(); j++)
		{
			pugi::xml_node computer_item_node = group_node.append_child("Computer_Item");
			pugi::xml_node computer_node = computer_item_node.append_child(
				"Computer");
			pugi::xml_node computer_pcdata =
				computer_node.append_child(pugi::node_pcdata);
			computer_pcdata.set_value(
				groups_and_computers->get_groups_and_computers().values(
				groups_and_computers->get_groups_and_computers()
				.uniqueKeys().at(i)).at(j).toStdString().c_str());
		}
	}
	groups_and_computers_document.save_file("./groups_and_computers.xml");

	/* Save group_playlist to file */
	/*pugi::xml_document group_playlist_document;
	pugi::xml_node root_node_group_playlist =
		group_playlist_document.append_child("Group_Playlist");

	for (int i = 0; i < group_playlist->get_group_playlist()->size(); i++)
	{
		pugi::xml_node group_playlist_node =
			root_node_group_playlist.append_child("Group_Playlist_Item");
		pugi::xml_node group_name_node =
			group_playlist_node.append_child("Group_Name");
		pugi::xml_node playlist_name_node =
			group_playlist_node.append_child("Playlist_Name");
		pugi::xml_node group_name_pcdata =
			group_name_node.append_child(pugi::node_pcdata);
		pugi::xml_node playlist_name_pcdata =
			playlist_name_node.append_child(pugi::node_pcdata);
		group_name_pcdata.set_value(
			group_playlist->get_group_playlist()->
			at(i).first.toStdString().c_str());
		playlist_name_pcdata.set_value(
			group_playlist->get_group_playlist()->
			at(i).second.toStdString().c_str());
	}
	group_playlist_document.save_file("./group_playlist.xml");*/
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
    new_connection()
{
#ifdef _DEBUG
    std::cout << "=Artemis_Server::Artemis_Server()" << std::endl
              << "====================================================" << std::endl;
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

Artemis_Server::~Artemis_Server()
{
#ifdef _DEBUG
	std::cout << "=Artemis_Server::~Artemis_Server()" << std::endl;
#endif //_DEBUG
	store_variables();
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
                         parameters, groups_and_computers, playlist,
						 group_playlist));
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
