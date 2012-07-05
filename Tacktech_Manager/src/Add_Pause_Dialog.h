#ifndef ADD_PAUSE_DIALOG_H
#define ADD_PAUSE_DIALOG_H

#include <QtGui/QDialog>
#include "ui_Add_Pause_Dialog.h"

class Add_Pause_Dialog : public QDialog
{
    Q_OBJECT

public:
    Add_Pause_Dialog(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~Add_Pause_Dialog();

private:
    Ui::Add_Pause_DialogClass ui;
private slots:
    void ok_clicked();
    void cancel_clicked();
signals:
    void pause_changed(int);
    void pause_unchanged();
};

#endif // ADD_PAUSE_DIALOG_H
