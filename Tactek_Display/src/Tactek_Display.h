/****************************************************************************
* VLC-Qt CMake Demo
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef TACTEK_DISPLAY_H
#define TACTEK_DISPLAY_H

#include <QtGui/QMainWindow>
#include <QtGui/QFileDialog>
#include <QTimer>
#include <QPair>
#include <vlc-qt/Common.h>
#include <vlc-qt/Instance.h>
#include <vlc-qt/Media.h>
#include <vlc-qt/MediaPlayer.h>
#include <iostream>
#include "Playlist.h"
#include "ui_Tactek_Display.h"
#include "Recieve_Data.h"
namespace Ui
{
class Tactek_Display;
}

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

class Tactek_Display : public QMainWindow
{
    Q_OBJECT
public:
    explicit Tactek_Display(QWidget *parent = 0);
    ~Tactek_Display();
signals:
    void start_next_media();
private slots:
    void check_media_state();
    void play_next_media_in_queue();
    void open_and_play(QString);
private:
    Ui::Tactek_Display *ui;
    Playlist *playlist;
    QTimer *update_timer;
    VlcInstance *m_vlc_instance;
    VlcMedia *m_vlc_media;
    VlcMediaPlayer *m_vlc_player;
//    Receive_Data *server;
};

#endif // TACTEK_DISPLAY_H
