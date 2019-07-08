#ifndef NODES_HPP
#define NODES_HPP

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsSceneEvent>

class Node : public QGraphicsItem
{
public:
	Node(QGraphicsView *graph_view);

	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		   QWidget *widget) override;

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
	QPointF new_pos;
	QGraphicsView *graph_view;
};

#endif // NODES_HPP
