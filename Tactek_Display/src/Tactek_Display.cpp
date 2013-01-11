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

/**
 * Initiates and sets up the class for usage. Connects the check_media_state and
 * play_next_media_in_queue signals to their respective slots.
 * @param parent
 */
Tactek_Display::Tactek_Display(QWidget *parent) :
		QMainWindow(parent), ui(new Ui::Tactek_Display), m_vlc_media(0)
{
	ui->setupUi(this);
	display_client.reset(new Display_Client());
	read_config();

	display_client->set_identification(parameters["general.computer_name"]);
	display_client->add_organization(parameters["general.computer_name"]);

	Tactek_Display::update_timer = new QTimer(this);
	Tactek_Display::check_update_timer = new QTimer(this);
	Tactek_Display::identify_timer = new QTimer(this);

	io_service.reset(new boost::asio::io_service);
	network_manager.reset(
		new Tacktech_Network_Manager(*io_service, parameters));

	m_vlc_instance = new VlcInstance(VlcCommon::args(), this);
	m_vlc_player = new VlcMediaPlayer(m_vlc_instance);
	m_vlc_player->setVideoWidget(ui->vlc_video_widget);

	ui->vlc_video_widget->setMediaPlayer(m_vlc_player);

	connect(update_timer, SIGNAL(timeout()), this, SLOT(check_media_state()));
	connect(check_update_timer, SIGNAL(timeout()), this, SLOT(check_for_updates()));
	connect(identify_timer, SIGNAL(timeout()), this, SLOT(update_display_client()));
	connect(this, SIGNAL(start_next_media()), this,
			SLOT(play_next_media_in_queue()));
	connect(this, SIGNAL(new_file_added(QString, int)), this,
			SLOT(handle_new_file_added(QString, int)));

	update_timer->start(1000);
	check_update_timer->start(180000);
	identify_timer->start(30000);
	check_for_updates();
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
}

