#include "Edit_Group.h"
/** Default constructor for the Edit_Group class */
Edit_Group::Edit_Group(QWidget *parent, Qt::WFlags flags) :
		QMainWindow(parent, flags)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Setting up Edit_Group GUI" << std::endl;
#endif // _DEBUG
	ui = new Ui::Edit_GroupClass();
	ui->setupUi(this);

	add_computer_dialog = new Add_Computer_Dialog();
	add_group_dialog = new Add_Group_Dialog();

	node_menu = new QMenu(ui->main_tree_widget);
	ui->main_tree_widget->setContextMenuPolicy(Qt::ActionsContextMenu);
	QStringList headers;
	headers << "Group View";
	ui->main_tree_widget->setHeaderLabels(headers);

	/* Creating context menu actions */
	add_computer = new QAction(tr("Add Computer To Group"), node_menu);
	remove_computer = new QAction(tr("Remove Computer From"), node_menu);
	add_group = new QAction(tr("Add Group"), node_menu);
	remove_group = new QAction(tr("Remove Group"), node_menu);

	/* Adding actions to context_menu */
	ui->main_tree_widget->addAction(add_computer);
	ui->main_tree_widget->addAction(remove_computer);
	ui->main_tree_widget->addAction(add_group);
	ui->main_tree_widget->addAction(remove_group);

	/* Connecting the main GUI signal for this class */
	connect(add_computer, SIGNAL(triggered()), this, SLOT(add_computer_slot()));
	connect(remove_computer, SIGNAL(triggered()), this,
			SLOT(remove_computer_slot()));
	connect(add_group, SIGNAL(triggered()), this, SLOT(add_group_slot()));
	connect(remove_group, SIGNAL(triggered()), this, SLOT(remove_group_slot()));
	connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(ok_clicked()));
	connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(cancel_clicked()));

	/* Connecting secondary signals that connect between this class and
	 * a called class */
	connect(add_group_dialog, SIGNAL(group_name_added()), this,
			SLOT(repopulate_tree_widget()));
	connect(add_computer_dialog, SIGNAL(computer_name_added()), this,
			SLOT(repopulate_tree_widget()));
}

Edit_Group::~Edit_Group()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= ~Edit_Group" << std::endl;
#endif // _DEBUG
	delete ui;
	delete add_computer;
	delete remove_computer;
	delete add_group;
	delete remove_group;
	delete node_menu;
	delete add_computer_dialog;
	delete add_group_dialog;
	//delete groups_and_computer_names;
}

/** Function to populate the global groups_and_computer_names variable with
 ** a pointer to the calling class' groups_and_computer_names variable */
void Edit_Group::set_groups_and_computer_names(
		Group_Container_Ptr p_groups_and_computer_names)
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= set_groups_and_computer_names()" << std::endl;
#endif // _DEBUG
	groups_and_computer_names.reset(
			new Group_Container(*p_groups_and_computer_names));
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " - Populate tree_widget" << std::endl;
#endif // _DEBUG
	/* Populating the main_tree_widget */
	repopulate_tree_widget();
}

/** Slot to start the add_computer GUI and show the GUI. Check to see
 ** if the selected item is a group node. If it is not, displays info
 ** message*/
void Edit_Group::add_computer_slot()
{
	if (ui->main_tree_widget->selectedItems().count() > 0)
	{
		if (static_cast<Typed_QTreeWidgetItem*>(ui->main_tree_widget->selectedItems().at(
				0))->get_type() == "GROUP")
		{
			add_computer_dialog->set_groups_and_computer_names(
					groups_and_computer_names);
			add_computer_dialog->set_group_name(
					ui->main_tree_widget->selectedItems().at(0)->text(0));
			add_computer_dialog->show();
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setText("Select a group to add a computer to");
			msgBox.exec();
		}
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText("Add/Select a group first");
		msgBox.exec();
	}
}

/** Will be fired on the remove_computer's triggered signal, and will
 ** remove a computer from the treewidget if it is selected. If a computer
 ** is not selected, an info message will be displayed */
void Edit_Group::remove_computer_slot()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= remove_computer_slot()" << std::endl;
#endif // _DEBUG
	if (ui->main_tree_widget->selectedItems().count() > 0)
	{
		if (static_cast<Typed_QTreeWidgetItem*>(ui->main_tree_widget->selectedItems().at(
				0))->get_type() == "COMPUTER")
		{
			/* Deleting from the variable */
			groups_and_computer_names->remove_computer_name(
					static_cast<Typed_QTreeWidgetItem*>(ui->main_tree_widget->selectedItems().at(
							0))->get_group_name().toStdString(),
					static_cast<Typed_QTreeWidgetItem*>(ui->main_tree_widget->selectedItems().at(
							0))->get_computer_name().toStdString());

			/* Deleting from the UI */
			QPair<QTreeWidgetItem*, int> pair_item;
			pair_item =
					static_cast<Typed_QTreeWidgetItem*>(ui->main_tree_widget->selectedItems().at(
							0))->get_index_and_item_of_selected();
			ui->main_tree_widget->removeItemWidget(
					pair_item.first->child(pair_item.second), 0);
			delete pair_item.first->child(pair_item.second);
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setText("Select a group to remove a computer from");
			msgBox.exec();
		}
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText("Add/Select a group first");
		msgBox.exec();
	}
}

