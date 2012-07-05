#include "Select_Playlist_Dialog.h"

Select_Playlist_Dialog::Select_Playlist_Dialog(QWidget *parent, Qt::WFlags flags)
    : QDialog(parent, flags)
{
    ui.setupUi(this);
    connect(ui.buttonBox, SIGNAL(accepted()),
            this, SLOT(ok_clicked()));
    connect(ui.buttonBox, SIGNAL(rejected()),
            this, SLOT(cancel_clicked()));
}

Select_Playlist_Dialog::~Select_Playlist_Dialog()
{

}

void Select_Playlist_Dialog::set_playlist( Playlist_Container *p_playlist )
{
    playlist = p_playlist;
    ui.playlist_combobox->clear();
    foreach(QString playlist_name, playlist->get_playlist().uniqueKeys())
    ui.playlist_combobox->addItem(playlist_name);
}

void Select_Playlist_Dialog::set_group_playlist(
    Group_Playlist_Container *p_group_playlist )
{
    group_playlist = p_group_playlist;
}

void Select_Playlist_Dialog::set_selected_group( QString p_selected_group )
{
    selected_group = p_selected_group;
}

void Select_Playlist_Dialog::ok_clicked()
{
    group_playlist->connect_group_to_playlist(
        selected_group, ui.playlist_combobox->currentText());
    emit group_playlist_changed();
    this->close();
}

void Select_Playlist_Dialog::cancel_clicked()
{
    this->close();
}
