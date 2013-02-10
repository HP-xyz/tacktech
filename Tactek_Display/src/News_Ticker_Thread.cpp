#include "News_Ticker_Thread.h"
News_Ticker_Thread::News_Ticker_Thread(std::map<std::string, std::string> p_parameters)
{
    parameters = p_parameters;
}
News_Ticker_Thread::~News_Ticker_Thread()
{
#ifdef _SHOW_DEBUG_OUTPUT
    std::cout << "~News_Ticker_Thread" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
    delete news_ticker;
}
void News_Ticker_Thread::run()
{
#ifdef _SHOW_DEBUG_OUTPUT
    std::cout << "=News_Ticker_Thread::run" << std::endl;
#endif // _SHOW_DEBUG_OUTPUT
    news_ticker = new News_Ticker(parameters);
    news_ticker->set_screen_length(40);
    connect(news_ticker, SIGNAL(news_ready(QString)), this, SLOT(news_recieved(QString)));
    exec();
}

void News_Ticker_Thread::news_recieved(QString news)
{
    emit news_ready(news);
}
