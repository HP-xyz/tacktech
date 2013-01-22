#ifndef SELECT_PLAYLIST_DIALOG_H
#define SELECT_PLAYLIST_DIALOG_H

#include <QtGui/QDialog>
#include <Group_Playlist_Container.h>
#include <Playlist_Container.h>
#include "ui_Select_Playlist_Dialog.h"

class Select_Playlist_Dialog: public QDialog
{
Q_OBJECT

public:
	Select_Playlist_Dialog(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Select_Playlist_Dialog();
	void set_playlist(Playlist_Container_Ptr);
	void set_group_playlist(Group_Playlist_Container_Ptr);
	void set_selected_group(QString);
private:
	Ui::Select_Playlist_DialogClass ui;
	Playlist_Container_Ptr playlist;
	Group_Playlist_Container_Ptr group_playlist;

	QString selected_group;
private slots:
	void ok_clicked();
	void cancel_clicked();signals:
	void group_playlist_changed();
};

#endif // SELECT_PLAYLIST_DIALOG_H
