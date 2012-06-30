#ifndef ADD_GROUP_DIALOG_H
#define ADD_GROUP_DIALOG_H

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

#include <QtGui/QMainWindow>
#include <QMessageBox>
#include "ui_Add_Group_Dialog.h"

class Add_Group_Dialog : public QMainWindow
{
	Q_OBJECT

public:
	Add_Group_Dialog(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Add_Group_Dialog();
	void set_group_and_computer_names(QMap<QString, QList<QString>* >*);
private:
	Ui::Add_Group_DialogClass ui;
	QMap<QString, QList<QString>* > *group_and_computer_names;
private slots:
	void ok_clicked();
	void cancle_clicked();
signals:
	void group_name_added();
};

#endif // ADD_GROUP_DIALOG_H
