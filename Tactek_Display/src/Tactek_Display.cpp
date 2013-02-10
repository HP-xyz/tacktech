/****************************************************************************
 * VLC-Qt CMake Demo
 * Copyright (C) 2012 Tadej Novak <tadej@tano.si>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/
#include "Tactek_Display.h"

/** Struct to define a xml_writer to string.
 ** Copied directly from the pugixml quickstart */
struct xml_string_writer: pugi::xml_writer
{
	std::string result;
	virtual void write(const void* data, size_t size)
	{
		result += std::string(static_cast<const char*>(data), size);
	}
};

template <class T>
std::string make_list( T p_vector)
{
	std::string comma_separated_list;
	for (typename T::iterator it = p_vector.begin();
		it != p_vector.end(); ++it)
	{
		comma_separated_list += *it;
		comma_separated_list += ", ";
	}
	comma_separated_list =
		comma_separated_list.substr(0, comma_separated_list.length() - 2);
	return comma_separated_list;
}

class Write_File
{
public:
    static void write_file(std::string filename, std::string file_data)
    {
        std::stringstream encoded_stream;
        std::stringstream decoded_stream;
        encoded_stream << file_data;
        base64::decoder D;
        D.decode(encoded_stream, decoded_stream);
        file_data = decoded_stream.str();
        std::ofstream out_file(filename.c_str(), std::ios::binary);
        out_file << file_data;
        out_file.close();
    }
};
/**
 * Initiates and sets up the class for usage. Connects the check_media_state and
 * play_next_media_in_queue signals to their respective slots.
 * @param parent
 */
Tactek_Display::Tactek_Display(QWidget *parent) :
		QMainWindow(parent), ui(new Ui::Tactek_Display), m_vlc_media(0)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tactek_Display()" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
    read_config();
	ui->setupUi(this);
	m_display_client.reset(new Display_Client());
	news_ticker_thread = new News_Ticker_Thread(parameters);

    QFont font("Arial", boost::lexical_cast<int, std::string>(parameters["news.font_size"]));
    ui->news_label->setFont(font);
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Chekcing file directory" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
    check_file_directory();

#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Assigning Computer_name and Organization_name" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	m_display_client->set_identification(parameters["general.computer_name"]);
	m_display_client->add_organization(parameters["general.organization_name"]);
	m_display_client->set_last_ping();

#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Setting up timers" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	Tactek_Display::update_timer = new QTimer(this);
	Tactek_Display::check_update_timer = new QTimer(this);
	Tactek_Display::identify_timer = new QTimer(this);
	Tactek_Display::update_display_client_timer = new QTimer(this);

#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Setting up Network_Managers" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
    io_service_file_transfer.reset(new boost::asio::io_service);
	io_service_identification.reset(new boost::asio::io_service);
	network_manager_file_transfer.reset(
		new Tacktech_Network_Manager(*io_service_file_transfer, parameters));
    network_manager_identification.reset(
		new Tacktech_Network_Manager(*io_service_identification, parameters));

#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Setting up VLC" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
    try
    {
        m_vlc_instance = new VlcInstance(VlcCommon::args(), this);
        m_vlc_player = new VlcMediaPlayer(m_vlc_instance);
        m_vlc_player->setVideoWidget(ui->vlc_video_widget);
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

	ui->vlc_video_widget->setMediaPlayer(m_vlc_player);

#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Connecting Signals" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
    connect(update_display_client_timer, SIGNAL(timeout()), this, SLOT(check_display_container()));
	connect(update_timer, SIGNAL(timeout()), this, SLOT(check_media_state()));
	connect(check_update_timer, SIGNAL(timeout()), this, SLOT(check_playlist_items_downloaded()));
	connect(identify_timer, SIGNAL(timeout()), this, SLOT(update_display_client()));
	connect(this, SIGNAL(start_next_media()), this,
			SLOT(play_next_media_in_queue()));
	connect(this, SIGNAL(new_file_added(QString, int)), this,
			SLOT(handle_new_file_added(QString, int)));
    connect(news_ticker_thread, SIGNAL(news_ready(QString)), this, SLOT(display_news(QString)));

#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Starting Timers" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	update_timer->start(1000);
	check_update_timer->start(80000);
	update_display_client_timer->start(40000);
	identify_timer->start(30000);
	update_display_client();
	news_ticker_thread->start();
}

