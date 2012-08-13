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
	read_config();
	Tactek_Display::update_timer = new QTimer(this);
	Tactek_Display::check_update_timer = new QTimer(this);
	Tactek_Display::playlist = new Playlist();
	server = new Recieve_Data();

	m_vlc_instance = new VlcInstance(VlcCommon::args(), this);
	m_vlc_player = new VlcMediaPlayer(m_vlc_instance);
	m_vlc_player->setVideoWidget(ui->vlc_video_widget);

	ui->vlc_video_widget->setMediaPlayer(m_vlc_player);

	connect(update_timer, SIGNAL(timeout()), this, SLOT(check_media_state()));
	connect(check_update_timer, SIGNAL(timeout()), this, SLOT(check_for_updates()));
	connect(this, SIGNAL(start_next_media()), this,
			SLOT(play_next_media_in_queue()));
	connect(server, SIGNAL(data_recieved(std::string)), this,
			SLOT(handle_recieved_data(std::string)));
	connect(this, SIGNAL(new_file_added(QString, int)), this,
			SLOT(handle_new_file_added(QString, int)));

	update_timer->start(1000);
	check_update_timer->start(30000);
}

Tactek_Display::~Tactek_Display()
{
	delete ui;
	delete m_vlc_media;
	delete m_vlc_player;
	delete m_vlc_instance;
	delete update_timer;
	delete check_update_timer;
	delete server;
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
	pugi::xml_document document;
	pugi::xml_node tacktech_node = document.append_child("Tacktech");
	pugi::xml_node type_node = tacktech_node.append_child("Type");
	type_node.append_attribute("TYPE") = "GET_UPDATES";
	pugi::xml_node files_node = tacktech_node.append_child("Files");
	pugi::xml_node playlist_node = tacktech_node.append_child("Playlist");
	playlist_name = "Playlist 1";
	playlist_node.append_attribute("PLAYLIST") = playlist_name.toStdString().c_str();
	for (int i = 0; i < playlist->get_playlist().size(); i++)
	{
		files_node.append_attribute("FILE") =
				playlist->get_playlist().at(i).first.toStdString().c_str();
	}
	xml_string_writer writer;
	document.save(writer);
	document.print(std::cout);
	Send_Data *send_data = new Send_Data(parameters["general.server_ip"].c_str(),
					boost::lexical_cast<int>(parameters["general.server_port"]),
					writer.result);
}

/**
 * Opens and plays the file specified in the filepath parameter.
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
	if (playlist->get_playlist().size() > 0)
	{
		if (playlist->current_index_of_queue == playlist->get_playlist().size())
			playlist->current_index_of_queue = 0;
		QPair<QString, int> queue_item =
				playlist->get_playlist()[playlist->current_index_of_queue];
		if (queue_item.second == 0)
		{
			open_and_play(queue_item.first);
		}
		else
		{
			std::cout << "Pausing for: " << queue_item.second * 1000
					<< std::endl;
			QTimer::singleShot((queue_item.second * 1000), this, NULL);
			open_and_play(queue_item.first);
		}
		playlist->current_index_of_queue += 1;
	}
	else
	{
		std::cout << "There is no media in the playlist" << std::endl;
	}
}

/** Slots gets called by the data_recieved() signal of the
 ** recieve_data class object. Parses the data recieved into
 ** xml format, and calls the appropriate functions to handle
 ** the commands specified in the xml */
void Tactek_Display::handle_recieved_data(std::string data)
{
#ifdef _DEBUG
	std::cout << "= Tactek_Display::handle_recieved_data()" << std::endl;
#endif //_DEBUG
	pugi::xml_document tacktech;
	tacktech.load(data.c_str());
	pugi::xml_node computer_node = tacktech.child("Computer");
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
	//tacktech.print(std::cout);
}
void Tactek_Display::handle_new_file_added(QString new_filename, int pause)
{
#ifdef _DEBUG
	std::cout << "= Tactek_Display::handle_new_file_added()" << std::endl;
#endif
	QPair<QString, int> playlist_item;
	playlist_item.first = new_filename;
	playlist_item.second = pause;
	playlist->add_to_playlist(playlist_item);
}
