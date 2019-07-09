#ifndef EDGE_HPP
#define EDGE_HPP

#include <QGraphicsItem>
#include <QGraphicsView>
#include "vertex_ui.hpp"

class EdgeUI : public QGraphicsItem
{
public:
	EdgeUI(VertexUI *, VertexUI *, unsigned long);
	VertexUI *src_node(void) const;
	VertexUI *dest_node(void) const;
	unsigned long weight(void) const;
	void adjust(void);

protected:
	QRectF boundingRect() const override;
	void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;

private:
	VertexUI *src;
	VertexUI *dest;
	unsigned long _weight;

	QPointF src_point;
	QPointF dest_point;
	qreal arrow_sz;
};

#endif // EDGE_HPP