Tactek_Display::~Tactek_Display()
{
	delete ui;
	delete m_vlc_media;
	delete m_vlc_player;
	delete m_vlc_instance;
	delete update_timer;
	delete check_update_timer;
	delete identify_timer;
	delete update_display_client_timer;
	delete news_ticker_thread;
}

void Tactek_Display::read_config()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tacktech_Manager::read_config()" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	std::ifstream config("config.ini");
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Created config ifstream" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	if (!config)
	{
		std::cerr << " == Error -> No config found" << std::endl;
		;
	}

#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Creating options.insert" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	options.insert("*");

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
		std::string error_msg;
		error_msg = "Config ERROR: ";
		error_msg += e.what();
	}
}

void Tactek_Display::check_playlist_items_downloaded()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tacktech_Manager::check_for_updates()" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
    /* First, we check what files we have... */
    check_file_directory();
    std::vector<std::string> files_needed;
    for(Container::iterator it = m_display_client->get_playlist_container()->get_playlist_container()->begin();
        it != m_display_client->get_playlist_container()->get_playlist_container()->end(); ++it)
    {
#ifdef _SHOW_DEBUG_OUTPUT
        std::cout << " - Checking: " << it->first->get_playlist_name() << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
        for(std::vector< std::pair<std::string,int> >::iterator it2 = it->first->get_playlist_items()->begin();
            it2 != it->first->get_playlist_items()->end(); ++it2)
        {
            std::string file_to_find = parameters["general.playlist_directory"];
            file_to_find += it2->first;
            std::vector<std::string>::iterator item_found = std::find(filelist.begin(), filelist.end(), file_to_find);
            if(item_found == filelist.end())
            {//We do not have the needed file
#ifdef _SHOW_DEBUG_OUTPUT
                std::cout << "  ++ " << it2->first << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
                files_needed.push_back(it2->first);
            }
        }
    }
#ifdef _SHOW_DEBUG_OUTPUT
        std::cout << " - Need to get '" << files_needed.size() << "' items" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
    if(files_needed.size() > 0)
    {
        pugi::xml_document document;
        pugi::xml_node tacktech_node = document.append_child("Tacktech");
        pugi::xml_node type_node = tacktech_node.append_child("Type");
        type_node.append_attribute("TYPE") = "GET_UPDATES";
        pugi::xml_node identification_node = tacktech_node.append_child("Identification_Node");
        identification_node.append_attribute("Identification") = m_display_client->get_identification().c_str();
        identification_node.append_attribute("Organization") = parameters["general.organization_name"].c_str();
        pugi::xml_node file_node = tacktech_node.append_child("File_Node");
        for(std::vector<std::string>::iterator it = files_needed.begin(); it != files_needed.end(); ++it)
            file_node.append_attribute("File") = it->c_str();
        xml_string_writer writer;
        document.save(writer);
        document.print(std::cout);

        if(!network_manager_file_transfer->busy)
        {
            boost::shared_ptr<std::string> string_to_send;
            string_to_send.reset(new std::string(writer.result));

            io_service_file_transfer->reset();
            network_manager_file_transfer.reset(
                    new Tacktech_Network_Manager(*io_service_file_transfer, parameters));
            network_manager_file_transfer->connect(parameters["general.server_ip"],
                    parameters["general.server_port"]);
            network_manager_file_transfer->busy = true;
            connect(network_manager_file_transfer.get(), SIGNAL(data_recieved(QString)), this,
                        SLOT(handle_recieved_data(QString)));
            network_manager_file_transfer->start_write(string_to_send);
            boost::thread t(
                boost::bind(&boost::asio::io_service::run, boost::ref(io_service_file_transfer)));
            t.join();
        }
    }
    else
    {
#ifdef _SHOW_DEBUG_OUTPUT
        std::cout << " - WE NEED NO NEW FILES!" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
    }
