#ifndef UPLOAD_DATA_H
#define UPLOAD_DATA_H

#include <QWidget>
#include <QDate>
#include <boost/lexical_cast.hpp>
#include "Typed_QTreeWidgetItem.h"
namespace Ui {
class Upload_Data;
}

class Upload_Data : public QWidget
{
    Q_OBJECT

public:
    explicit Upload_Data(QWidget *parent = 0);
    ~Upload_Data();
private:
    Ui::Upload_Data *ui;
    Typed_QTreeWidgetItem *selected_item;
private slots:
	void ok_clicked();
	void cancel_clicked();
signals:
	void scheduled_item_added(QDate);
};

#endif // UPLOAD_DATA_H
