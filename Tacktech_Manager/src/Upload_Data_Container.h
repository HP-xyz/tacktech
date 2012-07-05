#ifndef UPLOAD_DATA_CONTAINER_H
#define UPLOAD_DATA_CONTAINER_H

#include <QString>
#include <fstream>
#include <iostream>
#include <pugixml.hpp>
#include <boost/lexical_cast.hpp>
#include "Playlist_Container.h"
#include "Group_Container.h"

class Upload_Data_Container
{
public:
    /** Default constructor */
    Upload_Data_Container();
    /** Default destructor */
    virtual ~Upload_Data_Container();
    void set_playlist(Playlist_Container*);
    void set_groups(Group_Container*);
    void set_playlist_name(QString);
    void set_group_name(QString);
    void set_upload_time(QString);
    std::string get_xml_upload();      
protected:
private:
	char* get_binary_file(QString);
    Playlist_Container *playlist;
    Group_Container *groups;
    QString group_name;
    QString playlist_name;
    QString upload_time;
    std::ifstream::pos_type m_size;
};

#endif // UPLOAD_DATA_CONTAINER_H
