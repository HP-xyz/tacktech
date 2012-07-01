#ifndef EDIT_PLAYLIST_H
#define EDIT_PLAYLIST_H

#include <QtGui/QWidget>
#include <QMenu>
#include <QAction>
#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG
#include "ui_Edit_Playlist.h"
#include "Playlist_Container.h"

class Edit_Playlist : public QWidget
{
	Q_OBJECT

public:
	Edit_Playlist(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Edit_Playlist();
	void set_playlist(Playlist_Container*);
private:
	Ui::Edit_PlaylistClass ui;
	Playlist_Container *playlist;

	/* GUI variables */
	QMenu *node_menu;
	QAction *add_playlist;
	QAction *add_file;
	QAction *remove_playlist;
	QAction *remove_file;

	/* Primary slots, called by this class */
	void add_playlist_slot();
	void add_file_slot();
	void remove_playlist_slot();
	void remove_file_slot();
};

#endif // EDIT_PLAYLIST_H
