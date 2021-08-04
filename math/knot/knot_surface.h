#ifndef __KNOT_SURFACE_H__
#define __KNOT_SURFACE_H__

#include <vector>

#include "../../gl/surface/surface.h"

namespace KE {

namespace ThreeD {

class Knot;

}

namespace GL {

class KnotSurface : public Surface {

private:
	const ThreeD::Knot &knot;
  double thickness;
	const std::vector<double> sines;
	const std::vector<double> cosines;

public:
  KnotSurface(const ThreeD::Knot &knot, double thickness, std::size_t pointsOnCircle);
  void calculate(void);
  void setThickness(double t) {thickness = t;}

private:
	KnotSurface(const KnotSurface&) = delete;
	KnotSurface& operator = (const KnotSurface&) = delete;
};

}}

#endif /* __KNOT_SURFACE_H__ */
