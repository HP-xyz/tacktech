#include "Edit_Group.h"
/** Default constructor for the Edit_Group class */
Edit_Group::Edit_Group(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
#ifdef _DEBUG
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
	connect(add_computer, SIGNAL(triggered()),
		this, SLOT(add_computer_slot()));
	connect(remove_computer, SIGNAL(triggered()),
		this, SLOT(remove_computer_slot()));
	connect(add_group, SIGNAL(triggered()),
		this, SLOT(add_group_slot()));
	connect(remove_group, SIGNAL(triggered()),
		this, SLOT(remove_group_slot()));
	connect(ui->buttonBox, SIGNAL(accepted()),
		this, SLOT(ok_clicked()));
	connect(ui->buttonBox, SIGNAL(rejected()),
		this, SLOT(cancel_clicked()));

	/* Connecting secondary signals that connect between this class and
	 * a called class */
	connect(add_group_dialog, SIGNAL(group_name_added(QString)),
		this, SLOT(group_name_added(QString)));
	connect(add_computer_dialog, SIGNAL(computer_name_added(QString)),
		this, SLOT(computer_name_added(QString)));
}

Edit_Group::~Edit_Group()
{
	delete ui;
	delete group_names;
	delete computer_names;
	delete node_menu;
	delete add_computer;
	delete remove_computer;
	delete add_group;
	delete remove_group;
	delete add_computer_dialog;
	delete add_group_dialog;
}

/** Function to populate the global group_names variable with a pointer to
 ** the calling class' group_names variable */
void Edit_Group::set_group_names( QList<QString>& p_group_names)
{
	group_names = &p_group_names;
}

/** Function to populate the global computer_names variable with a pointer
 ** to the calling class' computer_names variable */
void Edit_Group::set_computer_names( QList<QString>& p_computer_names)
{
	computer_names = &p_computer_names;
}

/** Slot to start the add_computer GUI and show the GUI. Check to see
 ** if the selected item is a group node. If it is not, displays info
 ** message*/
void Edit_Group::add_computer_slot()
{
	if (ui->main_tree_widget->selectedItems().count() > 0)
	{
		if (static_cast<Typed_QTreeWidgetItem*>(ui->main_tree_widget->
			selectedItems().at(0))->get_type() == "GROUP")
		{
			add_computer_dialog->set_computer_names(*computer_names);
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
	if (ui->main_tree_widget->selectedItems().count() > 0)
	{
		if (static_cast<Typed_QTreeWidgetItem*>(ui->main_tree_widget->
			selectedItems().at(0))->get_type() == "COMPUTER")
		{
#ifdef _DEBUG
			int index = ui->main_tree_widget->selectedIndexes().at(0).child(0,0);
			std::cout << "Index of selected item: " 
				<< index;
#endif // _DEBUG
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

void Edit_Group::add_group_slot()
{
	add_group_dialog->show();
	add_group_dialog->set_group_names(*group_names);
}

void Edit_Group::remove_group_slot()
{

}
 /** Slot fired when the group_names list changes in value */
void Edit_Group::group_name_added(QString new_group_name)
{
	Typed_QTreeWidgetItem *item = new Typed_QTreeWidgetItem();
	item->set_type("GROUP");
	item->setText(0, new_group_name);
	ui->main_tree_widget->addTopLevelItem(item);
}
/** Slot fired when the computer_names list changes in value */
void Edit_Group::computer_name_added( QString new_computer_name)
{
	Typed_QTreeWidgetItem *item = new Typed_QTreeWidgetItem();
	item->set_type("COMPUTER");
	item->setText(0, new_computer_name);
	ui->main_tree_widget->selectedItems().at(0)->addChild(item);
}

/** Slot gets fired if the accept signal is received from the class 
 ** buttonbox. Emits both the group_editing_complete and
 ** computer_editing_complete signals */
void Edit_Group::ok_clicked()
{
	emit group_editing_complete(*group_names);
	emit computer_editing_complete(*computer_names);
	this->close();
}

/** Slot gets fired if the rejected signal is received from the class
 ** buttonbox */
void Edit_Group::cancel_clicked()
{
	this->close();
}
