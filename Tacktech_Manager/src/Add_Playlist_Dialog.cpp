#include "Add_Playlist_Dialog.h"

Add_Playlist_Dialog::Add_Playlist_Dialog(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    ui.setupUi(this);
    connect(ui.buttonBox, SIGNAL(accepted()),
            this, SLOT(ok_clicked()));
    connect(ui.buttonBox, SIGNAL(rejected()),
            this, SLOT(cancel_clicked()));
}

Add_Playlist_Dialog::~Add_Playlist_Dialog()
{

}

void Add_Playlist_Dialog::set_playlist( Playlist_Container *p_playlist )
{
    playlist = p_playlist;
}

void Add_Playlist_Dialog::ok_clicked()
{
    if(playlist->add_playlist_name(ui.playlist_name_line_edit->text()))
    {
        emit playlist_name_added();
        this->close();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Playlist name already exists");
        msgBox.exec();
    }
}

void Add_Playlist_Dialog::cancel_clicked()
{
    this->close();
}

void Add_Playlist_Dialog::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Enter:
		{  
			ok_clicked();
			break;
		}
	case Qt::Key_Return:
		{  
			ok_clicked();
			break;
		}
	default:
		QWidget::keyPressEvent(event);
	}
}
