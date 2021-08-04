#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPixmap>

#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

#include "diagramWindow.h"
#include "../knotWindow/knotWindow.h"

diagramWindow::diagramWindow(const rapidjson::Document &doc) : diagram(doc) {
	init();
	setWindowTitle(this->diagram.caption.c_str());
	actions[0]->setChecked(false);
	actions_convert->setEnabled(true);
	actions_simplify->setEnabled(true);
	actions_clear->setEnabled(true);
}

diagramWindow::diagramWindow() {
	init();
	setWindowTitle(this->diagram.caption.c_str());
}

void diagramWindow::init() {
	actionsMenu = this->menuBar()->addMenu("&Actions");
	actions_convert = actionsMenu->addAction("&Convert", this, SLOT(convert()));
	actions_simplify = actionsMenu->addAction("&Simplify", this, SLOT(simplify()));
	actions_clear = actionsMenu->addAction("C&lear", this, SLOT(clear()));
	
	addToolBarSeparator();
	addToolBarButton("mini_trefoil.xpm", "Convert to knot", SLOT(convert()));
	addToolBarSeparator();
	addToolBarButton("clear.xpm", "Clear", SLOT(clear()));

	addToolBarSeparator();
	actions = new QToolButton*[editorModeNumber];
	actions[0] = addToolBarButton("draw_diagram.xpm", "Draw diagram", 0);
	actions[1] = addToolBarButton("add_point.xpm", "Add point", 0);
	actions[2] = addToolBarButton("move_point.xpm", "Move point", 0);
	actions[3] = addToolBarButton("delete_point.xpm", "Delete point", 0);
	actions[4] = addToolBarButton("change_crossing.xpm", "Change crossing", 0);
	actions[5] = addToolBarButton("move_diagram.xpm", "Move diagram", 0);

	QButtonGroup *grp = new QButtonGroup;
	for (int i = 0; i < editorModeNumber; i++) {
		actions[i]->setCheckable(true);
		grp->addButton(actions[i]);
		this->connect(actions[i], &QToolButton::pressed, [=](){ this->setmode(i); });
	}

	grp->setExclusive(false);

	mode = DRAW_NEW_DIAGRAM;
	actions[0]->setChecked(true);

	actions_convert->setEnabled(this->diagram.isClosed());
	actions_simplify->setEnabled(this->diagram.isClosed());
	actions_clear->setEnabled(!isEmpty());

	setCentralWidget(new diagramMainWidget(this));

	setWindowIcon(QPixmap((QString) getenv("KNOTEDITOR_PIXMAPS") + "/diagram.xpm"));

	complete();
}

diagramWindow::~diagramWindow() {
	delete actionsMenu;
	delete[] actions;
}

void diagramWindow::setmode(int newmode) {
	if (newmode < 0 || newmode >= editorModeNumber) {
		return;
	}

	int oldmode = this->mode;

	if (newmode == oldmode) {
		actions[newmode]->toggle();
		return;
	}

	if (newmode == DRAW_NEW_DIAGRAM && this->diagram.isClosed()) {
		actions[newmode]->toggle();
		return;
	}

	if (newmode != DRAW_NEW_DIAGRAM && isEmpty()) {
		actions[newmode]->toggle();
		return;
	}

	this->mode = (editorMode)newmode;

	actions[oldmode]->setChecked(false);
}

void diagramWindow::clear() {
	this->diagram.clear();
	actions_convert->setEnabled(false);
	actions_simplify->setEnabled(false);
	actions_clear->setEnabled(false);
	centralWidget()->repaint();
	setmode(DRAW_NEW_DIAGRAM);
	actions[0]->setChecked(true);

	isSaved = true;
}

void diagramWindow::convert() {
	if (!this->diagram.isClosed()) {
		QMessageBox::critical(this, "Error", "\nCannot convert non-closed diagram.\n");
		return;
	}

	if (this->diagram.vertices().size() <= 2) {
		QMessageBox::critical(this, "Error", "\nCannot convert diagram with less than three points.\n");
		return;
	}

	(new knotWindow(*this))->show();
}

void diagramWindow::saveIt(std::ostream &os) {
	const rapidjson::Document doc = this->diagram.save();
	rapidjson::OStreamWrapper wrapper(os);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(wrapper);
	doc.Accept(writer);
}
