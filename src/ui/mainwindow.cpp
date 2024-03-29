#include <QtWidgets>
#include <QDebug>

#include "mainwindow.hpp"
#include "core.hpp"
#include "edge_ui.hpp"
#include "protocol.hpp"

MainWindow::MainWindow(Core *core, QWidget *parent)
	: QMainWindow(parent), _ngraph(core->network_graph())
{
	_create_actions();
	_create_menus();
	_create_status_bar();

	_graph_scene = new QGraphicsScene(this);
	_graph_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	_graph_scene->setSceneRect(-200, -200, 400, 400);

	_graph_view = new QGraphicsView(this);
	_graph_view->setAlignment(Qt::AlignTop|Qt::AlignLeft);
	_graph_view->setScene(_graph_scene);
	_graph_view->setCacheMode(QGraphicsView::CacheBackground);
	_graph_view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	_graph_view->setRenderHint(QPainter::Antialiasing);
	_graph_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

	setCentralWidget(_graph_view);

	QDockWidget *dock_rt = new QDockWidget(tr("Routing table"), this);
	dock_rt->setAllowedAreas(Qt::BottomDockWidgetArea);
	_rtable_view = new QListWidget(dock_rt);
	connect(_rtable_view, &QListWidget::itemClicked, this,
			&MainWindow::find_shortest_path_from_view);
	dock_rt->setWidget(_rtable_view);
	dock_rt->setMaximumHeight(150);
	addDockWidget(Qt::BottomDockWidgetArea, dock_rt);

	QDockWidget *dock_msg = new QDockWidget(tr("Messages"), this);
	dock_msg->setAllowedAreas(Qt::RightDockWidgetArea);
	_msg_table_view = new QListWidget(dock_msg);
	dock_msg->setWidget(_msg_table_view);
//	dock_msg->setMaximumWidth(150);
	addDockWidget(Qt::RightDockWidgetArea, dock_msg);

	QDockWidget *dock_msg_rt = new QDockWidget(tr("Neighbor Routing table"), this);
	dock_msg_rt->setAllowedAreas(Qt::BottomDockWidgetArea);
	_msg_rt_table_view = new QListWidget(dock_msg_rt);
	connect(_msg_rt_table_view, &QListWidget::itemClicked, this,
			&MainWindow::show_msg_rt_table);
	dock_msg_rt->setWidget(_msg_rt_table_view);
	dock_msg_rt->setMaximumHeight(150);
	addDockWidget(Qt::BottomDockWidgetArea, dock_msg_rt);

	setMinimumSize(800, 600);
	setWindowTitle(tr("Wireless Network Simulator"));
}

MainWindow::~MainWindow()
{
}

// if the user is running the program more than once within a single startup
// it's necessary to clear all data we have about the last graph: scene, tables
// vectors, ..
void MainWindow::_clear_all()
{
	_graph_scene->clear();
	_ngraph->clear_graph();
	if (_rtable_view->count() > 0)
		_rtable_view->clear();
	if (_msg_table_view->count() > 0)
		_msg_table_view->clear();
	if (_msg_rt_table_view->count() > 0)
		_msg_rt_table_view->clear();
	_clean_run = true;
}

void MainWindow::new_random_graph()
{
	// make random seems a true rng
	srand(static_cast<unsigned int>(time(nullptr)));

	// repeat until there are no graphs disconnected within the generated one
	do {
		if (!_clean_run)
			_clear_all();

		_ngraph->add_routes_random();
		// add_routes_random actually generates a file with all nodes
		// that's why calling add_routes_from_file is fine
		_ngraph->add_routes_from_file();
		_clean_run = false;
	} while (_ngraph->calc_routing_graph() != 0);
	statusBar()->showMessage(tr("Random nodes created"), 5000);
	_draw_nodes();
}

void MainWindow::open_graph_file()
{
	if (!_clean_run)
		_clear_all();

	QString filename = QFileDialog::getOpenFileName(this);
	if (!filename.isEmpty()) {
		_ngraph->graph_file(filename.toStdString());
		_ngraph->add_routes_from_file();
		statusBar()->showMessage(tr("File loaded"), 5000);
		if (_ngraph->calc_routing_graph() != 0) {
			QMessageBox msg_box;
			msg_box.setWindowTitle("Disconnected graphs");
			msg_box.setText("Disconnected graphs were inserted.");
			msg_box.setSizeIncrement(msg_box.sizeHint());
			msg_box.setStandardButtons(QMessageBox::Ok);
			msg_box.setDefaultButton(QMessageBox::Ok);
			msg_box.exec();
		}
		_draw_nodes();
		_clean_run = false;
	} else {
		statusBar()->showMessage(tr("No file were loaded. Ignoring."), 5000);
	}
}

