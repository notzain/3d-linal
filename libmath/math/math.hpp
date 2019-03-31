#pragma once

#include "matrix.hpp"
#include "vector.hpp"

namespace math {
vector multiply(const vector &vec, const matrix &mat);
matrix make_identity();
matrix make_rotation_x(float angle);
matrix make_rotation_z(float angle);
matrix make_rotation_y(float angle);

matrix make_translation(const vector &translation);
matrix make_scaling(const math::vector &scale);
matrix make_projection(float fov, float aspect_ratio, float near, float far);

matrix inverse(const matrix &matrix);
matrix point_at(const math::vector &camera, const math::vector &target,
                const math::vector &up);

vector rotation_to_direction(const math::vector &dir, const math::vector& rot);

float to_radians(float degrees);
float to_degrees(float radians);

float dot_product(const vector &a, const vector &b);
} // namespace math