#include "mainwindow.hpp"
#include "core.hpp"

#include <QFileDialog>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	create_actions();
	create_menus();
	create_status_bar();

	graph_scene = new QGraphicsScene(this);
	graph_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	graph_scene->setSceneRect(-200, -200, 400, 400);

	graph_view = new QGraphicsView(this);
	graph_view->setScene(graph_scene);
	graph_view->setCacheMode(QGraphicsView::CacheBackground);
	graph_view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	graph_view->setRenderHint(QPainter::Antialiasing);
	graph_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

	this->setMinimumSize(400, 400);
	this->setWindowTitle(tr("Wireless Network Simulator"));
	this->setCentralWidget(graph_view);
}

MainWindow::~MainWindow()
{
}

void MainWindow::open_graph_file()
{
	QString filename = QFileDialog::getOpenFileName(this);
	if (!filename.isEmpty())
		run_core_gui(filename.toStdString());
	statusBar()->showMessage(tr("File loaded"), 2000);
	draw_nodes();
}

void MainWindow::create_status_bar()
{
	statusBar()->showMessage(tr("Ready"));
}

void MainWindow::create_actions()
{
	open_action = new QAction(tr("&Open"));
	open_action->setShortcut(QKeySequence::Open);
	open_action->setStatusTip(tr("Open graph file"));
	connect(open_action, &QAction::triggered, this, &MainWindow::open_graph_file);

	exit_action = new QAction(tr("&Exit"), this);
	exit_action->setShortcuts(QKeySequence::Quit);
	exit_action->setStatusTip(tr("Quit WNet-Sim"));
	connect(exit_action, &QAction::triggered, this, &QWidget::close);
}

void MainWindow::create_menus()
{
	file_menu = menuBar()->addMenu(tr("&File"));
	file_menu->addAction(open_action);
	file_menu->addSeparator();
	file_menu->addAction(exit_action);
}

void MainWindow::draw_nodes()
{
	Node *node1 = new Node(graph_view);
	graph_scene->addItem(node1);
	node1->setPos(10, 10);
}