//	pugi::xml_document document;
//	pugi::xml_node tacktech_node = document.append_child("Tacktech");
//	pugi::xml_node type_node = tacktech_node.append_child("Type");
//	type_node.append_attribute("TYPE") = "GET_UPDATES";
//	pugi::xml_node files_node = tacktech_node.append_child("Files");
//	pugi::xml_node playlist_node = tacktech_node.append_child("Playlist");
//	playlist_name = "Playlist 1";
//	playlist_node.append_attribute("PLAYLIST") = playlist_name.toStdString().c_str();
//	for (int i = 0; i < playlist->get_playlist().size(); i++)
//	{
//		files_node.append_attribute("FILE") =
//				playlist->get_playlist().at(i).first.toStdString().c_str();
//	}
//	xml_string_writer writer;
//	document.save(writer);
//	document.print(std::cout);
//
//	boost::shared_ptr<std::string> string_to_send;
//	string_to_send.reset(new std::string(writer.result));
//
//	io_service->reset();
//	network_manager.reset(
//			new Tacktech_Network_Manager(*io_service, parameters));
//	network_manager->connect(parameters["general.server_ip"],
//			parameters["general.server_port"]);
//	connect(network_manager.get(), SIGNAL(data_recieved(QString)), this,
//				SLOT(handle_recieved_data(QString)));
//	network_manager->start_write(string_to_send);
//	boost::thread t(
//		boost::bind(&boost::asio::io_service::run, boost::ref(io_service)));
//	t.join();
}
void Tactek_Display::update_display_client()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tacktech_Manager::send_identity_to_server()" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
    std::string upload_string = "<Tacktech>";
    upload_string += "<Type TYPE=\"";
    upload_string += "IDENTIFY";
    upload_string += "\"/>";
    upload_string += "<Identity Organization_Name=\"" + parameters["general.organization_name"] + "\" Computer_Name=\"" + parameters["general.computer_name"] + "\"/>";
    upload_string += "<CONTAINER>";
    upload_string += m_display_client->get_playlist_container()->get_playlist_container_xml();
    upload_string += "</CONTAINER>";
    upload_string += "</Tacktech>";

#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << upload_string << std::endl;
#endif // _SHOW_DEBUG_OUTPUT

    if(!network_manager_identification->busy)
    {
        boost::shared_ptr<std::string> string_to_send;
        string_to_send.reset(new std::string(upload_string));

        io_service_identification->reset();
        network_manager_identification.reset(
                    new Tacktech_Network_Manager(*io_service_identification, parameters));
        network_manager_identification->connect(parameters["general.server_ip"],
                parameters["general.server_port"]);
        network_manager_identification->busy = true;
        connect(network_manager_identification.get(), SIGNAL(data_recieved(QString)), this,
                    SLOT(handle_recieved_data(QString)));
        network_manager_identification->start_write(string_to_send);
        boost::thread t(
            boost::bind(&boost::asio::io_service::run, boost::ref(io_service_identification)));
        t.join();
    }
}

/**
 * Opens and plays the file specified in the filepath parameter.send_identity_to_server
 * @param filepath Path to the file that needs to be played
 */