void MainWindow::find_shortest_path()
{
	_path_def_dialog = new PathDefDialog(this);
}

void MainWindow::find_shortest_path_from_view(QListWidgetItem *item)
{
	QStringList item_text = item->text().split(" ");

	path_def(_last_id_clicked, item_text.at(1).toUInt());
}

void MainWindow::_create_status_bar()
{
	statusBar()->showMessage(tr("Ready"));
}

void MainWindow::_create_actions()
{
	_new_action = new QAction(tr("&New"));
	_new_action->setShortcut((QKeySequence::New));
	_new_action->setStatusTip(tr("Create a new and random graph"));
	connect(_new_action, &QAction::triggered, this,
			&MainWindow::new_random_graph);

	_open_action = new QAction(tr("&Open"));
	_open_action->setShortcut(QKeySequence::Open);
	_open_action->setStatusTip(tr("Open graph file"));
	connect(_open_action, &QAction::triggered, this,
			&MainWindow::open_graph_file);

	_exit_action = new QAction(tr("&Exit"), this);
	_exit_action->setShortcuts(QKeySequence::Quit);
	_exit_action->setStatusTip(tr("Quit WNet-Sim"));
	connect(_exit_action, &QAction::triggered, this, &QWidget::close);

	_find_shortest_action = new QAction(tr("&Find Shortest Path"));
	_find_shortest_action->setShortcut(QKeySequence::Find);
	_find_shortest_action->setStatusTip(tr("Find the shortest path in the "
										   "current graph"));
	_find_shortest_action->setEnabled(false);
	connect(_find_shortest_action, &QAction::triggered, this,
			&MainWindow::find_shortest_path);
}

void MainWindow::_create_menus()
{
	_file_menu = menuBar()->addMenu(tr("&File"));
	_file_menu->addAction(_new_action);
	_file_menu->addAction(_open_action);
	_file_menu->addSeparator();
	_file_menu->addAction(_exit_action);

	_tools_menu = menuBar()->addMenu(tr("&Tools"));
	_tools_menu->addAction(_find_shortest_action);
}

void MainWindow::_draw_nodes()
{
	auto edges_attr = _ngraph->graph_edges();
	VertexUI *node1, *node2;
	EdgeUI *edge;
//	QRandomGenerator *rand = QRandomGenerator::system();
//	QSize gview_sz = _graph_view->size();

	for (auto graph_edge : edges_attr) {
		auto vid_pair = graph_edge.second;
		node1 = new VertexUI(vid_pair.first);
		node2 = new VertexUI(vid_pair.second);
		bool node1_found = false;
		bool node2_found = false;

		for (QGraphicsItem *item : _graph_scene->items()) {
			VertexUI *tmp_node = qgraphicsitem_cast<VertexUI *>(item);
			if (tmp_node != nullptr) {
				if (!node1_found && tmp_node->id() == node1->id()) {
					node1 = tmp_node;
					node1_found = true;
				} else if (!node2_found && tmp_node->id() == node2->id()) {
					node2 = tmp_node;
					node2_found = true;
				}
			}
		}

		if (!node1_found) {
			//node1->setPos(rand->bounded(-gview_sz.width()/3, gview_sz.width()/3),
			//	rand->bounded(-gview_sz.height()/3, gview_sz.height()/3));
			node1->setPos(node1->vertex()->coord.x * _graph_scene->width()/(_ngraph->grid_dim()+5),
						  node1->vertex()->coord.y * _graph_scene->height()/(_ngraph->grid_dim()+5));
			_graph_scene->addItem(node1);
		}
		if (!node2_found) {
			//node2->setPos(rand->bounded(-gview_sz.width()/3, gview_sz.width()/3),
			//	rand->bounded(-gview_sz.height()/3, gview_sz.height()/3));
			node2->setPos(node2->vertex()->coord.x * _graph_scene->width()/(_ngraph->grid_dim()+5),
						  node2->vertex()->coord.y * _graph_scene->height()/(_ngraph->grid_dim()+5));
			_graph_scene->addItem(node2);
		}
		edge = new EdgeUI(graph_edge.first, node1, node2);
		_graph_scene->addItem(edge);
	}

	_find_shortest_action->setEnabled(true);
}

