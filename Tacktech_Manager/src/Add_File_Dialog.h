#ifndef ADD_FILE_DIALOG_H
#define ADD_FILE_DIALOG_H

#ifdef _SHOW_DEBUG_OUTPUT
#include <iostream>
#endif // _DEBUG
#include <QtGui/QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QSpinBox>
#include <QKeyEvent>
#include "ui_Add_File_Dialog.h"
#include <Playlist_Container.h>
#include "Typed_QTreeWidgetItem.h"
#include "Add_Pause_Dialog.h"
#include "Add_Playlist_Dialog.h"

class Add_File_Dialog: public QMainWindow
{
Q_OBJECT

public:
	Add_File_Dialog(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Add_File_Dialog();
	void set_playlist(Playlist_Container_Ptr);
	void set_playlist_name(QString);
private:
	Ui::Add_File_DialogClass ui;
	Playlist_Container *original_playlist;
	Playlist_Container_Ptr playlist;

	Add_Pause_Dialog *add_pause_dialog;
	QString playlist_name;

	QMenu *node_menu;
	QAction *remove_filename;

	void repopulate_widget();
	void keyPressEvent(QKeyEvent *event);
private slots:
	void ok_clicked();
	void cancel_clicked();
	void remove_filename_slot();
	void add_filename_slot();
	void add_pause_slot(QTreeWidgetItem*, int);

	/* Secondary slots, called by other class */
	void pause_changed_slot(int);
	void pause_unchanged_slot();

signals:
	void filelist_changed();
};

#endif // ADD_FILE_DIALOG_H
