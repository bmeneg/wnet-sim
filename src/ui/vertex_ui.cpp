#include "vertex_ui.hpp"
#include "edge_ui.hpp"

VertexUI::VertexUI(unsigned long id)
	: _id(id)
{
	setFlag(ItemIsMovable);
	setFlag(ItemSendsGeometryChanges);
	setCacheMode(DeviceCoordinateCache);
	setZValue(-1);
}

void VertexUI::add_edge(EdgeUI *edge)
{
	_edge_list.push_back(edge);
	edge->adjust();
}

QRectF VertexUI::boundingRect() const
{
	return QRectF(-20, -20, 40, 40);
}

QPainterPath VertexUI::shape() const
{
	QPainterPath path;
	path.addEllipse(boundingRect());
	return path;
}

void VertexUI::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::green);
	painter->drawEllipse(boundingRect());
}

QVariant VertexUI::itemChange(GraphicsItemChange change, const QVariant &value)
{
	switch (change) {
	case ItemPositionHasChanged:
		for (EdgeUI *edge : qAsConst(_edge_list))
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
