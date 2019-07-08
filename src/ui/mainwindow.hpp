#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QGraphicsView>
#include <QAction>
#include <QMenu>
#include <QMenuBar>

#include "node_ui.hpp"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void open_graph_file();

private:
	void create_actions(void);
	void create_menus(void);
	void create_status_bar(void);

	void draw_nodes(void);

	QAction *open_action;
	QAction *exit_action;
	QMenu *file_menu;
	QGraphicsView *graph_view;
	QGraphicsScene *graph_scene;
};

#endif // MAINWINDOW_HPP
