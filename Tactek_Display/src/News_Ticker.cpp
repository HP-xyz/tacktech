#include "News_Ticker.h"
News_Ticker::News_Ticker(std::map<std::string, std::string> p_parameters)
{
    parameters = p_parameters;
    m_rss_url = "http://24.com.feedsportal.com/c/33816/f/607927/index.rss";
    m_news_current_reply = 0;
    current_news_segment = 0;

    #ifdef _SHOW_DEBUG_OUTPUT
    std::cout << " =News_Ticker::run" << std::endl;
    std::cout << "  - News should display" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
    get_news_from_network();
    emit_display_news_signal();

#ifdef _SHOW_DEBUG_OUTPUT
    std::cout << "  - Creating timers" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
    news_ticker_display_timer = new QTimer();
    news_refresh_timer = new QTimer();
    connect(news_ticker_display_timer, SIGNAL(timeout()), this, SLOT(emit_display_news_signal()));
    connect(news_refresh_timer, SIGNAL(timeout()), this, SLOT(get_news_from_network()));

#ifdef _SHOW_DEBUG_OUTPUT
    std::cout << "  - Starting timers" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
    news_refresh_timer->start(108000); //30 Minutes
    news_ticker_display_timer->start(boost::lexical_cast<int, std::string>(parameters["news.scroll_speed"]));
}

News_Ticker::~News_Ticker()
{
#ifdef _SHOW_DEBUG_OUTPUT
    std::cout << "~News_Ticker_Thread" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
    if (news_ticker_display_timer != 0)
        delete news_ticker_display_timer;
    if(news_refresh_timer != 0)
        delete news_refresh_timer;
}

void News_Ticker::set_screen_length(int length)
{
    screen_length = length;
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Setting Screen_lenght" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
    screen_length = boost::lexical_cast<int, std::string>(parameters["news.news_length"]);
#ifdef _SHOW_DEBUG_OUTPUT
    std::cout << " - Screen length: " << screen_length << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
}

void News_Ticker::get_news_from_network()
{
    QNetworkRequest request(m_rss_url);
    if(m_news_current_reply != 0)
    {
        try
        {
            m_news_current_reply->disconnect();
            m_news_current_reply->deleteLater();
        }
        catch(std::exception &e)
        {
            std::cerr << "Critical error getting news: " << e.what() << std::endl;
        }
    }
    m_news_current_reply = m_news_networkmanager.get(request);
    m_news_descriptions.reset(new QString());
    connect(m_news_current_reply, SIGNAL(readyRead()), this, SLOT(read_news_from_network()));
}

void News_Ticker::read_news_from_network()
{
    QByteArray data = m_news_current_reply->readAll();
    m_news_xml.addData(data);
    parse_xml_for_news();
}
void News_Ticker::parse_xml_for_news()
{
    QString description_str;
    QString current_tag;
    QString title_str;
    while (!m_news_xml.atEnd())
	{
		m_news_xml.readNext();
		if (m_news_xml.isStartElement()) {
			if (m_news_xml.name() == "description")
				description_str = m_news_xml.attributes().value("rss:about").toString();
			current_tag = m_news_xml.name().toString();
		}
		else if (m_news_xml.isEndElement())
		{
			if (m_news_xml.name() == "item")
			{
				int i = 0;
				i = description_str.indexOf("<", i);
				description_str = description_str.remove(i, description_str.length());

				QString temp;
				temp = title_str;
				temp.append(" : ");
				temp.append(description_str);
				temp.append(" | ");
				*m_news_descriptions += temp;

				description_str.clear();
				title_str.clear();
			}

		}
		else if (m_news_xml.isCharacters() && !m_news_xml.isWhitespace())
		{
			if (current_tag == "title")
				title_str += m_news_xml.text().toString();
			else if (current_tag == "description")
				description_str += m_news_xml.text().toString();
		}
	}
}

void News_Ticker::emit_display_news_signal()
{
    emit news_ready(get_news_segment());
}

QString News_Ticker::get_news_segment()
{
    if (m_news_descriptions->size() > 0)
    {
        if ((current_news_segment + screen_length) < m_news_descriptions->size())
        {
            if((current_news_segment + screen_length + 1) >= m_news_descriptions->size())
            {
                current_news_segment = 0;
//                std::string text_left = m_news_descriptions->substr(0, (m_news_descriptions->size() - (current_news_segment + screen_length)));
//                std::string text_right = m_news_descriptions->substr(current_news_segment);
//                current_news_segment += 1;
#ifdef _SHOW_DEBUG_OUTPUT
                std::cout << "News (Looping): " << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
//                return text_left + text_right;
            }
            else
            {
                int segment_to_return = current_news_segment;
                current_news_segment = current_news_segment + 1;
#ifdef _SHOW_DEBUG_OUTPUT
                //std::cout << "News (Normal): " << m_news_descriptions->substr(segment_to_return + 1, screen_length) << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
                return m_news_descriptions->mid(segment_to_return + 1, screen_length);
            }
        }
        else
            return *m_news_descriptions;
    }
    else
        return "";
}
