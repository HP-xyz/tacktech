#ifndef EDIT_PLAYLIST_H
#define EDIT_PLAYLIST_H

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG
#include <QtGui/QWidget>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include "ui_Edit_Playlist.h"
#include "Playlist_Container.h"
#include "Add_Playlist_Dialog.h"
#include "Add_File_Dialog.h"
#include "Typed_QTreeWidgetItem.h"

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
    Playlist_Container *original_playlist;

    Add_Playlist_Dialog *add_playlist_dialog;
    Add_File_Dialog *add_file_dialog;

    /* GUI variables */
    QMenu *node_menu;
    QAction *add_playlist;
    QAction *add_file;
    QAction *remove_playlist;
    QAction *remove_file;

private slots:
    /* Primary slots, called by this class */
    void add_playlist_slot();
    void add_file_slot();
    void remove_playlist_slot();
    void remove_file_slot();
    void ok_clicked();
    void cancel_clicked();

    /* Seconday slots */
    void repopulate_widget();
signals:
    void playlist_changed();
};

#endif // EDIT_PLAYLIST_H
