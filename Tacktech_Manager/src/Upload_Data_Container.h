#ifndef UPLOAD_DATA_CONTAINER_H
#define UPLOAD_DATA_CONTAINER_H


#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG
#include <QString>
#include <QObject>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <pugixml.hpp>
#include <boost/lexical_cast.hpp>
#include <b64/encode.h>
#include "Playlist_Container.h"
#include "Group_Container.h"

class Upload_Data_Container : public QObject
{
	Q_OBJECT
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
    void get_xml_upload();
protected:
private:
	char *get_binary_file(QString);
    Playlist_Container *playlist;
    Group_Container *groups;
    QString group_name;
    QString playlist_name;
    QString upload_time;
    std::ifstream::pos_type m_size;
signals:
	void xml_creation_complete(std::string);
};

#endif // UPLOAD_DATA_CONTAINER_H
