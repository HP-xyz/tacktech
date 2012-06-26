#ifndef ADD_GROUP_H
#define ADD_GROUP_H

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

#include <QtGui/QWidget>
#include "ui_Add_Group.h"


class Add_Group : public QWidget
{
	Q_OBJECT

public:
	Add_Group(QWidget *parent =0, Qt::WFlags flags = 0);
	~Add_Group();
private:
	Ui::Add_GroupClass ui;
private slots:
	void ok_clicked();
	void cancel_clicked();
signals:
	void add_group_input_complete(QString);

};

#endif // ADD_GROUP_H
