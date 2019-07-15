#ifndef EDGE_UI_HPP
#define EDGE_UI_HPP

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
	VertexUI *_src;
	VertexUI *_dest;
	unsigned long _weight;

	QPointF _src_point;
	QPointF _dest_point;
};

#endif // EDGE_HPP
