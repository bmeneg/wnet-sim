#ifndef PATHDEF_DIALOG_HPP
#define PATHDEF_DIALOG_HPP

#include <QDialog>

QT_BEGIN_NAMESPACE
class QDialog;
class QLineEdit;
class QLabel;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
QT_END_NAMESPACE
class MainWindow;

class PathDefDialog : public QDialog
{
public:
	PathDefDialog(QWidget *);

private slots:
	void push_path_values(void);

private:
	MainWindow *_parent;
	QDialog *_path_def_dialog;
	QLineEdit *_src_edit;
	QLineEdit *_dst_edit;
	QLabel *_src_lb;
	QLabel *_dst_lb;
	QPushButton *_ok_btn;
};

#endif // PATHDEF_DIALOG_HPP
