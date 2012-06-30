#ifndef TYPED_QTREEWIDGETITEM_H
#define TYPED_QTREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include <QString>
#include <QPair>
class Typed_QTreeWidgetItem : public QTreeWidgetItem
{
public:
	Typed_QTreeWidgetItem(QTreeWidgetItem *parent = 0);
	~Typed_QTreeWidgetItem();
	void set_type(QString);
	QString get_type();
	QPair<QTreeWidgetItem*, int> get_index_and_item_of_selected();
	void set_group_name(QString);
	void set_computer_name(QString);
	QString get_group_name();
	QString get_computer_name();
private:
	QString widget_type;
	QString computer_name;
	QString group_name;
};

#endif // TYPED_QTREEWIDGETITEM_H