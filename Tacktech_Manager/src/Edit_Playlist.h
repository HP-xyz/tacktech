#ifndef EDIT_PLAYLIST_H
#define EDIT_PLAYLIST_H

#ifdef _SHOW_DEBUG_OUTPUT
#include <iostream>
#endif // _DEBUG
#include <QtGui/QWidget>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QKeyEvent>
#include <boost/lexical_cast.hpp>
#include <Playlist_Container.h>
#include <Filelist.h>
#include <Display_Client_Container.h>
#include "ui_Edit_Playlist.h"
#include "Typed_QTreeWidgetItem.h"
#include "Add_File_Dialog.h"

class Edit_Playlist: public QWidget
{
Q_OBJECT

public:
	Edit_Playlist(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Edit_Playlist();
	void set_organization_name(std::string);

	/* MUST be called before set_display_client_container */
	void set_group_name(std::string);

	void set_filelist(Filelist_Ptr);
	void set_display_client_container(Display_Client_Container_Ptr);
private:
	Ui::Edit_PlaylistClass ui;
	Filelist_Ptr filelist;
	Display_Client_Container_Ptr display_client_container;

	std::vector<Display_Client_Ptr> display_client_list;
	std::vector<Playlist_Ptr> playlist_list;

	boost::shared_ptr<Add_File_Dialog> add_file_dialog;

	/* GUI variables */
	QMenu *node_menu;
	QAction *add_playlist;
	QAction *add_file;
	QAction *remove_playlist;
	QAction *remove_file;

	std::string m_organization_name;
	std::string m_group_name;

	void keyPressEvent(QKeyEvent *event);
private slots:
	/* Primary slots, called by this class */
	void add_file_slot();
	void remove_playlist_slot();
	void remove_file_slot();
	void ok_clicked();
	void cancel_clicked();
	void create_playlist_slot();

	/* Seconday slots */
	void playlist_added_slot(Playlist_Ptr);
	void repopulate_widget();
signals:
	void playlist_changed(Display_Client_Container_Ptr);
};

#endif // EDIT_PLAYLIST_H
