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
#include "ui_Edit_Playlist.h"
#include "Typed_QTreeWidgetItem.h"
#include "Add_File_Dialog.h"

class Edit_Playlist: public QWidget
{
Q_OBJECT

public:
	Edit_Playlist(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Edit_Playlist();
	void set_playlist_container(Playlist_Container_Ptr);
	void set_organization_name(std::string);
	void set_filelist(Filelist_Ptr);
private:
	Ui::Edit_PlaylistClass ui;
	Playlist_Container_Ptr playlist;
	Playlist_Container *original_playlist;
	Filelist_Ptr filelist;

	boost::shared_ptr<Add_File_Dialog> add_file_dialog;

	/* GUI variables */
	QMenu *node_menu;
	QAction *add_playlist;
	QAction *add_file;
	QAction *remove_playlist;
	QAction *remove_file;

	std::string m_organization_name;

	void keyPressEvent(QKeyEvent *event);
private slots:
	/* Primary slots, called by this class */
	//void add_file_slot();
	void remove_playlist_slot();
	void remove_file_slot();
	void ok_clicked();
	void cancel_clicked();
	void create_playlist_slot();

	/* Seconday slots */
	void repopulate_widget();
signals:
	void playlist_changed();

};

#endif // EDIT_PLAYLIST_H
