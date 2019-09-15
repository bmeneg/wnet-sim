#include <QtWidgets>

#include "pathdef_dialog.hpp"
#include "mainwindow.hpp"

PathDefDialog::PathDefDialog(QWidget *parent)
{
	_parent = qobject_cast<MainWindow *>(parent);

	_src_edit = new QLineEdit;
	_src_edit->setToolTip(tr("Source node ID"));
	_dst_edit = new QLineEdit;
	_dst_edit->setToolTip(tr("Destiation node ID"));

	_src_lb = new QLabel(tr("Source ID:"));
	_src_lb->setBuddy(_src_edit);
	_dst_lb = new QLabel(tr("Destination ID:"));
	_dst_lb->setBuddy(_dst_edit);

	_ok_btn = new QPushButton("Find");
	connect(_ok_btn, &QPushButton::clicked, this,
			&PathDefDialog::push_path_values);

	QHBoxLayout *src_hl = new QHBoxLayout;
	src_hl->addWidget(_src_lb);
	src_hl->addWidget(_src_edit);
	QHBoxLayout *dst_hl = new QHBoxLayout;
	dst_hl->addWidget(_dst_lb);
	dst_hl->addWidget(_dst_edit);
	QVBoxLayout *vlayout = new QVBoxLayout;
	vlayout->addItem(src_hl);
	vlayout->addItem(dst_hl);
	vlayout->addWidget(_ok_btn);

	setLayout(vlayout);
	setWindowTitle(tr("Choose Path"));
	show();
}

void PathDefDialog::push_path_values()
{
	_parent->path_def(_src_edit->text().toUInt(), _dst_edit->text().toUInt());
	this->close();
}
