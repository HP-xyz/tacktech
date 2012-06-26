#include "src/Tactek_Display.h"
#include <QtGui/QApplication>
#include <iostream>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Tactek_Display w;
	w.showFullScreen();
        return a.exec();
}
