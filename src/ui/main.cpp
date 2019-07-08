#include "mainwindow.hpp"
#include "core.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
	// run program without GUI
	if (init_core(argc, argv) > 0)
		return run_core_cli();

	// run program with GUI
	QApplication app(argc, argv);
	MainWindow mainWindow;
	mainWindow.setGeometry(100, 100, 800, 500);
	mainWindow.show();
	return app.exec();
}
