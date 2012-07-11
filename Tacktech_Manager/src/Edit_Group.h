#ifndef EDIT_GROUP_H
#define EDIT_GROUP_H

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

#include <QtGui/QMainWindow>
#include <QMenu>
#include <QAction>
#include "ui_Edit_Group.h"
#include "Typed_QTreeWidgetItem.h"
#include "Add_Computer_Dialog.h"
#include "Add_Group_Dialog.h"
#include "Group_Container.h"

class Edit_Group : public QMainWindow
{
    Q_OBJECT

public:
    Edit_Group(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~Edit_Group();
    void set_groups_and_computer_names(Group_Container*);
private:
    Ui::Edit_GroupClass *ui;

    QMenu *node_menu;

    QAction *add_computer;
    QAction *remove_computer;
    QAction *add_group;
    QAction *remove_group;

    /* Pointer to variable that have to be set by the calling class */
    Group_Container *groups_and_computer_names;

    /* Dialog variables */
    Add_Computer_Dialog *add_computer_dialog;
    Add_Group_Dialog *add_group_dialog;

private slots:
    /* Primary slots for this class */
    void add_computer_slot();
    void remove_computer_slot();
    void add_group_slot();
    void remove_group_slot();
    void ok_clicked();
    void cancel_clicked();

    /* Secondary slots that are fired from classes called from this class*/
    void repopulate_tree_widget();

signals:
    void groups_changed();
};

#endif // EDIT_GROUP_H
