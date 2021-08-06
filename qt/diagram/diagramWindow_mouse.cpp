#include <QtGui/QMouseEvent>

#include "diagramWindow.h"

static std::shared_ptr<KE::TwoD::Diagram::Vertex> localVertex;
static int localx, localy;
static bool doSomething = false;

void DiagramWidget::mousePressEvent(QMouseEvent *m) {
	switch (Parent->mode) {
		case NEW_DIAGRAM:
			if (Parent->isEmpty()) {
				Parent->actions_clear->setEnabled (true);
			}
			if (this->diagram.isClosed()) {
				return;
			}
			localVertex = this->diagram.addVertex(m->x(), m->y());
			if (m->button() == 0x02) {
				this->diagram.close();
				Parent->actions_convert->setEnabled(true);
				Parent->actions_simplify->setEnabled(true);
				Parent->actions[0]->setChecked(false);
			}
			repaint();
			Parent->isSaved = false;
			doSomething = true;
			return;
		case ADD_VERTEX:
		{
			std::shared_ptr<KE::TwoD::Diagram::Edge> edge = this->diagram.findEdge(KE::TwoD::FloatPoint(m->x(), m->y()), 5);
			if (edge) {
				localVertex = this->diagram.addVertex(*edge, m->x(), m->y());
				repaint();
				Parent->isSaved = false;
				doSomething = true;
			}
			return;
		}
		case MOVE_VERTEX:
		{
			const auto vertex = this->diagram.findVertex(KE::TwoD::FloatPoint(m->x(), m->y()), 17);
			if (vertex) {
				localVertex = vertex;
				Parent->isSaved = false;
				doSomething = true;
			}
			return;
		}
		case REMOVE_VERTEX:
		{
			const auto vertex = this->diagram.findVertex(KE::TwoD::FloatPoint(m->x(), m->y()), 17);
			if (vertex) {
				this->diagram.removeVertex(vertex);
				if (Parent->isEmpty()) {
					Parent->clear();
				} else {
					repaint();
					Parent->isSaved = false;
				}
			}
			return;
		}
		case FLIP_CROSSING:
		{
			auto c = this->diagram.findCrossing(KE::TwoD::FloatPoint(m->x(), m->y()), 17);
			if (c) {
				this->diagram.flipCrossing(*c);
				repaint();
				Parent->isSaved = false;
			}
			return;
		}
		case MOVE_DIAGRAM:
			localx = m->x();
			localy = m->y();
			Parent->isSaved = false;
			doSomething = true;
			return;
		default:
			return;
	}
}

void DiagramWidget::mouseReleaseEvent(QMouseEvent *m) {
	if (!doSomething)
		return;

	doSomething = false;

	switch (Parent->mode) {
		case ADD_VERTEX:
		case MOVE_VERTEX:
		case NEW_DIAGRAM:
			this->diagram.moveVertex(localVertex, m->x(), m->y());
			repaint();
			return;
		case MOVE_DIAGRAM:
			this->diagram.shift(m->x() - localx, m->y() - localy);
			localx = m->x();
			localy = m->y();
			repaint();
			return;
		default:
			return;
	}
}

void DiagramWidget::mouseMoveEvent(QMouseEvent *m) {
	if (!doSomething)
		return;

	switch (Parent->mode) {
		case ADD_VERTEX:
		case MOVE_VERTEX:
		case NEW_DIAGRAM:
			this->diagram.moveVertex(localVertex, m->x(), m->y());
			repaint();
			return;
		case MOVE_DIAGRAM:
			this->diagram.shift(m->x() - localx, m->y() - localy);
			repaint();
			localx = m->x();
			localy = m->y();
			return;
		default:
			return;
	}
}
