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

#ifndef TACTEK_DISPLAY_H
#define TACTEK_DISPLAY_H

#include <QtGui/QMainWindow>
#include <QtGui/QFileDialog>
#include <QTimer>
#include <QPair>
#include <vlc-qt/Common.h>
#include <vlc-qt/Instance.h>
#include <vlc-qt/Media.h>
#include <vlc-qt/MediaPlayer.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
//#include <boost/lexical_cast.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <b64/decode.h>
#include <pugixml.hpp>
#include <Tacktech_Network_Manager.h>
#include <Display_Client.h>
#include "ui_Tactek_Display.h"

namespace Ui
{
class Tactek_Display;
}

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

class Tactek_Display: public QMainWindow
{
Q_OBJECT
public:
	explicit Tactek_Display(QWidget *parent = 0);
	~Tactek_Display();signals:
	void start_next_media();
	void new_file_added(QString, int);
	void set_playlist_name(QString);
private slots:
	void check_media_state();
	void check_playlist_items_downloaded();
	void update_display_client();
	void play_next_media_in_queue();
	void open_and_play(QString);
	void handle_recieved_data(QString data);
	void handle_new_file_added(QString, int);
	void check_file_directory();
	void check_display_container();
private:
	Ui::Tactek_Display *ui;
	Display_Client_Ptr display_client;
	QTimer *update_timer;
	QTimer *check_update_timer;
	QTimer *identify_timer;
	QTimer *update_display_client_timer;
	VlcInstance *m_vlc_instance;
	VlcMedia *m_vlc_media;
	VlcMediaPlayer *m_vlc_player;

	Tacktech_Network_Manager_Ptr network_manager;
	boost::shared_ptr<boost::asio::io_service> io_service;

	QString playlist_name;
	std::vector<std::string> filelist;

	/** Variables for the config file */
	std::map<std::string, std::string> parameters;
	std::set<std::string> options;

	/** Reads a config file*/
	void read_config();

	std::set<std::string> make_set(std::string);
};

#endif // TACTEK_DISPLAY_H
