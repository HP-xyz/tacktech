#ifndef SEND_DATA_H
#define SEND_DATA_H

#include <QThread.h>
#include <QMutex>
#include <QMutexLocker>
#include <QTcpSocket>
#include <string>
#include <QByteArray>
#include <QDataStream>
#include <QWaitCondition>
#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

class Send_Data : public QThread
{
    Q_OBJECT
    public:
        /** Default constructor */
        Send_Data();
        /** Default destructor */
        virtual ~Send_Data();

        void run();
        void send_data_to_server(const QString&, quint16, std::string&);
    private:
        QString host_name;
        quint16 port;
        QMutex mutex;
        QWaitCondition cond;
        bool quit;

        std::string xml_string;
    signals:
        void error(int, const QString&);
        void status_changed(const QString&);
};

#endif // SEND_DATA_H
