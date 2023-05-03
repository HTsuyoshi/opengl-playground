#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

namespace Shape
{
	static std::vector<float> cube_shape(float size_x_neg, float size_x_pos, float size_y_neg, float size_y_pos, float size_z_neg, float size_z_pos)
	{
		float cube[] =
		{
			// Vertex                            Texture       Normal
			size_x_neg, size_y_neg, size_z_neg,  0.0f, 0.0f,   0.0f, 0.0f, -1.0f,
			size_x_pos, size_y_neg, size_z_neg,  1.0f, 0.0f,   0.0f, 0.0f, -1.0f,
			size_x_pos, size_y_pos, size_z_neg,  1.0f, 1.0f,   0.0f, 0.0f, -1.0f,
			size_x_pos, size_y_pos, size_z_neg,  1.0f, 1.0f,   0.0f, 0.0f, -1.0f,
			size_x_neg, size_y_pos, size_z_neg,  0.0f, 1.0f,   0.0f, 0.0f, -1.0f,
			size_x_neg, size_y_neg, size_z_neg,  0.0f, 0.0f,   0.0f, 0.0f, -1.0f,

			size_x_neg, size_y_neg, size_z_pos,  0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
			size_x_pos, size_y_neg, size_z_pos,  1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
			size_x_pos, size_y_pos, size_z_pos,  1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
			size_x_pos, size_y_pos, size_z_pos,  1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
			size_x_neg, size_y_pos, size_z_pos,  0.0f, 1.0f,   0.0f, 0.0f, 1.0f,
			size_x_neg, size_y_neg, size_z_pos,  0.0f, 0.0f,   0.0f, 0.0f, 1.0f,

			size_x_neg, size_y_pos, size_z_pos,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
			size_x_neg, size_y_pos, size_z_neg,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
			size_x_neg, size_y_neg, size_z_neg,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
			size_x_neg, size_y_neg, size_z_neg,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
			size_x_neg, size_y_neg, size_z_pos,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
			size_x_neg, size_y_pos, size_z_pos,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,

			size_x_pos, size_y_pos, size_z_pos,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,
			size_x_pos, size_y_pos, size_z_neg,  1.0f, 1.0f,   1.0f, 0.0f, 0.0f,
			size_x_pos, size_y_neg, size_z_neg,  0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
			size_x_pos, size_y_neg, size_z_neg,  0.0f, 1.0f,   1.0f, 0.0f, 0.0f,
			size_x_pos, size_y_neg, size_z_pos,  0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
			size_x_pos, size_y_pos, size_z_pos,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,

			size_x_neg, size_y_neg, size_z_neg,  0.0f, 1.0f,   0.0f, -1.0f, 0.0f,
			size_x_pos, size_y_neg, size_z_neg,  1.0f, 1.0f,   0.0f, -1.0f, 0.0f,
			size_x_pos, size_y_neg, size_z_pos,  1.0f, 0.0f,   0.0f, -1.0f, 0.0f,
			size_x_pos, size_y_neg, size_z_pos,  1.0f, 0.0f,   0.0f, -1.0f, 0.0f,
			size_x_neg, size_y_neg, size_z_pos,  0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
			size_x_neg, size_y_neg, size_z_neg,  0.0f, 1.0f,   0.0f, -1.0f, 0.0f,

			size_x_neg, size_y_pos, size_z_neg,  0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
			size_x_pos, size_y_pos, size_z_neg,  1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
			size_x_pos, size_y_pos, size_z_pos,  1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
			size_x_pos, size_y_pos, size_z_pos,  1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
			size_x_neg, size_y_pos, size_z_pos,  0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
			size_x_neg, size_y_pos, size_z_neg,  0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
		};
		
		return *new std::vector<float>(cube, cube + sizeof(cube)/sizeof(cube[0]));
	}

	std::vector<float> const& cube(float x, float y, float z, float offset_x, float offset_y, float offset_z)
	{
		float size_x_pos =  x/2 + offset_x,
			  size_x_neg = -x/2 + offset_x,
			  size_y_pos =  y/2 + offset_y,
			  size_y_neg = -y/2 + offset_y,
			  size_z_pos =  z/2 + offset_z,
			  size_z_neg = -z/2 + offset_z;
		return *new std::vector<float>(cube_shape(size_x_neg, size_x_pos, size_y_neg, size_y_pos, size_z_neg, size_z_pos));
	}

	std::vector<float> const& cube(float size, float offset_x, float offset_y, float offset_z)
	{
		float size_x_pos =  size/2 + offset_x,
			  size_x_neg = -size/2 + offset_x,
			  size_y_pos =  size/2 + offset_y,
			  size_y_neg = -size/2 + offset_y,
			  size_z_pos =  size/2 + offset_z,
			  size_z_neg = -size/2 + offset_z;
		return *new std::vector<float>(cube_shape(size_x_neg, size_x_pos, size_y_neg, size_y_pos, size_z_neg, size_z_pos));
	}

