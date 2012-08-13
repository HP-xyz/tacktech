#include "src/Tacktech_Manager.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Tacktech_Manager w;
	w.show();
	return a.exec();
}
