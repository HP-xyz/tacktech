#ifndef ADD_FILE_DIALOG_H
#define ADD_FILE_DIALOG_H

#ifdef _SHOW_DEBUG_OUTPUT
#include <iostream>
#endif // _DEBUG
#include <vector>
#include <QtGui/QMainWindow>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QFileDialog>
#include <QSpinBox>
#include <QKeyEvent>
#include <boost/lexical_cast.hpp>
#include <Playlist_Container.h>
#include <Filelist.h>
#include "ui_Add_File_Dialog.h"
#include "Typed_QTreeWidgetItem.h"
#include "Add_Pause_Dialog.h"
#include "Edit_Times.h"

class Add_File_Dialog: public QMainWindow
{
Q_OBJECT

public:
	Add_File_Dialog(Filelist_Ptr, QString, QString, QString, QWidget *parent = 0, Qt::WFlags flags = 0);
	~Add_File_Dialog();
	void set_filelist(Filelist_Ptr);
	void set_playlist_name(QString);
	void set_playlist_organization(QString);
	void set_playlist_group(QString);
private:
	Ui::Add_File_DialogClass ui;

	Filelist_Ptr filelist;
	Playlist_Ptr playlist;

	Add_Pause_Dialog *add_pause_dialog;
	QString playlist_name;
	QString playlist_organization;
	QString playlist_group;

	QMenu *node_menu;
	QAction *add_pause;

	void repopulate_widget();
	void add_new_playlist();
	void keyPressEvent(QKeyEvent *event);
	std::string make_list(std::set<std::string>);
private slots:
	void ok_clicked();
	void cancel_clicked();
	void add_file_slot();
	void remove_file_slot();
	void add_pause_slot();
	void add_pause_slot(QTreeWidgetItem*, int);

	/* Secondary slots, called by other class */
	void pause_changed_slot(int);
	void pause_unchanged_slot();
signals:
	void playlist_added(Playlist_Ptr);
};

#endif // ADD_FILE_DIALOG_H
