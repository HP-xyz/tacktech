#ifndef _NEWS_TICKER_THREAD_H
#define _NEWS_TICKER_THREAD_H
#include <QThread>
#include <map>
#include <iostream>
#include "News_Ticker.h"
class News_Ticker_Thread : public QThread
{
	Q_OBJECT
public:
	News_Ticker_Thread(std::map<std::string, std::string>);
	~News_Ticker_Thread();
public slots:
    void news_recieved(QString);
private:
	void run();
	News_Ticker *news_ticker;
	std::map<std::string, std::string> parameters;
signals:
    void news_ready(QString);
};
#endif //_NEWS_TICKER_THREAD_H
