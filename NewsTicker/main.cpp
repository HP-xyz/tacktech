#include "src/NewsTicker.h"
#include <QtGui/QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NewsTicker n;
	n.setUrl("http://24.com.feedsportal.com/c/33816/f/607927/index.rss");
	std::string *news = new std::string;
	n.getNews(news);
	n.show();
    return a.exec();
}

