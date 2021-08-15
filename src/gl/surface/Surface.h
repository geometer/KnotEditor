#ifndef __SURFACE_H__
#define __SURFACE_H__

#include <vector>

#include "../../math/knot/primitives.h"

namespace KE { namespace GL {

struct Color {
	float rgb[3];

	Color(int red, int green, int blue) : rgb {red / 255.0f, green / 255.0f, blue / 255.0f} {}
};

class Surface {

private:
	struct SurfacePoint {
		float vertex[3];
		float normal[3];

		SurfacePoint(float v0, float v1, float v2, float n0, float n1, float n2) {
			vertex[0] = v0;
			vertex[1] = v1;
			vertex[2] = v2;
			normal[0] = n0;
			normal[1] = n1;
			normal[2] = n2;
		}
	};

	const bool stripped;
	const bool showBackSide;
	bool visible;

	std::vector<SurfacePoint> points;

protected:
	void destroy();
	virtual void calculate() = 0;
	void addpoint(const ThreeD::Point &vertex, const ThreeD::Vector &normal);

public:
	Surface(bool stripped, bool showBackSide);
	virtual ~Surface();

	bool isVisible() { return this->visible; }
	void show() { this->visible = true; }
	void hide() { this->visible = false; }

	void paint();

	virtual const Color &getFrontColor() const = 0;
	virtual const Color &getBackColor() const = 0;

private:
	Surface(const Surface&) = delete;
	Surface& operator = (const Surface&) = delete;
};

}}

#endif /* __SURFACE_H__ */
