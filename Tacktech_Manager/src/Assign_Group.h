#ifndef ASSIGN_GROUP_H
#define ASSIGN_GROUP_H
#include <QtGui/QDialog>
#include <QtGui/QMessageBox>
#include <vector>
#include <Display_Client_Container.h>
#include "ui_Assign_Group.h"
#ifdef _SHOW_DEBUG_OUTPUT
#include <iostream>
#endif // _SHOW_DEBUG_OUTPUT
class Assign_Group : public QDialog
{
	Q_OBJECT
public:
	Assign_Group(Display_Client_Container_Ptr, std::vector<std::string>);
	~Assign_Group(void);
private:
	Ui::Dialog ui;
	Display_Client_Container_Ptr m_display_client_container;
	std::vector<std::string> m_selected_names;
private slots:
	void new_group_added();
	void ok_clicked();
	void cancel_clicked();
};
#endif //ASSIGN_GROUP_H
