#include "diagram.h"

namespace KE { namespace TwoD {

bool Diagram::tryAddCrossing(vertex *v1, vertex *v2) {
	const Edge e1(v1, v1->next());
	const Edge e2(v2, v2->next());
	if (!e1.intersects(e2)) {
		return false;
	}

	new crossing (v1, v2);

	return true;
}

bool Diagram::tryChangeCrossing(vertex *v1, vertex *v2) {
	if (tryRemoveCrossing (v2, v1)) {
		return tryAddCrossing (v1, v2);
	}

	return false;
}

bool Diagram::tryRemoveCrossing(vertex *v1, vertex *v2) {
	for (auto crs : v1->crossings()) {
		if (crs->up() == v2) {
			delete crs;
			return true;
		}
	}

	return false;
}

bool Diagram::isCrossing(vertex *v1, vertex *v2) {
	for (auto crs : v1->crossings()) {
		if (crs->up() == v2) {
			return true;
		}
	}

	return false;
}

}}
