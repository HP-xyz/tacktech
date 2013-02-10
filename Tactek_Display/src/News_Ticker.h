#ifndef NEWS_TICKER_H
#define NEWS_TICKER_H
#include <QObject>
#include <QUrl>
#include <QXmlStreamReader>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include <map>
class News_Ticker : public QObject
{
	Q_OBJECT
public:
	News_Ticker(std::map<std::string, std::string>);
	~News_Ticker();
	void set_screen_length(int);
public slots:
    void get_news_from_network();
    void read_news_from_network();
private slots:
    void emit_display_news_signal();
private:

	QTimer *news_ticker_display_timer;
	QTimer *news_refresh_timer;

	/** Variables for the config file */
	std::map<std::string, std::string> parameters;

	boost::shared_ptr<QString> m_news_descriptions;
	QUrl m_rss_url;
	QXmlStreamReader m_news_xml;
	QNetworkAccessManager m_news_networkmanager;
	QNetworkReply *m_news_current_reply;

    int screen_length;
    int current_news_segment;
	void parse_xml_for_news();
	QString get_news_segment();
signals:
    void news_ready(QString);
};
#endif // NEWS_TICKER_H
