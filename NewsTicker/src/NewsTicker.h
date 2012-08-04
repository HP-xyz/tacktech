#ifndef NEWSTICKER_H
#define NEWSTICKER_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMainWindow>
#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QUrl>
#include <QBuffer>
#include <QXmlStreamReader>
#include <iostream>

class Widget;

class NewsTicker : public QMainWindow
{
Q_OBJECT
public:
	NewsTicker (QWidget *parent = 0);
	void setUrl(QString url) {rssUrl = url;}
	void getNews(std::string * temp);

signals:
		void done();

public slots:
	void Fetch();
	void readyRead();
	void finished();
	void timeOut();

private:
	void parseXml();
	void get();

	int scrollLenght;
	int scrollCounter;
	QTimer *timer;
	QLabel *textLabel;
	QUrl rssUrl;
	QXmlStreamReader xml;
	QString currentTag;
	QString descriptionString;
	QString temp;
	QString titleString;
	std::string *newsDescriptions;

	QNetworkAccessManager manager;
	QNetworkReply *currentReply;

	Widget *widget;

};
#endif