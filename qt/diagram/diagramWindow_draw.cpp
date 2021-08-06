#include <cmath>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>

#include "diagramWindow.h"

DiagramWidget::DiagramWidget(diagramWindow *p) : QWidget(p) {
	Parent = p;
}

DiagramWidget::DiagramWidget(diagramWindow *p, const rapidjson::Document &doc) : QWidget(p), diagram(doc) {
	Parent = p;
}

void DiagramWidget::drawVertex(QPainter &painter, const KE::TwoD::Diagram::Vertex &v) {
	painter.drawEllipse(v.x() - 4, v.y() - 4, 9, 9);
}

void DiagramWidget::drawEdge(QPainter &painter, const KE::TwoD::Diagram::Edge &edge) {
	float deltaX = edge.end->x() - edge.start->x();
	float deltaY = edge.end->y() - edge.start->y();
	float hyp = hypotf(deltaX, deltaY);

	deltaX = 10 * deltaX / hyp;
	deltaY = 10 * deltaY / hyp;

	float x0 = edge.start->x(),
				y0 = edge.start->y(),
				x1, y1;

	for (const auto &crs : this->diagram.crossings(edge)) {
		auto coords = crs.coords();
		if (!coords) {
			continue;
		}
		x1 = coords->x - deltaX;
		y1 = coords->y - deltaY;

		if ((x1 - x0) * deltaX + (y1 - y0) * deltaY > 0) {
			painter.drawLine(QPointF(x0, y0), QPointF(x1, y1));
		}

		x0 = coords->x + deltaX;
		y0 = coords->y + deltaY;
	}

	x1 = edge.end->x();
	y1 = edge.end->y();

	if ((x1 - x0) * deltaX + (y1 - y0) * deltaY > 0) {
		painter.drawLine(QPointF(x0, y0), QPointF(x1, y1));
	}
}

void DiagramWidget::drawIt(QPainter &painter) {
	painter.setPen(Qt::black);
	painter.setBrush(Qt::black);
	
	for (auto vertex : this->diagram.vertices()) {
		drawVertex(painter, *vertex);
	}
	for (const auto &edge : this->diagram.edges()) {
		drawEdge(painter, edge);
	}
}

void DiagramWidget::paintEvent(QPaintEvent*) {
	QPainter pnt;
	pnt.begin(this);
	pnt.setRenderHint(QPainter::Antialiasing);
	pnt.fillRect(0, 0, this->width(), this->height(), Qt::white);
	drawIt(pnt);
	pnt.end();
}

void diagramWindow::printIt(QPrinter *prn) {
	QPainter pnt;
	pnt.begin(prn);
	this->diagramWidget()->drawIt(pnt);
	pnt.end();
}
