#include <qmath.h>
#include "edge_ui.hpp"

EdgeUI::EdgeUI(VertexUI *src, VertexUI *dest, unsigned long weight)
	: _weight(weight), arrow_sz(10)
{
	this->src = src;
	this->dest = dest;
	this->src->add_edge(this);
	this->dest->add_edge(this);
	adjust();
}

VertexUI * EdgeUI::src_node() const
{
	return src;
}

VertexUI * EdgeUI::dest_node() const
{
	return dest;
}

void EdgeUI::adjust()
{
	if (!src || !dest)
		return;

	QLineF line(mapFromItem(src, 0, 0), mapFromItem(dest, 0, 0));
	qreal line_len = line.length();
	prepareGeometryChange();

	if (line_len > qreal(20.)) {
		QPointF edge_offset((line.dx() * 10) / line_len,
				    (line.dy() * 10) / line_len);
		src_point = line.p1() + edge_offset;
		dest_point = line.p2() + edge_offset;
	} else {
		src_point = dest_point = line.p1();
	}
}

QRectF EdgeUI::boundingRect() const
{
	if (!src || !dest)
		return QRectF();

	qreal pen_width = 1;
	qreal extra = (pen_width + arrow_sz) / 2.0;
	QRectF rect(src_point, QSizeF(dest_point.x() - src_point.x(),
				      dest_point.y() - src_point.y()));
	return rect.normalized().adjusted(-extra, -extra, extra, extra);
}

void EdgeUI::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	if (!src || !dest)
		return;

	QLineF line(src_point, dest_point);
	if (qFuzzyCompare(line.length(), qreal(0.)))
		return;

	// draw line
	painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawLine(line);
	// draw arrow
	//double angle = std::atan2(-line.dy(), line.dx());

	//QPointF src_arrow_p1 = src_point +
	//		       QPointF(sin(angle + M_PI / 3) * arrow_sz,
	//			       cos(angle + M_PI / 3) * arrow_sz);
	//QPointF src_arrow_p2 = src_point +
	//		       QPointF(sin(angle + M_PI - M_PI / 3) * arrow_sz,
	//			       cos(angle + M_PI - M_PI / 3) * arrow_sz);
	//QPointF dest_arrow_p1 = dest_point +
	//			QPointF(sin(angle - M_PI / 3) * arrow_sz,
	//				cos(angle - M_PI / 3) * arrow_sz);
	//QPointF dest_arrow_p2 = dest_point +
	//			QPointF(sin(angle - M_PI + M_PI / 3) * arrow_sz,
	//				cos(angle - M_PI + M_PI / 3) * arrow_sz);
	//painter->setBrush(Qt::black);
	//painter->drawPolygon((QPolygonF() << line.p1() << src_arrow_p1 << src_arrow_p2));
	//painter->drawPolygon((QPolygonF() << line.p2() << dest_arrow_p1 << dest_arrow_p2));
}
