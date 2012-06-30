#ifndef TACKTECH_MANAGER_H
#define TACKTECH_MANAGER_H

#include <QtGui/QMainWindow>
#include <QMap>
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
	/* Variable for computer names and group names
	 * Note: Format is groups_and_computers[group_index][computer_index]
	 * Note: This can be hostnames or IP addresses */
	QMap<QString, QList<QString>* > *groups_and_computers;

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
	void group_editing_complete(QList<QString>);
	void computer_editing_complete(QList<QString>);
};

#endif // TACKTECH_MANAGER_H
