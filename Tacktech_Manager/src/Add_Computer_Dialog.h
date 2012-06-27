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
	void set_computer_names(QList<QString>&);
private:
	Ui::Add_Computer_DialogClass ui;
	QList<QString> *computer_names;
private slots:
	void ok_clicked();
	void cancel_clicked();
};

#endif // ADD_COMPUTER_DIALOG_H
