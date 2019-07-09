#include "vertex_ui.hpp"
#include "edge_ui.hpp"

VertexUI::VertexUI(unsigned long id, QGraphicsView *graph)
	: _id(id), graph_view(graph)
{
	setFlag(ItemIsMovable);
	setFlag(ItemSendsGeometryChanges);
	setCacheMode(DeviceCoordinateCache);
	setZValue(-1);
}

void VertexUI::add_edge(EdgeUI *edge)
{
	edge_list << edge;
	edge->adjust();
}

QRectF VertexUI::boundingRect() const
{
	qreal adjust = 2;
	return QRectF(-10 - adjust, -10 - adjust, 43 + adjust, 43 + adjust);
}

QPainterPath VertexUI::shape() const
{
	QPainterPath path;
	path.addEllipse(-10, -10, 40, 40);
	return path;
}

void VertexUI::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::green);
	painter->drawEllipse(-7, -7, 40, 40);
}

QVariant VertexUI::itemChange(GraphicsItemChange change, const QVariant &value)
{
	switch (change) {
	case ItemPositionHasChanged:
		for (EdgeUI *edge : qAsConst(edge_list))
		    edge->adjust();
		break;
	default:
		break;
	}

	return QGraphicsItem::itemChange(change, value);
}

void VertexUI::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	update();
	QGraphicsItem::mousePressEvent(event);
}

void VertexUI::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	update();
	QGraphicsItem::mouseReleaseEvent(event);
}

unsigned long VertexUI::id() const
{
	return _id;
}
