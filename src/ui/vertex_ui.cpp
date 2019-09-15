#include <QtWidgets>

#include "mainwindow.hpp"
#include "vertex_ui.hpp"
#include "edge_ui.hpp"

VertexUI::VertexUI(unsigned int id)
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

int VertexUI::type() const
{
	return Type;
}

QRectF VertexUI::boundingRect() const
{
	return QRectF(-20, -20, 40, 40);
}

QPainterPath VertexUI::shape() const
{
	QPainterPath path;
	QString id = QString::number(_id);
	QFont id_font;
	QPointF id_pos;

	id_pos.setX(boundingRect().center().x() - id.size()*5);
	id_pos.setY(boundingRect().center().y() + 5);
	id_font.setBold(true);
	path.addEllipse(boundingRect());
	path.addText(id_pos, id_font, id);
	return path;
}

void VertexUI::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->setPen(Qt::NoPen);

	if (_selected_state)
		painter->setBrush(Qt::red);
	else
		painter->setBrush(Qt::green);

	painter->drawPath(shape());
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
	qobject_cast<MainWindow *>(scene()->parent())->show_routing_table(_id);
}

unsigned int VertexUI::id() const
{
	return _id;
}

void VertexUI::state(bool state)
{
	_selected_state = state;
	update();
}
