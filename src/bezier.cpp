/**
 * Introduction to Computer Graphics 2019
 * Final Project: Random Tilemap Generation and Rendering
 *
 * Authors: Antoine Crettenand, Louis Vialar, François Quellec
 */

#include "bezier.h"
#include <utility>
#include <cmath>
#include <array>

// Calculate a point one of the Bezier curve segments
// @param bp_offset  index into bezier_control_points_ of the first of four
//                   control points defining the Bezier segment we want to evaluate.
// @param t          parametric distance along the curve at which to evaluate
vec3 PiecewiseBezier::eval_bezier(int bp_offset, float t) const {
    auto b = bezier_control_points_[bp_offset];

    return b * eval_bezier_polynom(bp_offset, bezier_control_points_.size() - 1, t);
}

float PiecewiseBezier::eval_bezier_polynom(int i, int n, float t) const {
	if (n == 0 && i == 0) {
		return 1.0;
	} else if (i < 0 || i > n) {
		return 0.0;
	}

	return eval_bezier_polynom(i, n - 1, t) * (1 - t) + t * eval_bezier_polynom(i - 1, n - 1, t);
}

std::vector<vec3> PiecewiseBezier::control_polygon_to_bezier_points(std::vector<vec3> const& cp) {
    std::vector<vec3> bezier_pts;
    size_t numSegments = cp.size() - 3;
    bezier_pts.resize(3 * numSegments + 1);

    return bezier_pts;
}

vec3 PiecewiseBezier::eval_piecewise_bezier_curve(float t) const {
	while (t > 1.0) t -= 1.0;
	while (t < 0.0) t += 1.0;

	if (bezier_control_points_.size() == 1)
		return bezier_control_points_[0];

	vec3 sum = vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < bezier_control_points_.size(); ++i) {
		sum += eval_bezier(i, t);
	}
    return sum;
}

vec3 PiecewiseBezier::operator()(float t) const {
    return eval_piecewise_bezier_curve(t);
}

void PiecewiseBezier::set_control_polygon(const std::vector<vec3> &control_polygon, bool loop) {
    control_polygon_ = control_polygon;
    if(loop) {
        control_polygon_.push_back(control_polygon[0]);
        control_polygon_.push_back(control_polygon[1]);
        control_polygon_.push_back(control_polygon[2]);
    }

    bezier_control_points_ = control_polygon_;


}