void Tactek_Display::open_and_play(QString filepath)
{
    try
    {
        m_vlc_media = new VlcMedia(filepath, true, m_vlc_instance);
        m_vlc_player->open(m_vlc_media);
    }
    catch(std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
}

/**
 * Checks the currents media state and fires a playback_ended signal if the
 * current m_vlc_player state = 0 (IDLE)
 */
void Tactek_Display::check_media_state()
{
	if (Tactek_Display::m_vlc_player->state() == 0
			|| Tactek_Display::m_vlc_player->state() == 6)
		emit Tactek_Display::start_next_media();
}

/**
 * Slot to play the next media in the playlist. Gets called by the
 * start_next_media() signal. Will pause the output for the specified number
 * of seconds in the playlist, if the pause is not equal to 0. Does nothing
 * when there is no media in the playlist.
 */
void Tactek_Display::play_next_media_in_queue()
{
    std::pair<std::string, int> next_item = m_display_client->get_playlist_container()->get_next_item();
    std::string filename = parameters["general.playlist_directory"];
    filename += next_item.first;
    std::ifstream file(filename.c_str());
	if (next_item.first != "NO ITEMS" && file.good())
	{
		if (next_item.second == 0)
		{
			open_and_play(QString::fromStdString(filename));
		}
		else
		{
			std::cout << "Pausing for: " << next_item.second * 1000
					<< std::endl;
			QTimer::singleShot((next_item.second * 1000), this, NULL);
			open_and_play(QString::fromStdString(filename));
		}
	}
	else
	{
		std::cout << "There is no media in the playlist" << std::endl;
	}
}

/** Slots gets called by the data_recieved() signal of the
 ** recieve_data class object. Parses the data received into
 ** xml format, and calls the appropriate functions to handle
 ** the commands specified in the xml */
void Tactek_Display::handle_recieved_data(QString data)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tactek_Display::handle_recieved_data()" << std::endl;
	std::cout << " - Received data size: " << data.size() << std::endl;
#endif //_SHOW_DEBUG_OUTPUT
	pugi::xml_document tacktech;
	tacktech.load(data.toStdString().c_str());
	pugi::xml_node tacktech_node = tacktech.child("Tacktech");
	std::string type_string = tacktech_node.child("Type").attribute("TYPE").as_string();
	if (type_string == "UPLOAD")
	{
        /** Disconnect allows the network_manager smart pointer to be deleted
         *  as no further reference to it exists */
        disconnect(network_manager_file_transfer.get(), SIGNAL(data_recieved(QString)), this,
                        SLOT(handle_recieved_data(QString)));
        network_manager_file_transfer->busy = false;
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Recieved UPLOAD command" << std::endl;
#endif //_SHOW_DEBUG_OUTPUT
		pugi::xml_node computer_node = tacktech_node.child("Items_Node");
		for (pugi::xml_node file_data_node = computer_node.child("Item");
				file_data_node;
				file_data_node = file_data_node.next_sibling("Item"))
		{
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << "Filename: " << file_data_node.child_value("Filename")
					<< std::endl;
			std::cout << "Pause: " << file_data_node.child_value("Pause")
					<< std::endl;
#endif //_SHOW_DEBUG_OUTPUT
			std::string filename = file_data_node.child_value("Filename");
			filename = parameters["general.playlist_directory"] + filename;
			std::string file_data = file_data_node.child_value("File_Data");
#ifdef _SHOW_DEBUG_OUTPUT
                std::cout << "File_Data size: " << file_data.size() << std::endl;
#endif //_SHOW_DEBUG_OUTPUT
			if(file_data.size() > 0)
            {
                class Write_File wf;
                boost::thread write_file_thread(Write_File::write_file, filename, file_data);
            }
//			std::string pause = file_data_node.child_value("Pause");
//			if (pause == "")
//				pause = "0";
//
//			emit new_file_added(QString::fromStdString(filename),
//					boost::lexical_cast<int>(pause));
		}
		check_file_directory();
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << "  **** Server status: " << tacktech_node.child("Status").attribute("STATUS").as_string();
#endif // _SHOW_DEBUG_OUTPUT
		if(tacktech_node.child("Status").attribute("STATUS").as_string() == "MORE_ITEMS")
        {//If there is still items to download, start new request
            check_playlist_items_downloaded();
        }
	}
	else if (type_string == "UPDATE")
    {
    /** Disconnect allows the network_manager smart pointer to be deleted
	 *  as no further reference to it exists */
        disconnect(network_manager_file_transfer.get(), SIGNAL(data_recieved(QString)), this,
					SLOT(handle_recieved_data(QString)));
#ifdef _SHOW_DEBUG_OUTPUT
        std::cout << " - Recieved UPDATE command" << std::endl;
#endif //_SHOW_DEBUG_OUTPUT
        std::string groups_string = tacktech.child("Identification").attribute("GROUPS").as_string();
        std::set<std::string> groups_set = make_set(groups_string);
        /* Update the groups of the display_container */
        m_display_client->set_groups(groups_set);

        pugi::xml_node container_node = tacktech_node.child("CONTAINER").child("Display_Client_Item");
        xml_string_writer writer;
        container_node.print(writer);
        container_node.print(std::cout);

        if (writer.result.size() > 0)
        {
            Display_Client client(writer.result);
            bool playlist_container_changed =
                m_display_client->get_playlist_container()->update_playlist(*client.get_playlist_container(), parameters["general.organization_name"]);
        }
        network_manager_file_transfer->busy = false;
//        if(playlist_container_changed)
//            check_playlist_items_downloaded();
//#ifdef _SHOW_DEBUG_OUTPUT
//        std::cout << "  - Display_Container groups count: " << display_client->get_groups()->size() << std::endl;
//#endif //_SHOW_DEBUG_OUTPUT
//        for(std::set<std::string>::iterator it = display_client->get_groups()->begin();
//            it != display_client->get_groups()->end(); ++it)
//        {
//            display_client->update_playlist_container(
//                    client.get_playlist_container(),
//                    parameters["general.organization"],
//                    *it);
//        }
#ifdef _SHOW_DEBUG_OUTPUT
        m_display_client->get_playlist_container()->print_contents();
#endif //_SHOW_DEBUG_OUTPUT
    }
	else if(type_string == "IDENTIFY")
	{
        /** Disconnect allows the network_manager smart pointer to be deleted
         *  as no further reference to it exists */
        disconnect(network_manager_identification.get(), SIGNAL(data_recieved(QString)), this,
					SLOT(handle_recieved_data(QString)));
		/** This return is just to notify the display that its
		 *  identity has been sent to the server. It does nothing
		 *  more at this moment.
		 */
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Recieved IDENTIFY command" << std::endl;
#endif //_SHOW_DEBUG_OUTPUT
        network_manager_identification->busy = false;
	}
	//tacktech.print(std::cout);
}
void Tactek_Display::handle_new_file_added(QString new_filename, int pause)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tactek_Display::handle_new_file_added()" << std::endl;
#endif
	//QPair<QString, int> playlist_item;
	//playlist_item.first = new_filename;
	//playlist_item.second = pause;
