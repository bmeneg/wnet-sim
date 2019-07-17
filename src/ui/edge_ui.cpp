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
	/* we are going to use node radius to calculate line offset on both source
	 * and destination vertex. Since both have the same shape we can choose _src
	 * or _dest inhere for retrieve such value */
	qreal vertex_radius = qAbs(_src->boundingRect().x());
	qreal vertex_width = _src->boundingRect().width();

	if (line_len > qreal(vertex_width)) {
		QPointF edge_offset(line.dx() * vertex_radius / line_len,
			line.dy() * vertex_radius / line_len);
		_src_point = line.p1() + edge_offset;
		_dest_point = line.p2() - edge_offset;
	} else {
		_src_point = _dest_point = line.p1();
	}
}

QRectF EdgeUI::boundingRect() const
{
	if (!_src || !_dest)
		return QRectF();

	QRectF line_rect(_src_point, QSizeF(_dest_point.x() - _src_point.x(),
		_dest_point.y() - _src_point.y()));
	QFontMetricsF cost_fontm(scene()->font());
	QRectF text_rect = cost_fontm.boundingRect(QString::number(_weight)).translated(line_rect.center());

	return line_rect.united(text_rect);
}

void EdgeUI::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	if (!_src || !_dest)
		return;

	QLineF line(_src_point, _dest_point);
	if (qFuzzyCompare(line.length(), qreal(0.)))
		return;

	painter->setPen(QPen(Qt::black));
	painter->drawLine(line);
	painter->drawText(line.center(), QString::number(_weight));
}
