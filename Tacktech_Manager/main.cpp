#include "src/Tacktech_Manager_MainWindow.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Tacktech_Manager_MainWindow w;
	w.show();
	return a.exec();
}
