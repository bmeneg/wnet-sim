#include "mainwindow.hpp"
#include "core.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
	Core core;
	// run program without GUI
	if (core.init(argc, argv) > 0)
		return core.run_cli();

	// run program with GUI
	QApplication app(argc, argv);
	MainWindow mainWindow(&core);
	mainWindow.setGeometry(100, 100, 800, 500);
	mainWindow.show();
	return app.exec();
}