	static std::vector<float> pyramid_shape(float size_x_neg, float size_x_pos, float size_y_neg, float size_y_pos, float size_z_neg, float size_z_pos)
	{
		float pyramid[] = {
			// Vertex                            Texture       Normal
			size_x_neg, size_y_neg, size_z_neg,  0.0f, 0.0f,   0.0f, -1.0f,  0.0f,
			size_x_pos, size_y_neg, size_z_neg,  1.0f, 0.0f,   0.0f, -1.0f,  0.0f,
			size_x_pos, size_y_neg, size_z_pos,  1.0f, 1.0f,   0.0f, -1.0f,  0.0f,
			size_x_pos, size_y_neg, size_z_pos,  1.0f, 1.0f,   0.0f, -1.0f,  0.0f,
			size_x_neg, size_y_neg, size_z_pos,  0.0f, 1.0f,   0.0f, -1.0f,  0.0f,
			size_x_neg, size_y_neg, size_z_neg,  0.0f, 0.0f,   0.0f, -1.0f,  0.0f,

			size_x_neg, size_y_neg, size_z_neg,  0.0f, 0.0f,  -1.0f,  0.5f,  0.0f,
			0.0f,       size_y_pos, 0.0f,        0.5f, 1.0f,  -1.0f,  0.5f,  0.0f,
			size_x_neg, size_y_neg, size_z_pos,  1.0f, 0.0f,  -1.0f,  0.5f,  0.0f,

			size_x_neg, size_y_neg, size_z_pos,  0.0f, 0.0f,   0.0f,  0.5f,  1.0f,
			0.0f,       size_y_pos, 0.0f,        0.5f, 1.0f,   0.0f,  0.5f,  1.0f,
			size_x_pos, size_y_neg, size_z_pos,  1.0f, 0.0f,   0.0f,  0.5f,  1.0f,

			size_x_pos, size_y_neg, size_z_pos,  0.0f, 0.0f,   1.0f,  0.5f,  0.0f,
			0.0f,       size_y_pos, 0.0f,        0.5f, 1.0f,   1.0f,  0.5f,  0.0f,
			size_x_pos, size_y_neg, size_z_neg,  1.0f, 0.0f,   1.0f,  0.5f,  0.0f,

			size_x_pos, size_y_neg, size_z_neg,  0.0f, 0.0f,   0.0f,  0.5f, -1.0f,
			0.0f,       size_y_pos, 0.0f,        0.5f, 1.0f,   0.0f,  0.5f, -1.0f,
			size_x_neg, size_y_neg, size_z_neg,  1.0f, 0.0f,   0.0f,  0.5f, -1.0f,
		};
		
		return *new std::vector<float>(pyramid, pyramid + sizeof(pyramid)/sizeof(pyramid[0]));
	}

	std::vector<float> const& pyramid(float x, float y, float z, float offset_x, float offset_y, float offset_z)
	{
		float size_x_pos =  x/2 + offset_x,
			  size_x_neg = -x/2 + offset_x,
			  size_y_pos =  y/2 + offset_y,
			  size_y_neg = -y/2 + offset_y,
			  size_z_pos =  z/2 + offset_z,
			  size_z_neg = -z/2 + offset_z;
		return *new std::vector<float>(pyramid_shape(size_x_neg, size_x_pos, size_y_neg, size_y_pos, size_z_neg, size_z_pos));
	}

	std::vector<float> const& pyramid(float size, float offset_x, float offset_y, float offset_z)
	{
		float size_x_pos =  size/2 + offset_x,
			  size_x_neg = -size/2 + offset_x,
			  size_y_pos =  size/2 + offset_y,
			  size_y_neg = -size/2 + offset_y,
			  size_z_pos =  size/2 + offset_z,
			  size_z_neg = -size/2 + offset_z;
		return *new std::vector<float>(pyramid_shape(size_x_neg, size_x_pos, size_y_neg, size_y_pos, size_z_neg, size_z_pos));
	}

	static std::vector<float> plane_shape(float size_x_neg, float size_x_pos, float size_y_neg, float size_y_pos, float size_z_neg, float size_z_pos)
	{
		float plane[] =
		{
			// Vertex                      Texture       Normal
			size_x_neg, size_y_neg, size_z_neg,  0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
			size_x_pos, size_y_neg, size_z_neg,  1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
			size_x_pos, size_y_neg, size_z_pos,  1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
			size_x_pos, size_y_neg, size_z_pos,  1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
			size_x_neg, size_y_neg, size_z_pos,  0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
			size_x_neg, size_y_neg, size_z_neg,  0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
		};
		return *new std::vector<float>(plane, plane + sizeof(plane)/sizeof(plane[0]));
	}

	std::vector<float> const& plane(float x, float z, float offset_x, float offset_y, float offset_z)
	{
		float size_x_pos =  x/2 + offset_x,
			  size_x_neg = -x/2 + offset_x,
			  size_y_pos =  z/2 + offset_y,
			  size_y_neg = -z/2 + offset_y,
			  size_z_pos =  z/2 + offset_z,
			  size_z_neg = -z/2 + offset_z;
		return *new std::vector<float>(plane_shape(size_x_neg, size_x_pos, size_y_neg, size_y_pos, size_z_neg, size_z_pos));
	}

	std::vector<float> const& plane(float size, float offset_x, float offset_y, float offset_z)
	{
		float size_x_pos =  size/2 + offset_x,
			  size_x_neg = -size/2 + offset_x,
			  size_y_pos =  size/2 + offset_y,
			  size_y_neg = -size/2 + offset_y,
			  size_z_pos =  size/2 + offset_z,
			  size_z_neg = -size/2 + offset_z;
		return *new std::vector<float>(plane_shape(size_x_neg, size_x_pos, size_y_neg, size_y_pos, size_z_neg, size_z_pos));
	}

};
