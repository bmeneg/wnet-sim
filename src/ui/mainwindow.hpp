#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

#include "core.hpp"
#include "network_graph.hpp"
#include "vertex_ui.hpp"
#include "pathdef_dialog.hpp"

QT_BEGIN_NAMESPACE
class QGraphicsView;
class QAction;
class QMenu;
class QMenuBar;
class QListWidget;
class QListWidgetItem;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(Core *, QWidget *parent = nullptr);
	~MainWindow();

	void path_def(unsigned int, unsigned int);
	void show_routing_table(unsigned int);
	void show_msg_table(unsigned int);

private slots:
	void new_random_graph();
	void open_graph_file();
	void find_shortest_path();
	void find_shortest_path_from_view(QListWidgetItem *);
	void show_msg_rt_table(QListWidgetItem *);

private:
	void _create_actions(void);
	void _create_menus(void);
	void _create_status_bar(void);
	void _draw_nodes(void);
	void _clear_all(void);

	QAction *_open_action;
	QAction *_new_action;
	QAction *_exit_action;
	QAction *_find_shortest_action;
	QMenu *_file_menu;
	QMenu *_tools_menu;
	QGraphicsView *_graph_view;
	QGraphicsScene *_graph_scene;
	QListWidget *_rtable_view;
	QListWidget *_msg_table_view;
	QListWidget *_msg_rt_table_view;

	PathDefDialog *_path_def_dialog;
	NetworkGraph *_ngraph;

	bool _clean_run = true;
	unsigned int _last_id_clicked = UINT_MAX;
};

#endif // MAINWINDOW_HPP
