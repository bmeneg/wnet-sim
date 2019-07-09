#ifndef NODES_HPP
#define NODES_HPP

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsSceneEvent>

class EdgeUI;

class VertexUI : public QGraphicsItem
{
public:
	VertexUI(unsigned long, QGraphicsView *);
	void add_edge(EdgeUI *);
	unsigned long id(void) const;
	QList<EdgeUI *> edges() const;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;

protected:
	QVariant itemChange(GraphicsItemChange, const QVariant &) override;
	void mousePressEvent(QGraphicsSceneMouseEvent *) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;

private:
	unsigned long _id;
	QList<EdgeUI *> edge_list;

	QPointF new_pos;
	QGraphicsView *graph_view;
};

#endif // NODES_HPP
