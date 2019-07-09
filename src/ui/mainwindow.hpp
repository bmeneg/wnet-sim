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
	void create_actions(void);
	void create_menus(void);
	void create_status_bar(void);

	void draw_nodes(void);

	Core *core;
	NetworkGraph *ngraph;

	QAction *open_action;
	QAction *exit_action;
	QMenu *file_menu;
	QGraphicsView *graph_view;
	QGraphicsScene *graph_scene;
};

#endif // MAINWINDOW_HPP
