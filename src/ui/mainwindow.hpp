#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QGraphicsView>
#include <QAction>
#include <QMenu>
#include <QMenuBar>

#include "core.hpp"
#include "network_graph.hpp"
#include "vertex_ui.hpp"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(Core *, QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void open_graph_file();

private:
	void _create_actions(void);
	void _create_menus(void);
	void _create_status_bar(void);

	void _draw_nodes(void);

	Core *_core;
	NetworkGraph *_ngraph;

	QAction *_open_action;
	QAction *_exit_action;
	QMenu *_file_menu;
	QGraphicsView *_graph_view;
	QGraphicsScene *_graph_scene;
};

#endif // MAINWINDOW_HPP
