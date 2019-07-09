#include "mainwindow.hpp"
#include "core.hpp"
#include "edge_ui.hpp"

#include <QFileDialog>
#include <QStatusBar>

MainWindow::MainWindow(Core *core, QWidget *parent)
	: QMainWindow(parent), core(core)
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
		ngraph = core->run_gui(filename.toStdString());
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
	auto edges_attr = ngraph->graph_edges();
	VertexUI *node1, *node2;
	EdgeUI *edge;

	for (auto graph_edge : edges_attr) {
		auto vid_pair = graph_edge.second;
		node1 = new VertexUI(vid_pair.first, graph_view);
		node2 = new VertexUI(vid_pair.second, graph_view);

		for (QGraphicsItem *item : graph_scene->items()) {
			VertexUI *tmp_node = qgraphicsitem_cast<VertexUI *>(item);
			if (tmp_node) {
				if (tmp_node->id() == node1->id())
					node1 = tmp_node;
				else if (tmp_node->id() == node2->id())
					node2 = tmp_node;
			}
		}

		edge = new EdgeUI(node1, node2, graph_edge.first);
		graph_scene->addItem(node1);
		graph_scene->addItem(node2);
		graph_scene->addItem(edge);
	}
}
