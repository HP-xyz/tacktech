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
#include "Tactek_Display.h"

/**
 * Initiates and sets up the class for usage. Connects the check_media_state and
 * play_next_media_in_queue signals to their respective slots.
 * @param parent
 */
Tactek_Display::Tactek_Display(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::Tactek_Display),
    m_vlc_media(0)
{
    ui->setupUi(this);

    Tactek_Display::update_timer = new QTimer(this);
    Tactek_Display::playlist = new Playlist();

    m_vlc_instance = new VlcInstance(VlcCommon::args(), this);
    m_vlc_player = new VlcMediaPlayer(m_vlc_instance);
    m_vlc_player->setVideoWidget(ui->vlc_video_widget);

    ui->vlc_video_widget->setMediaPlayer(m_vlc_player);

    connect(update_timer, SIGNAL(timeout()),
            this, SLOT(check_media_state()));
    connect(this, SIGNAL(start_next_media()),
            this, SLOT(play_next_media_in_queue()));

    update_timer->start(1000);
}

Tactek_Display::~Tactek_Display()
{
    delete ui;
    delete m_vlc_media;
    delete m_vlc_player;
    delete m_vlc_instance;
}

/**
 * Opens and plays the file specified in the filepath parameter.
 * @param filepath Path to the file that needs to be played
 */
void Tactek_Display::open_and_play(QString filepath)
{
    m_vlc_media = new VlcMedia(filepath, true, m_vlc_instance);
    m_vlc_player->open(m_vlc_media);
}

/**
 * Checks the currents media state and fires a playback_ended signal if the
 * current m_vlc_player state = 0 (IDLE)
 */
void Tactek_Display::check_media_state()
{
    std::cout << "Current media state: " <<
              Tactek_Display::m_vlc_player->state() << std::endl;
    if (Tactek_Display::m_vlc_player->state() == 0 || Tactek_Display::m_vlc_player->state() == 6)
        emit Tactek_Display::start_next_media();
}

/**
 * Slot to play the next media in the playlist. Gets called by the
 * start_next_media() signal. Will pause the output for the specified number
 * of seconds in the playlist, if the pause is not equal to 0. Does nothing
 * when there is no media in the playlist.
 */
void Tactek_Display::play_next_media_in_queue()
{
    if (playlist->get_playlist().size() > 0)
    {
        if (playlist->current_index_of_queue == playlist->get_playlist().size())
            playlist->current_index_of_queue = 0;
        QPair<QString, int> queue_item =
            playlist->get_playlist()
            [playlist->current_index_of_queue];
        if(queue_item.second == 0)
        {
            open_and_play(queue_item.first);
        }
        else
        {
            std::cout << "Pausing for: " << queue_item.second * 1000 << std::endl;
            QTimer::singleShot((queue_item.second * 1000), this,
                               NULL);
            open_and_play(queue_item.first);
        }
        playlist->current_index_of_queue += 1;
    }
    else
    {
        std::cout << "There is no media in the playlist" << std::endl;
    }
}
