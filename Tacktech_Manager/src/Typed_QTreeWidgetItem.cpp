#include "Typed_QTreeWidgetItem.h"

Typed_QTreeWidgetItem::Typed_QTreeWidgetItem(QTreeWidgetItem *parent) 
{
	
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