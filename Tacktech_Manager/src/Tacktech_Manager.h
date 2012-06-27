#ifndef TACKTECH_MANAGER_H
#define TACKTECH_MANAGER_H

#include <QtGui/QMainWindow>
#include <iostream>
#include "Edit_Group.h"
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

	/* Global variable for Edit_Group GUI */
	Edit_Group *edit_group_class;

protected:
	/* Variable for group names */
	QList<QString> group_names;

	/* Variable for computer names
	*  Note: This can be hostnames or IP addresses */
	QList<QString> computer_names;

private slots:
	void refresh_all_groups();

	/* Slots that start GUIs (this)*/
	/* ===================== */
	void edit_group();
	void create_playlist();
	void edit_playlist();
	void edit_preferences();

	/* Slots that receive signals from GUIs (not this) */
	/* ==================================== */
	void add_group_input_complete(QString);
};

#endif // TACKTECH_MANAGER_H
