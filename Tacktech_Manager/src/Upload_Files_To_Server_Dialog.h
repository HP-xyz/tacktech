#ifndef UPLOAD_FILES_TO_SERVER_DIALOG_H
#define UPLOAD_FILES_TO_SERVER_DIALOG_H
#include <QtGui/QWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <algorithm>
#include "ui_Upload_Files_To_Server_Dialog.h"
#ifdef _SHOW_DEBUG_OUTPUT
#include <iostream>
#endif // _SHOW_DEBUG_OUTPUT
class Upload_Files_To_Server_Dialog : QWidget
{
	Q_OBJECT
public:
	Upload_Files_To_Server_Dialog(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Upload_Files_To_Server_Dialog(void);
private:
	Ui::Uplaod_Files_Dialog ui;
	std::vector<std::string> selected_files;
	QTime upload_time;
private slots:
	void add_files();
	void ok_clicked();
	void cancel_clicked();
	void repopulate_widget();
signals:
	void selection_complete(std::vector<std::string>, QTime);
};
#endif //UPLOAD_FILES_TO_SERVER_DIALOG_H