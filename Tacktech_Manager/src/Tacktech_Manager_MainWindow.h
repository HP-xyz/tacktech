#ifndef TACKTECH_MANAGER_MAINWINDOW_H
#define TACKTECH_MANAGER_MAINWINDOW_H
#include <QtGui/QMainWindow>
#include <QtGui/QWidget>
#include <QtCore/QStringList>
#include <QtCore/QTimer>
#include <set>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/date_time.hpp>
#include <Playlist_Container.h>
#include <Display_Client_Container.h>
#include <Tacktech_Network_Manager.h>
#include "Upload_Data_Container.h"
#include "Typed_QTreeWidgetItem.h"
#include "ui_Tacktech_Manager_MainWindow.h"
typedef boost::shared_ptr<Upload_Data_Container> Upload_Data_Container_Ptr;
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

	/* Variable for the upload_data container */
	Upload_Data_Container_Ptr upload_data;

	Tacktech_Network_Manager_Ptr network_manager;
	boost::shared_ptr<boost::asio::io_service> io_service;
	Display_Client_Container_Ptr display_client_container;
	Playlist_Container_Ptr playlist_container;
private slots:
	void read_config();
	/** Repopulates the UI, updating the main_treewidget */
	void repopulate_ui();
	void refresh_all_request();
	void start_upload(std::string);
	void data_recieved_slot(QString data_recieved);
};

#endif //TACKTECH_MANAGER_MAINWINDOW_H