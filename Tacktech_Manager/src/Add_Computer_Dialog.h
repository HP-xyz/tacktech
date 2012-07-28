#ifndef ADD_COMPUTER_DIALOG_H
#define ADD_COMPUTER_DIALOG_H

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

#include <QtGui/QMainWindow>
#include <QMessageBox>
#include <QKeyEvent>
#include "ui_Add_Computer_Dialog.h"
#include <Group_Container.h>

class Add_Computer_Dialog : public QMainWindow
{
    Q_OBJECT

public:
    Add_Computer_Dialog(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~Add_Computer_Dialog();
    void set_groups_and_computer_names(Group_Container_Ptr);
    void set_group_name(QString);
private:
    Ui::Add_Computer_DialogClass ui;
    Group_Container_Ptr groups_and_computer_names;
    QString group_name;

	void keyPressEvent(QKeyEvent *event);
private slots:
    void ok_clicked();
    void cancel_clicked();
signals:
    void computer_name_added();
};

#endif // ADD_COMPUTER_DIALOG_H