/** Slot to start the add_group GUI and show the GUI. */
void Edit_Group::add_group_slot()
{
	add_group_dialog->set_group_and_computer_names(groups_and_computer_names);
	add_group_dialog->show();
}

/** Slot removes a group from the widget and the groups_and_computers
 ** variable */
void Edit_Group::remove_group_slot()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= remove_group_slot()" << std::endl;
#endif // _DEBUG
	if (ui->main_tree_widget->selectedItems().count() > 0)
	{
		if (static_cast<Typed_QTreeWidgetItem*>(ui->main_tree_widget->selectedItems().at(
				0))->get_type() == "GROUP")
		{
			groups_and_computer_names->remove_group_name(
					static_cast<Typed_QTreeWidgetItem*>(ui->main_tree_widget->selectedItems().at(
							0))->get_group_name().toStdString());
			ui->main_tree_widget->removeItemWidget(
					ui->main_tree_widget->selectedItems().at(0), 0);
			delete ui->main_tree_widget->selectedItems().at(0);
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setText("Select a group to remove");
			msgBox.exec();
		}
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText("Add/Select a group first");
		msgBox.exec();
	}
}

/** Slot gets fired when the editing of either the group or computer dialog
 ** sends its _added() signal */
void Edit_Group::repopulate_tree_widget()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << " = repopulate_tree_widget" << std::endl;
	std::cout << "  - Clearing the main_tree_widget" << std::endl;
#endif // _DEBUG
	ui->main_tree_widget->clear();
	Typed_QTreeWidgetItem *group_item;
	Typed_QTreeWidgetItem *computer_item;
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "  - Beginning population" << std::endl;
#endif // _DEBUG
	for (Group_Multimap::iterator it =
			groups_and_computer_names->get_groups_and_computers()->begin();
			it != groups_and_computer_names->get_groups_and_computers()->end();
			it++)
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << "   - Group Name: " << it->first << std::endl;
#endif // _DEBUG
		group_item = new Typed_QTreeWidgetItem();
		group_item->set_group_name(QString::fromStdString(it->first));
		group_item->set_type("GROUP");
		group_item->setText(0, QString::fromStdString(it->first));
		ui->main_tree_widget->addTopLevelItem(group_item);
		Group_Computer_Range range =
				groups_and_computer_names->get_groups_and_computers()->equal_range(
						it->first);
		Group_Multimap::iterator it2 = range.first;
		for (it2; it2 != range.second; ++it2)
		{
#ifdef _SHOW_DEBUG_OUTPUT
			std::cout << "    - Computer Name: " << it2->second.first << std::endl;
#endif // _DEBUG
			computer_item = new Typed_QTreeWidgetItem();
			computer_item->set_computer_name(
					QString::fromStdString(it2->second.first));
			computer_item->set_group_name(QString::fromStdString(it2->first));
			computer_item->set_type("COMPUTER");
			computer_item->setText(0, QString::fromStdString(it2->second.first));
			group_item->addChild(computer_item);
		}
	}
	Group_Multimap no_group_map =
		groups_and_computer_names->get_computers_not_in_groups();
	for (Group_Multimap::iterator it = no_group_map.end();
		it != no_group_map.end();
		it++)
	{
		computer_item = new Typed_QTreeWidgetItem();
		computer_item->set_computer_name(
			QString::fromStdString(it->second.first));
		computer_item->set_group_name(QString::fromStdString(it->first));
		computer_item->set_type("COMPUTER");
		computer_item->setText(0, QString::fromStdString(it->second.first));
		ui->all_computers_tree_widget->addTopLevelItem(computer_item);
	}
}

/** Slot gets fired if the accept signal is received from the class
 ** buttonbox. Emits both the group_editing_complete and
 ** computer_editing_complete signals */
void Edit_Group::ok_clicked()
{
	emit groups_changed();
	this->close();
}

/** Slot gets fired if the rejected signal is received from the class
 ** buttonbox */
void Edit_Group::cancel_clicked()
{
	this->close();
}

void Edit_Group::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Enter:
	{
		ok_clicked();
		break;
	}
	case Qt::Key_Return:
	{
		ok_clicked();
		break;
	}
	default:
		QWidget::keyPressEvent(event);
	}
}
