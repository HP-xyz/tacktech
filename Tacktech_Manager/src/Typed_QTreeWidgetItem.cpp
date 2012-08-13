#include "Typed_QTreeWidgetItem.h"

Typed_QTreeWidgetItem::Typed_QTreeWidgetItem(QTreeWidgetItem *parent)
{
	group_name = "";
	computer_name = "";
}

Typed_QTreeWidgetItem::~Typed_QTreeWidgetItem(void)
{

}

void Typed_QTreeWidgetItem::set_type(QString type)
{
	widget_type = type;
}

QString Typed_QTreeWidgetItem::get_type()
{
	return widget_type;
}

QPair<QTreeWidgetItem*, int> Typed_QTreeWidgetItem::get_index_and_item_of_selected()
{

	QTreeWidgetItem *parent_item = this->parent();
	int index = parent_item->indexOfChild(this);
	QPair<QTreeWidgetItem*, int> pair_item;
	pair_item.first = parent_item;
	pair_item.second = index;
	return pair_item;
}

void Typed_QTreeWidgetItem::set_group_name(QString p_group_name)
{
	group_name = p_group_name;
}

void Typed_QTreeWidgetItem::set_computer_name(QString p_computer_name)
{
	computer_name = p_computer_name;
}

QString Typed_QTreeWidgetItem::get_group_name()
{
	return group_name;
}

QString Typed_QTreeWidgetItem::get_computer_name()
{
	return computer_name;
}

void Typed_QTreeWidgetItem::set_playlist_name(QString p_playlist_name)
{
	playlist_name = p_playlist_name;
}

void Typed_QTreeWidgetItem::set_filename(QString p_filename)
{
	filename = p_filename;
}

void Typed_QTreeWidgetItem::set_pause(int p_pause)
{
	pause = p_pause;
}

QString Typed_QTreeWidgetItem::get_playlist_name()
{
	return playlist_name;
}

QString Typed_QTreeWidgetItem::get_filename()
{
	return filename;
}

int Typed_QTreeWidgetItem::get_pause()
{
	return pause;
}
