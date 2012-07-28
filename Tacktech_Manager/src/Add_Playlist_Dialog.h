#ifndef ADD_PLAYLIST_DIALOG_H
#define ADD_PLAYLIST_DIALOG_H

#include <QtGui/QMainWindow>
#include <QMessageBox>
#include <QKeyEvent>
#include "ui_Add_Playlist_Dialog.h"
#include <Playlist_Container.h>
class Add_Playlist_Dialog : public QMainWindow
{
    Q_OBJECT

public:
    Add_Playlist_Dialog(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~Add_Playlist_Dialog();
    void set_playlist(Playlist_Container_Ptr);
private:
    Ui::Add_Playlist_DialogClass ui;
    Playlist_Container_Ptr playlist;

	void keyPressEvent(QKeyEvent *event);
private slots:
    void ok_clicked();
    void cancel_clicked();
signals:
    void playlist_name_added();
};

#endif // ADD_PLAYLIST_DIALOG_H