//	playlist->add_to_playlist(playlist_item);
}

void Tactek_Display::check_file_directory()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Tactek_Display::check_file_directory()" << std::endl;
	std::cout << " - Checking directory: " << parameters["general.playlist_directory"] << std::endl;
#endif
    filelist.clear();
    boost::filesystem::path current_dir(parameters["general.playlist_directory"]);
	for (boost::filesystem::recursive_directory_iterator iter(current_dir), end;
		iter != end;
		++iter)
	{
		std::string name = iter->path().generic_string();
#ifdef _SHOW_DEBUG_OUTPUT
        std::cout << " ++ " << name << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
		filelist.push_back(name);
	}
}


std::set<std::string> Tactek_Display::make_set( std::string comma_separated_list)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Tactek_Display::make_vector" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	std::set<std::string> vector_from_string;
	while(comma_separated_list.find(",") != std::string::npos)
	{
		vector_from_string.insert(comma_separated_list.substr(0, comma_separated_list.find(",")));
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " ++ " << comma_separated_list.substr(0, comma_separated_list.find(",")) << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
		comma_separated_list = comma_separated_list.substr(comma_separated_list.find(",") + 1);
	}
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " ++ " << comma_separated_list << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
	vector_from_string.insert(comma_separated_list);
	return vector_from_string;
}

void Tactek_Display::check_display_container()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "=Tactek_Display::check_display_container" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
    std::string upload_string = "<Tacktech>";
    upload_string += "<Type TYPE=\"UPDATE_DISPLAY_CLIENT\" />";
    upload_string += "<Identification ORGANIZATION=\"";
    upload_string += parameters["general.organization_name"];
    upload_string += "\" />";
    upload_string += "<CONTAINER>";
    upload_string += "<Display_Client_Item>";
    upload_string += m_display_client->get_display_client_xml();
    upload_string += "</Display_Client_Item>";
    upload_string += "</CONTAINER>";
    upload_string += "</Tacktech>";

    if(!network_manager_file_transfer->busy)
    {
#ifdef _SHOW_DEBUG_OUTPUT
        std::cout << " - Sending update request" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
        boost::shared_ptr<std::string> string_to_send;
        string_to_send.reset(new std::string(upload_string));
        io_service_file_transfer->reset();
        network_manager_file_transfer.reset(
                    new Tacktech_Network_Manager(*io_service_file_transfer, parameters));
        network_manager_file_transfer->connect(parameters["general.server_ip"],
                parameters["general.server_port"]);
        network_manager_file_transfer->busy = true;
        connect(network_manager_file_transfer.get(), SIGNAL(data_recieved(QString)), this,
                    SLOT(handle_recieved_data(QString)));
        network_manager_file_transfer->start_write(string_to_send);
        boost::thread t(
            boost::bind(&boost::asio::io_service::run, boost::ref(io_service_file_transfer)));
        t.join();
    }
    else
    {
 #ifdef _SHOW_DEBUG_OUTPUT
        std::cout << " - Network Manager busy, not sending request" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
    }
}
void Tactek_Display::display_news(QString news)
{
    ui->news_label->setText(news);
}
