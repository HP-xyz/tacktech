#ifndef TACKTECH_MANAGER_H
#define TACKTECH_MANAGER_H

#include <QtGui/QMainWindow>
#include <QMap>
#include <iostream>
#include "Group_Container.h"
#include "Playlist_Container.h"
#include "Edit_Group.h"
#include "Edit_Playlist.h"
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

	/* Global variable for Edit_Group GUI */
	Edit_Group *edit_group_class;
	Edit_Playlist *edit_playlist_class;

protected:
	/* Variable for computer names and group names
	 * Note: Format is groups_and_computers[group_index][computer_index]
	 * Note: This can be hostnames or IP addresses */
	Group_Container *groups_and_computers;

	/* Variable for the playlis */
	Playlist_Container *playlist;

private slots:
	void refresh_all_groups();

	/* Slots that start GUIs (this)*/
	/* ===================== */
	void edit_group();
	void create_playlist();
	void edit_playlist();
	void edit_preferences();

	/* Slots that receive signals from GUIs (not this) */
	/* ==================================== */
	void group_editing_complete();
};

#endif // TACKTECH_MANAGER_H
