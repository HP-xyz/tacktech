#ifndef TACKTECH_MANAGER_H
#define TACKTECH_MANAGER_H

#include <QtGui/QMainWindow>
#include <QMap>
#include <QDate>
#include <QMenu>
#include <QAction>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <pugixml.hpp>
#include "Group_Container.h"
#include "Playlist_Container.h"
#include "Group_Playlist_Container.h"
#include "Edit_Group.h"
#include "Edit_Playlist.h"
#include "Select_Playlist_Dialog.h"
#include "Upload_Data.h"
#include "Upload_Data_Container.h"
#include "ui_Tacktech_Manager.h"

class Tacktech_Manager : public QMainWindow
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

    void save_variables_to_xml();
    void read_variables_from_xml();

protected:
    /* Variable for computer names and group names
     * Note: Format is groups_and_computers[group_index][computer_index]
     * Note: This can be hostnames or IP addresses */
    Group_Container *groups_and_computers;

    /* Variable for the playlist */
    Playlist_Container *playlist;

    /* Variable for the group_playlist container */
    Group_Playlist_Container *group_playlist;

	/* Variable for the upload_data container */
	Upload_Data_Container *upload_data;
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

    void group_editing_complete();
    void repopulate_widget();
};

#endif // TACKTECH_MANAGER_H
