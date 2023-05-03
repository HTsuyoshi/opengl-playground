#ifndef SHAPES_H
#define SHAPES_H

#include <vector>

namespace Shape
{
	std::vector<float> const& cube(float x, float y, float z, float offset_x, float offset_y, float offset_z);
	std::vector<float> const& cube(float size, float offset_x, float offset_y, float offset_z);
	std::vector<float> const& pyramid(float x, float y, float z, float offset_x, float offset_y, float offset_z);
	std::vector<float> const& pyramid(float size, float offset_x, float offset_y, float offset_z);
	std::vector<float> const& plane(float x, float z, float offset_x, float offset_y, float offset_z);
	std::vector<float> const& plane(float size, float offset_x, float offset_y, float offset_z);
};
#endif
