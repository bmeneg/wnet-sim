#include "mainwindow.hpp"
#include "core.hpp"
#include "edge_ui.hpp"

#include <QFileDialog>
#include <QStatusBar>
#include <QRandomGenerator>

MainWindow::MainWindow(Core *core, QWidget *parent)
	: QMainWindow(parent), _core(core)
{
	_create_actions();
	_create_menus();
	_create_status_bar();

	_graph_scene = new QGraphicsScene(this);
	_graph_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	_graph_scene->setSceneRect(-200, -200, 400, 400);

	_graph_view = new QGraphicsView(this);
	_graph_view->setScene(_graph_scene);
	_graph_view->setCacheMode(QGraphicsView::CacheBackground);
	_graph_view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	_graph_view->setRenderHint(QPainter::Antialiasing);
	_graph_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

	setMinimumSize(400, 400);
	setWindowTitle(tr("Wireless Network Simulator"));
	setCentralWidget(_graph_view);
}

MainWindow::~MainWindow()
{
}

void MainWindow::open_graph_file()
{
	QString filename = QFileDialog::getOpenFileName(this);
	if (!filename.isEmpty())
		_ngraph = _core->run_gui(filename.toStdString());
	statusBar()->showMessage(tr("File loaded"), 2000);
	_draw_nodes();
}

void MainWindow::_create_status_bar()
{
	statusBar()->showMessage(tr("Ready"));
}

void MainWindow::_create_actions()
{
	_open_action = new QAction(tr("&Open"));
	_open_action->setShortcut(QKeySequence::Open);
	_open_action->setStatusTip(tr("Open graph file"));
	connect(_open_action, &QAction::triggered, this, &MainWindow::open_graph_file);

	_exit_action = new QAction(tr("&Exit"), this);
	_exit_action->setShortcuts(QKeySequence::Quit);
	_exit_action->setStatusTip(tr("Quit WNet-Sim"));
	connect(_exit_action, &QAction::triggered, this, &QWidget::close);
}

void MainWindow::_create_menus()
{
	_file_menu = menuBar()->addMenu(tr("&File"));
	_file_menu->addAction(_open_action);
	_file_menu->addSeparator();
	_file_menu->addAction(_exit_action);
}

void MainWindow::_draw_nodes()
{
	auto edges_attr = _ngraph->graph_edges();
	VertexUI *node1, *node2;
	EdgeUI *edge;
	QRandomGenerator *rand = QRandomGenerator::system();
	QSize gview_sz = _graph_view->size();

	for (auto graph_edge : edges_attr) {
		auto vid_pair = graph_edge.second;
		node1 = new VertexUI(vid_pair.first);
		node2 = new VertexUI(vid_pair.second);
		bool node1_found = false;
		bool node2_found = false;

		for (QGraphicsItem *item : _graph_scene->items()) {
			VertexUI *tmp_node = qgraphicsitem_cast<VertexUI *>(item);
			if (tmp_node) {
				if (!node1_found && tmp_node->id() == node1->id()) {
					node1 = tmp_node;
					node1_found = true;
				} else if (!node2_found && tmp_node->id() == node2->id()) {
					node2 = tmp_node;
					node2_found = true;
				}
			}
		}

		edge = new EdgeUI(node1, node2, graph_edge.first);
		if (!node1_found) {
			node1->setPos(rand->bounded(-gview_sz.width()/3, gview_sz.width()/3),
				rand->bounded(-gview_sz.height()/3, gview_sz.height()/3));
			_graph_scene->addItem(node1);
		}
		if (!node2_found) {
			node2->setPos(rand->bounded(-gview_sz.width()/3, gview_sz.width()/3),
				rand->bounded(-gview_sz.height()/3, gview_sz.height()/3));
			_graph_scene->addItem(node2);
		}
		_graph_scene->addItem(edge);
	}
}
