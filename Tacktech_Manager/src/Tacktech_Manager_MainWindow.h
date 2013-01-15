#ifndef TACKTECH_MANAGER_MAINWINDOW_H
#define TACKTECH_MANAGER_MAINWINDOW_H
#include <QtGui/QMainWindow>
#include <QtGui/QWidget>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtCore/QStringList>
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <set>
#include <vector>
#include <utility>
#include <algorithm>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/date_time.hpp>
#include <Playlist_Container.h>
#include <Display_Client_Container.h>
#include <Tacktech_Network_Manager.h>
#include <Filelist.h>
#include "Upload_Data_Container.h"
#include "Upload_Files_To_Server_Dialog.h"
#include "Assign_Group.h"
#include "Typed_QTreeWidgetItem.h"
#include "Edit_Playlist.h"
#include "ui_Tacktech_Manager_MainWindow.h"
typedef boost::shared_ptr<Upload_Files_To_Server_Dialog> File_Upload_Dialog_Ptr;
typedef boost::shared_ptr<Upload_Data_Container> Upload_Data_Container_Ptr;
typedef boost::shared_ptr<std::vector<std::pair<std::vector<std::string>, QTime> > > Pending_Uploads;
class Tacktech_Manager_MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	Tacktech_Manager_MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Tacktech_Manager_MainWindow();
private:
	Ui::Tacktech_Manager_MainWindow ui;
	std::map<std::string, std::string> parameters;
	std::set<std::string> options;
	QTimer *refresh_timer;

	boost::shared_ptr<QMenu> node_menu;
	boost::shared_ptr<QAction> add_to_group;
	boost::shared_ptr<QAction> edit_playlist;

    boost::shared_ptr<Edit_Playlist> edit_playlist_dialog;

	/* Variable for the upload_data container */
	Upload_Data_Container_Ptr upload_data;

	Tacktech_Network_Manager_Ptr network_manager;
	boost::shared_ptr<boost::asio::io_service> io_service;
	Display_Client_Container_Ptr display_client_container;
	Playlist_Container_Ptr playlist_container;
	Filelist_Ptr filelist;

	Display_Client_Container_Ptr temp_display_client_container;
	Playlist_Container_Ptr temp_playlist_container;

	/** Holds the pending uploads that will go to the server. Consists
	 ** of a std::vector of pairs, where the pair consists of a std::vector
	 ** containing the filenames to be uploaded, and a boost::posix_time::ptime
	 ** specifying the time the upload should happen. */
	 Pending_Uploads pending_uploads;

	 File_Upload_Dialog_Ptr file_upload_dialog;

	 boost::shared_ptr<Assign_Group> assign_group_dialog;

private slots:
	void read_config();
	/** Repopulates the UI, updating the main_treewidget */
	void repopulate_ui();
	void show_upload_gui();
	void refresh_all_request();
	void start_upload(std::string);
	void upload_files_to_server(std::vector<std::string>, QTime);
	void data_recieved_slot(QString data_recieved);
	void edit_playlist_slot();
	void check_uploads_pending();
	void assign_group();
	void display_container_changed(Display_Client_Container_Ptr);
	void display_container_changed();
};

#endif //TACKTECH_MANAGER_MAINWINDOW_H
