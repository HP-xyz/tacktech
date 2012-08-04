#include "NewsTicker.h"

class Widget : public QWidget
{
public:
	Widget(QString text, QWidget *parent = 0) : QWidget(parent)
	{
		label = new QLabel (text, this);
		label
		this->setFixedSize(1890, 50);
	}
	QLabel *label;
};


NewsTicker::NewsTicker (QWidget *parent) : QMainWindow(parent)
{
	this->setFixedSize(1900, 100);
	currentReply = 0;
	widget = new Widget ("", this);
	timer = new QTimer();
	connect(&manager, SIGNAL(finished(QNetworkReply*)),
		this, SLOT(finished()));
	connect(timer, SIGNAL(timeout()), this, SLOT(timeOut()));
	timer->start(20);
}

void NewsTicker::Fetch()
{
	xml.clear();
	get ();
}

void NewsTicker::readyRead()
{
	QByteArray data = currentReply->readAll();
	xml.addData(data);
	parseXml();
}

void NewsTicker::parseXml()
{
	while (!xml.atEnd()) 
	{
		xml.readNext();
		if (xml.isStartElement()) {
			if (xml.name() == "description")
				descriptionString = xml.attributes().value("rss:about").toString();
			currentTag = xml.name().toString();
		} 
		else if (xml.isEndElement()) 
		{
			if (xml.name() == "item") 
			{
				int i = 0;
				i = descriptionString.indexOf("<", i);
				descriptionString = descriptionString.remove(i, descriptionString.length());

				QString temp;
				temp = titleString;
				temp.append(" : ");
				temp.append(descriptionString);
				temp.append(" | ");
				*newsDescriptions += temp.toStdString();

				descriptionString.clear();
				titleString.clear();
			}

		} 
		else if (xml.isCharacters() && !xml.isWhitespace()) 
		{
			if (currentTag == "title")
				titleString += xml.text().toString();
			else if (currentTag == "description")
				descriptionString += xml.text().toString();
		}
	}
}

void NewsTicker::get()
{
	QNetworkRequest request(rssUrl);
	if (currentReply)
	{
		currentReply->disconnect();
		currentReply->deleteLater();
	}
	currentReply = manager.get(request);
	newsDescriptions = new std::string;
	connect(currentReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

void NewsTicker::finished()
{
	std::cout << std::endl;
	QString tmp = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t" + QString::fromStdString(*newsDescriptions);
//	std::cout << "this is the news" << std::endl << tmp.toStdString() << std::endl;
	

// 	widget = new Widget(this);
// 	widget->show();
// 	widget->setVisible(true);
	//widget->scroll(-200, 0);
	scrollLenght = tmp.size() + 625;
	scrollCounter = 0;
	widget->setVisible(false);
	widget = new Widget(tmp, this);
	widget->setVisible(true);
	
	emit done();
}

void NewsTicker::getNews(std::string* temp)
{
	get();
	newsDescriptions = temp;
}

void NewsTicker::timeOut()
{
	//if (scrollCounter++ <= scrollLenght)
		widget->scroll(-1, 0);
	//else
	//	finished();
}
