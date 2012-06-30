#ifndef ADD_COMPUTER_DIALOG_H
#define ADD_COMPUTER_DIALOG_H

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

#include <QtGui/QMainWindow>
#include <QMessageBox>
#include "ui_Add_Computer_Dialog.h"

class Add_Computer_Dialog : public QMainWindow
{
	Q_OBJECT

public:
	Add_Computer_Dialog(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Add_Computer_Dialog();
	void set_groups_and_computer_names(QMap<QString, QList<QString>* >*);
	void set_group_index(int);
private:
	Ui::Add_Computer_DialogClass ui;
	QMap<QString, QList<QString>* > *groups_and_computer_names;
	int group_index;
private slots:
	void ok_clicked();
	void cancel_clicked();
signals:
	void computer_name_added();
};

#endif // ADD_COMPUTER_DIALOG_H
