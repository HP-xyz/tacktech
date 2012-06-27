#ifndef EDIT_GROUP_H
#define EDIT_GROUP_H

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

#include <QtGui/QMainWindow>
#include <QMenu>
#include <QAction>
#include "ui_Edit_Group.h"

class Edit_Group : public QMainWindow
{
	Q_OBJECT

public:
	Edit_Group(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Edit_Group();
	void set_group_names(QList<QString>&);
private:
	Ui::Edit_GroupClass *ui;
	QMenu *node_menu;
	QAction *insert_child;

	QList<QString> *group_names;
};

#endif // EDIT_GROUP_H
