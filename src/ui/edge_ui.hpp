#ifndef EDGE_UI_HPP
#define EDGE_UI_HPP

#include <QGraphicsItem>

QT_BEGIN_NAMESPACE
class QGraphicsView;
QT_END_NAMESPACE
class VertexUI;

class EdgeUI : public QGraphicsItem
{
public:
	enum { Type = UserType + 2 };

	EdgeUI(VertexUI *, VertexUI *, unsigned int);
	VertexUI *src_node(void) const;
	VertexUI *dest_node(void) const;
	unsigned int weight(void) const;
	void adjust(void);
	int type(void) const override;

protected:
	QRectF boundingRect() const override;
	void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;

private:
	VertexUI *_src;
	VertexUI *_dest;
	unsigned int _weight;

	QPointF _src_point;
	QPointF _dest_point;
};

#endif // EDGE_HPP
