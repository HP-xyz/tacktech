#ifndef EDIT_GROUP_H
#define EDIT_GROUP_H

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

#include <QtGui/QMainWindow>
#include <QMenu>
#include <QAction>
#include "ui_Edit_Group.h"
#include "Add_Computer_Dialog.h"
#include "Add_Group_Dialog.h"

class Edit_Group : public QMainWindow
{
	Q_OBJECT

public:
	Edit_Group(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Edit_Group();
	void set_group_names(QList<QString>&);
	void set_computer_names(QList<QString>&);
private:
	Ui::Edit_GroupClass *ui;

	QMenu *node_menu;

	QAction *add_computer;
	QAction *remove_computer;
	QAction *add_group;
	QAction *remove_group;

	/* Pointers to variables that have to be set by the calling class */
	QList<QString> *group_names;
	QList<QString> *computer_names;

	/* Dialog variables */
	Add_Computer_Dialog *add_computer_dialog;
	Add_Group_Dialog *add_group_dialog;

private slots:
	void add_computer_slot();
	void remove_computer_slot();
	void add_group_slot();
	void remove_group_slot();
};

#endif // EDIT_GROUP_H
