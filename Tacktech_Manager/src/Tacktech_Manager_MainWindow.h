#ifndef TACKTECH_MANAGER_MAINWINDOW_H
#define TACKTECH_MANAGER_MAINWINDOW_H
#include <QtGui/QMainWindow>
#include <QtGui/QWidget>
#include <QtCore/QStringList>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/date_time.hpp>
#include <Group_Container.h>
#include <Playlist_Container.h>
#include <Group_Playlist_Container.h>
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

	/* Variable for the upload_data container */
	Upload_Data_Container_Ptr upload_data;

	void read_config();
	/** Repopulates the UI, updating the main_treewidget */
	void repopulate_ui();
};

#endif //TACKTECH_MANAGER_MAINWINDOW_H