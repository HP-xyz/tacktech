#ifndef TACKTECH_MANAGER_H
#define TACKTECH_MANAGER_H

#include <QtGui/QMainWindow>
#include <iostream>
#include "Add_Group.h"
#include "ui_Tacktech_Manager.h"

class Tacktech_Manager : public QMainWindow
{
	Q_OBJECT

public:
	Tacktech_Manager(QWidget *parent = 0, Qt::WFlags flags = 0);
	~Tacktech_Manager();

private:
	/* The global ui variable */
	Ui::MainWindow ui;

	/* Global variable for the Add_Group class */
	Add_Group *add_group_window;

protected:
	/* Variable for group names */
	QList<QString> group_names;

private slots:
	void refresh_all_groups();

	/* Slots that start GUIs (this)*/
	/* ===================== */
	void add_group();
	void edit_group();
	void create_playlist();
	void edit_playlist();
	void edit_preferences();

	/* Slots that recieve signals from GUIs (not this) */
	/* ==================================== */
	void add_group_input_complete(QString);
};

#endif // TACKTECH_MANAGER_H
