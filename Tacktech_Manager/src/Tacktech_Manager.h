#ifndef TACKTECH_MANAGER_H
#define TACKTECH_MANAGER_H
#include <QtGui/QMainWindow>
#include <QMap>
#include <QDate>
#include <QMenu>
#include <QAction>
#include <iostream>
#include <fstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <pugixml.hpp>
#include <Group_Container.h>
#include <Playlist_Container.h>
#include <Group_Playlist_Container.h>
#include <Tacktech_Network_Manager.h>
#include "Edit_Group.h"
#include "Edit_Playlist.h"
#include "Select_Playlist_Dialog.h"
#include "Upload_Data.h"
#include "Upload_Data_Container.h"
#include "ui_Tacktech_Manager.h"
typedef boost::shared_ptr<Upload_Data_Container> Upload_Data_Container_Ptr;
class Tacktech_Manager: public QMainWindow
{
Q_OBJECT
public:
	Tacktech_Manager(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Tacktech_Manager();

private:
	/* The global ui variable */
	Ui::MainWindow ui;

	QMenu *node_menu;
	QAction *schedule_upload;

	Edit_Group *edit_group_class;
	Edit_Playlist *edit_playlist_class;
	Select_Playlist_Dialog *select_playlist_dialog;
	Upload_Data *upload_data_dialog;

	Tacktech_Network_Manager_Ptr network_manager;
	boost::shared_ptr<boost::asio::io_service> io_service;

	/** Variables for the config file */
	std::map<std::string, std::string> parameters;
	std::set<std::string> options;

	/** Reads a config file*/
	void read_config();

protected:
	/* Variable for computer names and group names
	 * Note: Format is groups_and_computers[group_index][computer_index]
	 * Note: This can be hostnames or IP addresses */
	Group_Container_Ptr groups_and_computers;

	/* Variable for the playlist */
	Playlist_Container_Ptr playlist;

	/* Variable for the group_playlist container */
	Group_Playlist_Container_Ptr group_playlist;

	/* Variable for the upload_data container */
	Upload_Data_Container_Ptr upload_data;

private slots:
	void refresh_all_groups();
	/* Slots that start GUIs (this)*/
	/* ===================== */
	void edit_group();
	void create_playlist();
	void edit_playlist();
	void edit_preferences();
	void show_playlist_selection(QTreeWidgetItem*, int);
	void show_schedule_upload_dialog();
	void scheduled_item_added(QDate);
	void start_upload(std::string);
	void refresh_all_request();
	void data_recieved_slot(QString data_recieved);
	void upload_new_variables_slot();

	void group_editing_complete();
	void repopulate_widget();
};

#endif // TACKTECH_MANAGER_H
