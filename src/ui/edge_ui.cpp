#include "edge_ui.hpp"

EdgeUI::EdgeUI(VertexUI *src, VertexUI *dest, unsigned long weight)
	: _src(src), _dest(dest), _weight(weight)
{
	_src->add_edge(this);
	_dest->add_edge(this);
	adjust();
}

VertexUI * EdgeUI::src_node() const
{
	return _src;
}

VertexUI * EdgeUI::dest_node() const
{
	return _dest;
}

void EdgeUI::adjust()
{
	if (!_src || !_dest)
		return;

	QLineF line(mapFromItem(_src, 0, 0), mapFromItem(_dest, 0, 0));
	qreal line_len = line.length();
	prepareGeometryChange();

	if (line_len > qreal(20.)) {
		QPointF edge_offset((line.dx() * 10) / line_len,
				    (line.dy() * 10) / line_len);
		_src_point = line.p1() + edge_offset;
		_dest_point = line.p2() + edge_offset;
	} else {
		_src_point = _dest_point = line.p1();
	}
}

QRectF EdgeUI::boundingRect() const
{
	if (!_src || !_dest)
		return QRectF();

	QRectF rect(_src_point, QSizeF(_dest_point.x() - _src_point.x(),
								   _dest_point.y() - _src_point.y()));
	return rect.normalized();
}

void EdgeUI::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	if (!_src || !_dest)
		return;

	QLineF line(_src_point, _dest_point);
	if (qFuzzyCompare(line.length(), qreal(0.)))
		return;

	painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawLine(line);
}
