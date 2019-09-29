#ifndef EDGE_UI_HPP
#define EDGE_UI_HPP

#include <QGraphicsItem>
#include "network_graph.hpp"

QT_BEGIN_NAMESPACE
class QGraphicsView;
QT_END_NAMESPACE
class VertexUI;

class EdgeUI : public QGraphicsItem
{
public:
	enum { Type = UserType + 2 };

	EdgeUI(struct Edge, VertexUI *, VertexUI *);
	VertexUI *src_node(void) const;
	VertexUI *dest_node(void) const;
	Edge edge(void) const;
	unsigned int weight(void) const;
	void adjust(void);
	int type(void) const override;

protected:
	QRectF boundingRect() const override;
	void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;

private:
	struct Edge _edge;
	VertexUI *_src;
	VertexUI *_dest;
	//unsigned int _weight;

	QPointF _src_point;
	QPointF _dest_point;
};

#endif // EDGE_HPP