void Tactek_Display::read_config()
{
#ifdef _DEBUG
	std::cout << "= Tacktech_Manager::read_config()" << std::endl;
#endif // _DEBUG
	std::ifstream config("config.ini");
#ifdef _DEBUG
	std::cout << " - Created config ifstream" << std::endl;
#endif // _DEBUG
	if (!config)
	{
		std::cerr << " == Error -> No config found" << std::endl;
		;
	}

#ifdef _DEBUG
	std::cout << " - Creating options.insert" << std::endl;
#endif // _DEBUG
	options.insert("*");

	try
	{
#ifdef _DEBUG
		std::cout << "Config file read: " << std::endl << "==================="
				<< std::endl;
#endif
		for (boost::program_options::detail::config_file_iterator i(config,
				options), e; i != e; ++i)
		{
#ifdef _DEBUG
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

void Tactek_Display::check_for_updates()
{
#ifdef _DEBUG
	std::cout << "= Tacktech_Manager::check_for_updates()" << std::endl;
#endif // _DEBUG
    /* First, we check what files we have... */
    std::vector<std::string> files_needed;
    for(Container::iterator it = display_client->get_playlist_container()->get_playlist_container()->begin();
        it != display_client->get_playlist_container()->get_playlist_container()->end(); ++it)
    {
        for(std::vector< std::pair<std::string,int> >::iterator it2 = it->first->get_playlist_items()->begin();
            it2 != it->first->get_playlist_items()->end(); ++it2)
        {
            std::vector<std::string>::iterator item_found = std::find(filelist.begin(), filelist.end(), it2->first);
            if(item_found == filelist.end())
            {//We do not have the needed file
                files_needed.push_back(it2->first);
            }
        }
    }
    if(files_needed.size() > 0)
    {
        pugi::xml_document document;
        pugi::xml_node tacktech_node = document.append_child("Tacktech");
        pugi::xml_node type_node = tacktech_node.append_child("Type");
        type_node.append_attribute("TYPE") = "GET_UPDATES";
        pugi::xml_node identification_node = tacktech_node.append_child("Identification_Node");
        identification_node.append_attribute("Identification") = display_client->get_identification().c_str();
        identification_node.append_attribute("Organization") = parameters["general.computer_name"].c_str();
        pugi::xml_node file_node = tacktech_node.append_child("File_Node");
        for(std::vector<std::string>::iterator it = files_needed.begin(); it != files_needed.end(); ++it)
            file_node.append_attribute("File") = it->c_str();
        xml_string_writer writer;
        document.save(writer);
        document.print(std::cout);

        boost::shared_ptr<std::string> string_to_send;
        string_to_send.reset(new std::string(writer.result));

        io_service->reset();
        network_manager.reset(
                new Tacktech_Network_Manager(*io_service, parameters));
        network_manager->connect(parameters["general.server_ip"],
                parameters["general.server_port"]);
        connect(network_manager.get(), SIGNAL(data_recieved(QString)), this,
                    SLOT(handle_recieved_data(QString)));
        network_manager->start_write(string_to_send);
        boost::thread t(
            boost::bind(&boost::asio::io_service::run, boost::ref(io_service)));
        t.join();
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
#ifdef _DEBUG
	std::cout << "= Tacktech_Manager::send_identity_to_server()" << std::endl;
#endif // _DEBUG
    display_client->get_display_client_xml();

	boost::shared_ptr<std::string> string_to_send;
	string_to_send.reset(new std::string(display_client->get_display_client_xml()));

	io_service->reset();
	network_manager.reset(
				new Tacktech_Network_Manager(*io_service, parameters));
	network_manager->connect(parameters["general.server_ip"],
			parameters["general.server_port"]);
	connect(network_manager.get(), SIGNAL(data_recieved(QString)), this,
				SLOT(handle_recieved_data(QString)));
	network_manager->start_write(string_to_send);
	boost::thread t(
		boost::bind(&boost::asio::io_service::run, boost::ref(io_service)));
	t.join();
}

/**
 * Opens and plays the file specified in the filepath parameter.send_identity_to_server
 * @param filepath Path to the file that needs to be played
 */
void Tactek_Display::open_and_play(QString filepath)
{
	m_vlc_media = new VlcMedia(filepath, true, m_vlc_instance);
	m_vlc_player->open(m_vlc_media);
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
    std::pair<std::string, int> next_item = display_client->get_playlist_container()->get_next_item();
	if (next_item.first != "NO ITEMS")
	{
		if (next_item.second == 0)
		{
			open_and_play(QString::fromStdString(next_item.first));
		}
		else
		{
			std::cout << "Pausing for: " << next_item.second * 1000
					<< std::endl;
			QTimer::singleShot((next_item.second * 1000), this, NULL);
			open_and_play(QString::fromStdString(next_item.first));
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
#ifdef _DEBUG
	std::cout << "= Tactek_Display::handle_recieved_data()" << std::endl;
	std::cout << " - Received data size: " << data.size() << std::endl;
#endif //_DEBUG
	/** Disconnect allows the network_manager smart pointer to be deleted
	 *  as no further reference to it exists */
	disconnect(network_manager.get(), SIGNAL(data_recieved(QString)), this,
					SLOT(handle_recieved_data(QString)));
	pugi::xml_document tacktech;
	tacktech.load(data.toStdString().c_str());
	pugi::xml_node tacktech_node = tacktech.child("Tacktech");
	std::string type_string = tacktech_node.child("Type").attribute("TYPE").as_string();
	if (type_string == "UPLOAD")
	{
#ifdef _DEBUG
	std::cout << " - Recieved UPLOAD command" << std::endl;
#endif //_DEBUG
		pugi::xml_node computer_node = tacktech_node.child("Computer");
		for (pugi::xml_node file_data_node = computer_node.child("Item");
				file_data_node;
				file_data_node = file_data_node.next_sibling("Item"))
		{
#ifdef _DEBUG
			std::cout << "Filename: " << file_data_node.child_value("Filename")
					<< std::endl;
			std::cout << "Pause: " << file_data_node.child_value("Pause")
					<< std::endl;
#endif //_DEBUG
			std::string filename = file_data_node.child_value("Filename");
			std::string file_data = file_data_node.child_value("File_Data");
			std::stringstream encoded_stream;
			std::stringstream decoded_stream;

			encoded_stream << file_data;
			base64::decoder D;
			D.decode(encoded_stream, decoded_stream);
			file_data = decoded_stream.str();

#ifdef _DEBUG
			std::cout << "File_Data size: " << file_data.size() << std::endl;
#endif

			std::ofstream out_file(filename.c_str(), std::ios::binary);
			out_file << file_data;
			out_file.close();

			std::string pause = file_data_node.child_value("Pause");
			if (pause == "")
				pause = "0";

			emit new_file_added(QString::fromStdString(filename),
					boost::lexical_cast<int>(pause));
		}
	}
	else if(type_string == "IDENTIFY")
	{
		/** This return is just to notify the display that its
		 *  identity has been sent to the server. It does nothing
		 *  more at this moment.
		 */
#ifdef _DEBUG
	std::cout << " - Recieved IDENTIFY command" << std::endl;
#endif //_DEBUG
	}
	//tacktech.print(std::cout);
}
void Tactek_Display::handle_new_file_added(QString new_filename, int pause)
{
#ifdef _DEBUG
	std::cout << "= Tactek_Display::handle_new_file_added()" << std::endl;
#endif
	//QPair<QString, int> playlist_item;
	//playlist_item.first = new_filename;
	//playlist_item.second = pause;
//	playlist->add_to_playlist(playlist_item);
}

void Tactek_Display::check_file_directory()
{
#ifdef _DEBUG
	std::cout << "= Tactek_Display::check_file_directory()" << std::endl;
#endif
    boost::filesystem::path current_dir(parameters["general.playlist_directory"]);
	for (boost::filesystem::recursive_directory_iterator iter(current_dir), end;
		iter != end;
		++iter)
	{
		std::string name = iter->path().generic_string();
		filelist.push_back(name);
	}
}