void MainWindow::path_def(unsigned int src_id, unsigned int dst_id)
{
	route_t route;
	QList<QGraphicsItem *> items_list = _graph_scene->items();
	bool src_found = false;

	route = _ngraph->find_shortest_path(src_id, dst_id);
#ifdef RUNTIME_PROCESSING
	if (route.second.size() == 0) {
		QMessageBox msg_box;
		msg_box.setWindowTitle("Disconnected graphs");
		msg_box.setText("The requested nodes are not in the same graph");
		msg_box.setStandardButtons(QMessageBox::Ok);
		msg_box.setDefaultButton(QMessageBox::Ok);
		msg_box.exec();
		return;
	}
#endif // RUNTIME_PROCESSING

	// reset vertices state if that's not the first time running this method
	// over the same graph
	if (!_clean_run) {
		for (QGraphicsItem *item : items_list) {
			VertexUI *tmp_node = qgraphicsitem_cast<VertexUI *>(item);
			if (tmp_node != nullptr) {
				tmp_node->state(false);
			}
		}
	}

	// actually set the state of each vertex that matches the route
	for (auto vertex : route.second) {
		for (QGraphicsItem *item : items_list) {
			VertexUI *tmp_node = qgraphicsitem_cast<VertexUI *>(item);
			if (tmp_node != nullptr) {
				if (tmp_node->id() == vertex->id) {
					tmp_node->state(true);
				} else if (!src_found && tmp_node->id() == src_id) {
					// src_id is not listed in the route table, that's why we
					// need to explicitly check for it
					src_found = true;
					tmp_node->state(true);
				}
			}
		}
	}

	// that's not the first time the user is running the program
	_clean_run = false;
}

void MainWindow::show_routing_table(unsigned int id)
{
	route_t route;
	QString str;

	if (_last_id_clicked == id)
		return;

	_last_id_clicked = id;

	if (_rtable_view->count() > 0)
		_rtable_view->clear();

	for (QGraphicsItem *item : _graph_scene->items()) {
		VertexUI *tmp_node = qgraphicsitem_cast<VertexUI *>(item);
		if (tmp_node != nullptr) {
			if (tmp_node->id() == id)
				continue;

			route = _ngraph->find_shortest_path(id, tmp_node->id());
			// weight == 0, no valid path to that vertex
			if (route.first.cost == 0)
				continue;

			str = "dest: " + QString::number(tmp_node->id()) + " route: ";
			for (auto route_vertices : route.second) {
				str += QString::number(route_vertices->id) + " ";
			}
			str += "weight: " + QString::number(route.first.cost);
			_rtable_view->addItem(str);
		}
	}
}

void MainWindow::show_msg_table(unsigned int id)
{
	QString str;
	ProtoMsg proto_msg;

	if (_msg_table_view->count() > 0)
		_msg_table_view->clear();

	for (QGraphicsItem *item : _graph_scene->items()) {
		VertexUI *tmp_node = qgraphicsitem_cast<VertexUI *>(item);
		if (tmp_node != nullptr) {
			if (tmp_node->id() == id)
				continue;

			for (auto msg : _ngraph->vertex(tmp_node->id())->msg_table) {
				proto_msg = msg;
				str = "src: " + QString::number(msg.src_id) + " msg: ";
				if (msg.payload.msg == 0)
					str += "HELLO ";
				else
					str += "UPDATE ";

				if (msg.dst_id == BROADCAST_ID)
					str += "dst: broad";
				else
					str += "dst: " + QString::number(msg.dst_id);
				_msg_table_view->addItem(str);
			}
		}
	}
}

void MainWindow::show_msg_rt_table(QListWidgetItem *item)
{
	QStringList item_text = item->text().split(" ");
	unsigned int vertex_id = item_text.at(1).toUInt();
	QString str;

	if (_msg_rt_table_view->count() > 0)
		_msg_rt_table_view->clear();

	str = "not implemented yet";
	_msg_rt_table_view->addItem(str);

	for (QGraphicsItem *item : _graph_scene->items()) {
		VertexUI *tmp_node = qgraphicsitem_cast<VertexUI *>(item);
		if (tmp_node != nullptr) {
			if (tmp_node->id() != vertex_id)
				continue;

			for (auto msg : tmp_node->vertex()->msg_table) {
				for (auto proto_route : msg.payload.routing_table) {
					// TDB
				}
			}
		}
	}
}
