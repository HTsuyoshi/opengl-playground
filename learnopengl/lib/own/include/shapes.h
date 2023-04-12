#ifndef SHAPES_H
#define SHAPES_H

#include <vector>

namespace Shape
{
	static std::vector<float> cube_shape(float size_x_neg, float size_x_pos, float size_y_neg, float size_y_pos, float size_z_neg, float size_z_pos);
	std::vector<float> const& cube(float x, float y, float z, float offset_x, float offset_y, float offset_z);
	std::vector<float> const& cube_shape(float size, float offset_x, float offset_y, float offset_z);
	static std::vector<float> pyramid_shape(float size_x_neg, float size_x_pos, float size_y_neg, float size_y_pos, float size_z_neg, float size_z_pos);
	std::vector<float> const& pyramid(float x, float y, float z, float offset_x, float offset_y, float offset_z);
	std::vector<float> const& pyramid(float size, float offset_x, float offset_y, float offset_z);
	static std::vector<float> plane_shape(float size_x_neg, float size_x_pos, float size_y_neg, float size_y_pos, float size_z_neg, float size_z_pos);
	std::vector<float> const& plane(float x, float z, float offset_x, float offset_y, float offset_z);
	std::vector<float> const& plane(float size, float offset_x, float offset_y, float offset_z);
};
#endif
