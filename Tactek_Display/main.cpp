#include "src/Tactek_Display.h"
#include "src/NewsTicker.h"
#include <QtGui/QApplication>
#include <iostream>
int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("Tackteck Advertising");
    QCoreApplication::setAttribute(Qt::AA_X11InitThreads);
	QApplication a(argc, argv);
	Tactek_Display w;
	w.showFullScreen();
	return a.exec();
}
