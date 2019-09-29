#ifndef VERTEX_UI_HPP
#define VERTEX_UI_HPP

#include <QGraphicsItem>
#include <QBrush>

QT_BEGIN_NAMESPACE
class QGraphicsView;
class QGraphicsSceneEvent;
QT_BEGIN_NAMESPACE
class EdgeUI;
struct Vertex;

class VertexUI : public QGraphicsItem
{
public:
	// set it as a new type o QGraphicsItem, thus we can _cast() it
	enum { Type = UserType + 1 };

	VertexUI(struct Vertex *);
	void add_edge(EdgeUI *);
	Vertex * vertex(void) const;
	unsigned int id(void) const;
	void state(bool);
	QList<EdgeUI *> edges() const;
	int type(void) const override;
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;

protected:
	QVariant itemChange(GraphicsItemChange, const QVariant &) override;
	void mousePressEvent(QGraphicsSceneMouseEvent *) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;

private:
	struct Vertex *_vertex;
	//unsigned int _id;
	bool _selected_state = false;
	QList<EdgeUI *> _edge_list;

	QPointF _new_pos;
};

#endif // NODES_HPP
