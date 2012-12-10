#include "Upload_Files_To_Server_Dialog.h"


Upload_Files_To_Server_Dialog::Upload_Files_To_Server_Dialog(QWidget *parent /*= 0*/, Qt::WFlags flags /*= 0*/)
{
	ui.setupUi(this);
	ui.dateTimeEdit = new QDateTimeEdit(QTime::currentTime());
	ui.checkBox->setChecked(true);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(add_files()));
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(ok_clicked()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(cancel_clicked()));
}


Upload_Files_To_Server_Dialog::~Upload_Files_To_Server_Dialog(void)
{
}

void Upload_Files_To_Server_Dialog::add_files()
{
#ifdef _SHOW_DEBUG_OUTPUT
	std::cout << "= Upload_Files_To_Server_Dialog::add_files()" << std::endl;
#endif // _DEBUG
	QStringList filenames = QFileDialog::getOpenFileNames(this,
		tr("Select file(s) to add to playlist"));
	foreach(QString filename, filenames)
	{
#ifdef _SHOW_DEBUG_OUTPUT
		std::cout << " - Adding " << qPrintable(filename) << std::endl;
#endif // _DEBUG
		if(std::find(selected_files.begin(), selected_files.end(), filename.toStdString()) 
			!= selected_files.end())
		{
			QMessageBox msgBox;
			msgBox.setText(filename + " is already in the playlist");
			msgBox.exec();
		}
		else
		{
			selected_files.push_back(filename.toStdString());
		}
	}
	repopulate_widget();
}

void Upload_Files_To_Server_Dialog::repopulate_widget()
{
	ui.listWidget->clear();
	for (std::vector<std::string>::iterator it = selected_files.begin();
		it != selected_files.end(); ++it)
	{
		ui.listWidget->addItem(QString::fromStdString(*it));
	}
}

void Upload_Files_To_Server_Dialog::ok_clicked()
{
	if (selected_files.size() > 0)
	{
		if (ui.checkBox->isChecked())
		{
			QTime time(22,22,22);
			emit selection_complete(selected_files, time);
			this->close();
		}
		else
		{
			emit selection_complete(selected_files, upload_time);
			this->close();
		}
	}
	else
	{
		QMessageBox messagebox;
		messagebox.setText("No files selected to upload, Please select cancel if this was the intention");
	}
}

void Upload_Files_To_Server_Dialog::cancel_clicked()
{
	this->close();
}

void Upload_Files_To_Server_Dialog::show_gui()
{
	this->show();
}
