#include "Upload_Data.h"
#include "ui_Upload_Data.h"

Upload_Data::Upload_Data(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Upload_Data)
{
    ui->setupUi(this);

	connect(ui->buttonBox, SIGNAL(accepted()),
		this, SLOT(ok_clicked()));
	connect(ui->buttonBox, SIGNAL(rejected()),
		this, SLOT(cancel_clicked()));
}

Upload_Data::~Upload_Data()
{
    delete ui;
}

void Upload_Data::ok_clicked()
{
	emit scheduled_item_added(ui->dateTimeEdit->date());
	this->close();
}

void Upload_Data::cancel_clicked()
{
	this->close();
}
